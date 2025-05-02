#include <stdint.h>
#include "matrix.h"
#include "ch32fun.h"

// Initializes the pixel buffer with as many bits as required to
// store all WIDTH*HEIGHT pixels (to the next integer bytes)
uint8_t pixbuf[HEIGHT][WIDTH];

uint32_t conf_register_clean = 0;

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

uint32_t conf_register;
uint32_t out_register;
uint8_t pin_index;
uint8_t col;
uint8_t row;
uint8_t tmp;

/**
 * @brief Displays the matrix contents
 */
void matrixDisplay() {
    // 4 Bit for each IO pin in the conf register
    // set the 1st bit to make it equiv to (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)
    pin_index = col;
    if (pin_index >= 5) pin_index ++;
    conf_register = conf_register_clean;
    conf_register &= ~(0xf << (4*pin_index));
    conf_register |= (uint32_t)2<<(4*pin_index);
    
    out_register = 0;
    pin_index = 0;
    row = 0;

    while (row < WIDTH) {
        if (row == col) pin_index ++;
        if (pin_index == 5) pin_index ++;

        if (matrixGetPixel(row, col)) {
            conf_register &= ~(0xf << (4*pin_index));
            conf_register |= (uint32_t)2<<(4*pin_index);
            out_register |= (uint32_t)1 << pin_index;
        }
        row ++;
        pin_index ++;
    }

    GPIOC->CFGLR = conf_register;
    GPIOC->OUTDR = out_register;
    
    col ++;
    if (col >= HEIGHT) col = 0;
}