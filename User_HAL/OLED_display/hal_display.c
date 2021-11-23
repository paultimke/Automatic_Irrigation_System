#include "hal_display.h"

uint8_t start_page_and_cols[3] = {
    OLED_CMD_SET_DISPLAY_START_LINE(0),
    OLED_CMD_SET_COLUMN_START_MSN(0),
    OLED_CMD_SET_COLUMN_START_LSN(0),
};

void hal_OLED_send_cmd(uint8_t data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN);  //Set i2c address for SSD1306 Oled display
    i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_SINGLE, ACK_CHECK_EN);         //Specify that data sent is a single-byte command
    i2c_master_write_byte(cmd, data, ACK_CHECK_EN);                                 //Send the actual command
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return;
}

static inline void hal_OLED_send_cmd_stream(uint8_t *data, uint8_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN);  //Set i2c address for SSD1306 Oled display
    i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, ACK_CHECK_EN);         //Specify that data sent is a single-byte command
    i2c_master_write(cmd, data, size, ACK_CHECK_EN);                                 //Send the actual command
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return;
}

/**
 * @brief Local function to display data in other functions of the file
 * @param  Byte of data to send
 */
static inline void hal_OLED_send_data_byte(uint8_t data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN);      //Set i2c address for SSD1306 Oled display
    i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_SINGLE, ACK_CHECK_EN);            //Specify that data sent is one byte of data
    i2c_master_write_byte(cmd, data, ACK_CHECK_EN);                                    //Send the actual data
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return;
}

/**
 * @brief Local function to display data in other functions of the file
 * @param Number of bytes of data to be sent
 */
static inline void hal_OLED_send_data_stream(uint8_t *data, uint8_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_STREAM, ACK_CHECK_EN);
    i2c_master_write(cmd, data, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return;
}

/**
 * @brief Local function to print letters or characters in hal_OLED_print
 * @param Ascii character to print
 * @param Page to start
 * @param Column to start
 */
static void hal_OLED_print_char(uint8_t character, uint8_t page_start, uint8_t column_start)
{
    hal_OLED_send_cmd(OLED_CMD_SET_DISPLAY_START_LINE(0));
    hal_OLED_send_cmd(OLED_CMD_SET_PAGE_START(page_start));

    hal_OLED_send_cmd(OLED_CMD_SET_COLUMN_START_LSN(column_start)); // Set column address lower nibble
    hal_OLED_send_cmd(OLED_CMD_SET_COLUMN_START_MSN(column_start)); // Set column address higher nibble

    for(uint8_t column = 0; column < FONT_SMALL_WIDTH; column++){
        //Subtract 32 to address the right row number in font matrix when Ascii data is inputed
        hal_OLED_send_data_byte(font8x8_basic_tr[character-32][column]);   // Print one single Ascii character
    }

    return;
}

void hal_OLED_print(char *string, uint8_t page_start, uint8_t column_start)
{
    uint8_t letter;
    uint8_t letter_space = column_start;    //The space that a character ocupies in width
    uint8_t* word = (uint8_t*) string;      //Cast to be able to treat data as string
    
    for(letter = 0; letter < strlen(string); letter++) {
        hal_OLED_print_char(word[letter], page_start, letter_space);
        letter_space = letter_space + FONT_SMALL_WIDTH; // Increment to print next letter without overlap
    }

    return;
}

void hal_OLED_draw_dot(dot_size_t size, uint8_t page_start, uint8_t column_start)
{
    hal_OLED_send_cmd(OLED_CMD_SET_DISPLAY_START_LINE(0));
    hal_OLED_send_cmd(OLED_CMD_SET_PAGE_START(page_start));

    hal_OLED_send_cmd(OLED_CMD_SET_COLUMN_START_LSN(column_start)); // Set column address lower nibble
    hal_OLED_send_cmd(OLED_CMD_SET_COLUMN_START_MSN(column_start)); // Set column address higher nibble

    uint8_t dot_height, dot_width;

    switch(size){
        case DOT_SMALL:
            dot_height = 0x07; 
            dot_width = 3;
            break;
        case DOT_MEDIUM:
            dot_height = 0x1F; 
            dot_width = 5;
            break;
        case DOT_BIG:
            dot_height = 0x7F; 
            dot_width = 7;
            break;
        default:
            dot_height = 0x03;
            dot_width = 2;
            break;
    }

    for(uint8_t column = 0; column < dot_width; column++){
        hal_OLED_send_data_byte(dot_height);   // Print one single Ascii character
    }

    return;
}

void hal_OLED_clear(void)
{
    uint8_t page;
    hal_OLED_send_cmd_stream(start_page_and_cols, 3);   //Send command to start pages and columns at 0

    for(page = 0; page < OLED_MAX_PAGE; page++) {
        hal_OLED_send_cmd(OLED_CMD_SET_PAGE_START(page));   //Increment pages
        hal_OLED_send_data_stream(clear_page, 128);     //Clear display page by page
    }

    return;
}

void hal_OLED_disp_image(uint8_t *image, uint8_t width, uint8_t height, uint8_t page_start, uint8_t column_start)
{
    uint8_t page, index, page_max, column_max;
    index = 0;

    /*Check for display boundaries before displaying*/
    page_max = page_start + (height-1) / OLED_DISPLAY_BITS;
	/*if(height % OLED_DISPLAY_BITS > 0)
	{
		page_max++;
	}
	if(page_max >(OLED_MAX_PAGE-1))
	{
		page_max = OLED_MAX_PAGE-1;
	}
    */
	column_max = column_start + width -1;
	/*if(column_max >= OLED_WIDTH)
	{
		column_max = OLED_WIDTH -1;
	}*/

    //hal_OLED_send_cmd(OLED_CMD_DISPLAY_OFF);
    hal_OLED_send_cmd(OLED_CMD_SET_DISPLAY_START_LINE(0));

    for(page = page_start; page < page_max; page++)
	{
		hal_OLED_send_cmd(OLED_CMD_SET_PAGE_START(page));
		for (uint8_t column = column_start; column <= column_max; column++)
		{
			hal_OLED_send_cmd(OLED_CMD_SET_COLUMN_START_LSN(column));
			hal_OLED_send_cmd(OLED_CMD_SET_COLUMN_START_MSN(column));

			hal_OLED_send_data_byte(image[index++]);
		}
	}
	hal_OLED_send_cmd(OLED_CMD_DISPLAY_ON);

    return;
}

void hal_OLED_init(void)
{
    /**
     * Initialization done as the datasheet suggests in Chapter 4.4
     * in http://www.adafruit.com/datasheets/UG-2864HSWEG01.pdf
     */

    esp_err_t init_return = usr_i2c_init();         // I2C Driver initialization

    hal_OLED_send_cmd(OLED_CMD_DISPLAY_OFF);

    hal_OLED_send_cmd(OLED_CMD_SET_DISPLAY_CLK_DIV);    // Set Display Clock Divide Ratio / OSC Frequency
    hal_OLED_send_cmd(OLED_CLK_DIV_RATIO);              // Display Clock Divide Ratio / OSC Frequency

    hal_OLED_send_cmd(OLED_CMD_SET_MUX_RATIO);  // Set Multiplex Ratio
    hal_OLED_send_cmd(OLED_MUX_RATIO_64);       // Multiplex Ratio for 128x64 (64-1)
    
    hal_OLED_send_cmd(OLED_CMD_SET_DISPLAY_OFFSET);
    hal_OLED_send_cmd(OLED_DISPLAY_OFFSET);

    hal_OLED_send_cmd(OLED_CMD_SET_DISPLAY_START_LINE(0));

    hal_OLED_send_cmd(OLED_CMD_SET_CHARGE_PUMP);
    hal_OLED_send_cmd(OLED_CHARGE_PUMP);

    hal_OLED_send_cmd(OLED_CMD_SET_SEGMENT_REMAP);
    hal_OLED_send_cmd(OLED_CMD_SET_COM_SCAN_DIRECTION);

    hal_OLED_send_cmd(OLED_CMD_SET_COM_HW_CONFIG);
    hal_OLED_send_cmd(OLED_COM_HW_CONFIG);

    hal_OLED_send_cmd(OLED_CMD_SET_CONTRAST);
    hal_OLED_send_cmd(OLED_CONTRAST);

    hal_OLED_send_cmd(OLED_CMD_SET_PRECHARGE);
    hal_OLED_send_cmd(OLED_PRECHARGE_PERIOD);

    hal_OLED_send_cmd(OLED_CMD_SET_VCOMH_DESELCT);
    hal_OLED_send_cmd(OLED_VCOHM_DESELCT_LVL);

    hal_OLED_send_cmd(OLED_CMD_DISPLAY_ENTIRE_OFF);         //Set entire display on/off
    hal_OLED_send_cmd(OLED_CMD_DISPLAY_NOTINVERTED);        //Display normal

    hal_OLED_clear();

    hal_OLED_send_cmd(OLED_CMD_DISPLAY_ON);

    /*Error handling*/
    if(init_return != ESP_OK){
        ESP_LOGE(OLED_TAG, "Display initialization failed. code: 0x%.2X", init_return);
    }
    else{
        ESP_LOGI(OLED_TAG, "Display initialized succesfully");
    }

    return;
}