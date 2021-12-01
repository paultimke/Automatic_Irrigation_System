#include "images.h"

uint8_t granja_hogar_glcd_bmp[]={
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xe0, 0x60, 0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0xf8, 0xfc, 0xfc, 0x7c, 0x9c, 0xdc, 0xe8, 0xf0, 0xf0, 0xf0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xc0, 0xe0, 0xf0, 0xf0, 0xf0, 0xe0, 0xe3, 0xc7, 0xf, 0x1f, 0x1f, 0x3e, 0x39, 0x37, 0x2f, 0x3f, 0x1f, 0xf, 0x01, 0x00, 0x00, 0x00, 0x03, 0xf, 0x07, 0x1e, 0x1f, 0x1f, 0x1f, 0x1f, 0xf, 0x8f, 0xc7, 0xe3, 0xe0, 0xe0, 0xf0, 0xf0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x70, 0xf8, 0xf9, 0xf7, 0xf7, 0xef, 0xef, 0xff, 0xff, 0x7f, 0x7f, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x37, 0x7b, 0xfb, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xf9, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0xe1, 0xf1, 0xf9, 0xf9, 0xf8, 0xf8, 0xf0, 0xf0, 0xf0, 0x60, 0xa0, 0xc0, 0x10, 0xe0, 0xc3, 0x8f, 0x9f, 0x1f, 0x3f, 0x3f, 0x1f, 0x9f, 0xcf, 0xc7, 0x61, 0x18, 0x04, 0xe0, 0xd0, 0xd0, 0xf0, 0xb8, 0xb8, 0x78, 0xf9, 0xf9, 0xf3, 0xf1, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x37, 0x7f, 0xfb, 0xfb, 0xfd, 0x7c, 0x3f, 0x1f, 0x03, 0x00, 0x00, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x07, 0x1f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x3e, 0x19, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xc, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0x08, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 
};


uint8_t clear_page[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


uint8_t yodita_glcd_bmp[]={
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xe, 0x1e, 0x32, 0xe2, 0x86, 0x14, 0x14, 0x14, 0x2c, 0x2c, 0xe8, 0xc8, 0x8c, 0xcc, 0xc6, 0xc6, 0xd2, 0x33, 0x63, 0x33, 0x03, 0x53, 0xb2, 0xd2, 0xc6, 0xc4, 0x8c, 0xc8, 0xc8, 0x48, 0x68, 0x28, 0x28, 0x28, 0x28, 0x2c, 0x84, 0xc4, 0x6c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0xc, 0xc, 0x08, 0xb8, 0xf9, 0xd9, 0x93, 0x13, 0x13, 0xf3, 0x33, 0x18, 0x18, 0x18, 0x13, 0x93, 0x93, 0x93, 0xdb, 0xd9, 0xe8, 0x38, 0xb8, 0xf8, 0xdc, 0xc, 0x04, 0x06, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x15, 0xfa, 0xfc, 0x5b, 0x2f, 0x03, 0xfe, 0x02, 0xfe, 0x03, 0x1b, 0xd, 0x06, 0x1f, 0xff, 0xf8, 0xb, 0xf, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x06, 0x06, 0x02, 0x07, 0x06, 0x07, 0x06, 0x06, 0x06, 0x06, 0x02, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 
};
 