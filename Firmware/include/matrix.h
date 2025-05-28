/**

Matrix Layout
x/y 0  1  2  3  4  5
 0     #  #  #  #
 1  #  #  #  #  #  #
 2  #  #  #  #  #  #
 3  #  #  #  #  #  #
 4  #  #  #  #  #  #
 5     #  #  #  #
 6  S  G  L  K  P  W

Legend:
    Empty -> No LED
    # -> Matrix LED
Arms:
    S -> Soldering iron
    G -> Gears
    L -> "Led"
    K -> Key
    P -> PC
    W -> WiFi

**/

#include <stdint.h>

#ifndef _MATRIX_H
#define _MATRIX_H

#define WIDTH 6
#define HEIGHT 7
// PIXS must equal WIDTH * HEIGHT
#define PIXS 42

#define MAX_BRIGHTNESS 15

void matrixInit();
void matrixPowerOff();
void matrixSetPixel(uint8_t x, uint8_t y, uint8_t v);
uint8_t matrixGetPixel(uint8_t x, uint8_t y);
void matrixDisplay();
void matrixClear();

#endif
