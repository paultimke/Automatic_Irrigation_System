#ifndef HAL_DISPLAY_H
#define HAL_DISPLAY_H

/*-----------------------Includes------------------------*/
#include <stdio.h>
#include <string.h>
#include "usr_i2c.h"
#include "font.h"
#include "images.h"

/*------------------------Macros-------------------------*/
// SLA (0x3C) + WRITE_MODE (0x00) =  0x78 (0b01111000)
#define OLED_I2C_ADDRESS    (0x3C)

#define OLED_WIDTH          (128)
#define OLED_HEIGHT         (64)
#define OLED_MAX_PAGE       (8)
#define OLED_DISPLAY_BITS   (8)
#define OLED_TAG            ("SSD1306")

// Control byte
#define OLED_CONTROL_BYTE_CMD_SINGLE            (0x80)
#define OLED_CONTROL_BYTE_CMD_STREAM            (0x00)
#define OLED_CONTROL_BYTE_DATA_SINGLE           (0xC0)
#define OLED_CONTROL_BYTE_DATA_STREAM           (0x40)

// Fundamental commands 
#define OLED_CMD_SET_CONTRAST                   (0x81)    // follow with 0x7F
#define OLED_CONTRAST                           (0xCF)
#define OLED_CMD_DISPLAY_ENTIRE_OFF             (0xA4)
#define OLED_CMD_DISPLAY_ALLON                  (0xA5)
#define OLED_CMD_DISPLAY_NOTINVERTED            (0xA6)
#define OLED_CMD_DISPLAY_INVERTED               (0xA7)
#define OLED_CMD_DISPLAY_OFF                    (0xAE)
#define OLED_CMD_DISPLAY_ON                     (0xAF)

// Addressing Command Table 
#define OLED_CMD_SET_MEMORY_ADDR_MODE           (0x20)    // follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
#define OLED_CMD_SET_COLUMN_RANGE               (0x21)    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define OLED_CMD_SET_PAGE_RANGE                 (0x22)    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7

//Line and Column start set
#define OLED_CMD_SET_DISPLAY_START_LINE(line)   (0x40 | (line & 0x01))
#define OLED_CMD_SET_COLUMN_START_LSN(column)   (0x00 | (column & 0x0F))
#define OLED_CMD_SET_COLUMN_START_MSN(column)   (0x10 | ((column >> 4) & 0x0F))
#define OLED_CMD_SET_PAGE_START(page)           (0xB0 | (page & 0x0F))

// Hardware Config 
#define OLED_CMD_SET_SEGMENT_REMAP              (0xA1)    
#define OLED_CMD_SET_MUX_RATIO                  (0xA8)    // follow with 0x3F = 64 MUX
#define OLED_MUX_RATIO_64                       (0x3F)
#define OLED_CMD_SET_COM_SCAN_DIRECTION         (0xC8)    
#define OLED_CMD_SET_DISPLAY_OFFSET             (0xD3)    // follow with 0x00
#define OLED_DISPLAY_OFFSET                     (0x00)
#define OLED_CMD_SET_COM_HW_CONFIG              (0xDA)    // follow with 0x12
#define OLED_COM_HW_CONFIG                      (0X12)
#define OLED_CMD_NOP                            (0xE3)    // NOP

// Timing and Driving Scheme
#define OLED_CMD_SET_DISPLAY_CLK_DIV            (0xD5)    // follow with 0x80
#define OLED_CLK_DIV_RATIO                      (0x80)
#define OLED_CMD_SET_PRECHARGE                  (0xD9)    // follow with 0xF1
#define OLED_PRECHARGE_PERIOD                   (0xF1)    // Internal Precharge period
#define OLED_CMD_SET_VCOMH_DESELCT              (0xDB)    // follow with 0x30
#define OLED_VCOHM_DESELCT_LVL                  (0x40)

// Charge Pump 
#define OLED_CMD_SET_CHARGE_PUMP                (0x8D)    // follow with 0x14
#define OLED_CHARGE_PUMP                        (0x14)    // Charge Pump (0x10 External, 0x14 Internal DC/DC)

/*-----------Variables and Types------------*/
typedef enum{
    DOT_SMALL = 0,
    DOT_MEDIUM,
    DOT_BIG
} dot_size_t;

/*------------------------Prototypes--------------------------*/
void hal_OLED_send_cmd(uint8_t data);
void hal_OLED_print(char *string, uint8_t page_start, uint8_t column_start);
void hal_OLED_draw_dot(dot_size_t size, uint8_t page_start, uint8_t column_start);
void hal_OLED_clear(void);
void hal_OLED_disp_image(uint8_t *image, uint8_t width, uint8_t height, uint8_t page_start, uint8_t column_start);
void hal_OLED_init(void);


#endif //HAL_DISPLAY_H