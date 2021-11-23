#ifndef USR_I2C_H
#define USR_I2C_H

/*---------------Includes----------------*/
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"

/*----------------Macros------------------*/
#define I2C_MASTER_SDA_PIN          32
#define I2C_MASTER_SCL_PIN          33

#define I2C_MASTER_FREQ_HZ          800000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000
#define WRITE_BIT                   I2C_MASTER_WRITE
#define ACK_CHECK_EN                0x1                 /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS               0x0                 /*!< I2C master will not check ack from slave */
#define ACK_VAL                     0x0                 /*!< I2C ack value */
#define NACK_VAL                    0x1   
#define I2C_PORT_NUM                0

#define USR_I2C_TAG                 "USR_I2C"

/*-----------Prototypes--------------*/
esp_err_t usr_i2c_init(void);


#endif //USR_I2C_H