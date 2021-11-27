#ifndef IMAGES_H
#define IMAGES_H

#include <stdio.h>

#define GRANJA_HOGAR_GLCD_HEIGHT 45 
#define GRANJA_HOGAR_GLCD_WIDTH  50 

#define LA_ROCA_GLCD_HEIGHT 55 
#define LA_ROCA_GLCD_WIDTH  88 

#define YODITA_GLCD_HEIGHT 43 
#define YODITA_GLCD_WIDTH  50 

extern uint8_t granja_hogar_glcd_bmp[];
extern uint8_t clear_page[128];
extern uint8_t la_roca_glcd_bmp[];
extern uint8_t yodita_glcd_bmp[];

#endif //IMAGES_H