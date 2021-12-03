/*
 * font8x8_basic.h
 *
 *  Created on: 2017/05/03
 *      Author: yanbe
 */

#ifndef FONT_H_
#define FONT_H_

#include <stdio.h>

/*
   Constant: font8x8_basic_tr
   Contains an 90 digree transposed 8x8 font map for unicode points 
   U+0000 - U+007F (basic latin)
   
   To make it easy to use with SSD1306's GDDRAM mapping and API,
   this constant is an 90 degree transposed.
   The original version written by Marcel Sondaar is availble at:
   https://github.com/dhepper/font8x8/blob/master/font8x8_basic.h 
*/

#define FONT_SMALL_WIDTH    (8)

extern uint8_t font8x8_basic_tr[96][8];


#endif /* FONT_H_ */


