#include <stdint.h>
#include "matrix.h"
#include "ch32fun.h"

// Initializes the pixel buffer with as many bits as required to
// store all WIDTH*HEIGHT pixels (to the next integer bytes)
uint8_t pixbuf[HEIGHT][WIDTH];

/**
 * @brief Initializes the matrix
 */
void matrixInit() {
}

/**
 * @brief turns off all LEDs
 */
void matrixPowerOff() {
    GPIOC->CFGLR = 0;
    GPIOC->OUTDR = 0;
}

/**
 * @brief Sets the pixel at x, y to v (0/1)
 */
void matrixSetPixel(uint8_t x, uint8_t y, uint8_t v) {
    if (x >= WIDTH || y >= HEIGHT) return;
    if (x == 0 && (y == 0 || y == 5)) return; // non-existing leds
    if (x == 5 && (y == 0 || y == 5)) return; // non-existing leds
    pixbuf[y][x] = v;
}

/**
 * @brief Returns the pixel at x, y
 */
uint8_t matrixGetPixel(uint8_t x, uint8_t y) {
    if (x >= WIDTH || y >= HEIGHT) return 0;
    return pixbuf[y][x];
}

/**
 * @brief clears the matrix
 */
void matrixClear() {
    for (uint8_t y = 0; y < HEIGHT; y ++){
        for (uint8_t x = 0; x < HEIGHT; x ++) {
            matrixSetPixel(x, y, 0);
        }
    }
}

uint32_t conf_register;
uint32_t out_register;
uint8_t pin_index;
uint8_t col;
uint8_t row;
uint8_t tmp;
uint8_t cur_pix;

/**
 * @brief Displays the matrix contents
 */
void matrixDisplay() {
    // 4 Bit for each IO pin in the conf register
    // set the 1st bit to make it equiv to (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)

    cur_pix = matrixGetPixel(row, col);

    tmp = col;
    if (tmp >= 5) tmp ++;
    conf_register = (uint32_t)2<<(4*tmp);
    
    if (row == col) pin_index ++;
    if (pin_index == 5) pin_index ++;

    conf_register |= (uint32_t)2<<(4*pin_index);
    out_register = (uint32_t)1 << pin_index;
    
    GPIOC->CFGLR = conf_register;
    // soft-pwm
    for (tmp = 0; tmp < MAX_BRIGHTNESS; tmp ++)
        GPIOC->OUTDR = (tmp < cur_pix) ? out_register : 0;

    GPIOC->OUTDR = 0;
    
    // prepare row/col addrs for next call
    row ++;
    pin_index ++;
    if (row >= WIDTH) {
        row = 0;
        pin_index = 0;
        col ++;
        if (col >= HEIGHT) {
            col = 0;
        }
    }
}