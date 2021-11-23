#include "usr_i2c.h"


esp_err_t usr_i2c_init(void){
    int i2c_master_port = 0;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_PIN,         // select GPIO specific to your project
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_PIN,         // select GPIO specific to your project
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,  // select frequency specific to your project
        // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };

    esp_err_t paramret = i2c_param_config(i2c_master_port, &conf);
    if(paramret != ESP_OK){
        ESP_LOGE(USR_I2C_TAG, "Parameter config failed. code: 0x%.2X", paramret);
    }

    esp_err_t install_ret = i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if(install_ret != ESP_OK){
        ESP_LOGE(USR_I2C_TAG, "Driver install failed. code: 0x%.2X", install_ret);
    }

    return ESP_OK;
}
