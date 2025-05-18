#include <stdint.h>
#include "matrix.h"
#include "ch32fun.h"

// Initializes the pixel buffer with as many bits as required to
// store all WIDTH*HEIGHT pixels (to the next integer bytes)
uint8_t pixbuf[HEIGHT][WIDTH];

// this array contains pre calculated values for the conf register
// for every pixel, which are calculated once on init to save
// calculation time while displaying the pixels
uint32_t conf_reg_c_precalc[HEIGHT][WIDTH];
uint32_t out_reg_c_precalc[HEIGHT][WIDTH];

uint8_t col;
uint8_t row;

/**
 * @brief Initializes the matrix
 */
void matrixInit() {
    // pre calculate conf register
    uint8_t row_pin_index = 0, col_pin_index = 0;
    for (row = 0; row < HEIGHT; row ++) {
        row_pin_index = row;
        if (row_pin_index >= 5) row_pin_index ++; // pin PC5 doesn't exist

        for (col = 0; col < WIDTH; col ++) {
            conf_reg_c_precalc[row][col] = 0; // clear the register
            if ((row == 0 || row == 5) && (col == 0 || col == 5)) continue; // non-existing leds

            col_pin_index = col;
            if (col_pin_index >= row_pin_index) col_pin_index ++;
            if (col_pin_index >= 5) col_pin_index ++;

            conf_reg_c_precalc[row][col] = (uint32_t)2<<(4*row_pin_index) | // row pin as output
                                           (uint32_t)2<<(4*col_pin_index);  // col pin as output
            
            out_reg_c_precalc[row][col] = (uint32_t)1<<col_pin_index; // col pin high

            
        }
    }
    row = 0;
    col = 0;
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


uint32_t out_register;
uint8_t tmp;
uint8_t cur_pix;

/**
 * @brief Displays the matrix contents
 */
void matrixDisplay() {
    GPIOC->CFGLR = conf_reg_c_precalc[row][col];
    out_register = out_reg_c_precalc[row][col];
    
    cur_pix = pixbuf[row][col];
    
    // soft-pwm 
    for (tmp = 0; tmp <= MAX_BRIGHTNESS; tmp ++) {
        GPIOC->OUTDR = (tmp < cur_pix) ? out_register : 0;
        //asm volatile( "c.nop;c.nop;" );
    }

    // prepare row/col addrs for next call
    if (++col == WIDTH) {
        col = 0;
        if (++row == HEIGHT) {
            row = 0;
        }
    }
}