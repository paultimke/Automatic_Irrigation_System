#ifndef USR_MQTT_H
#define USR_MQTT_H

/*------------------Includes-------------------*/
#include <string.h>
#include <stdlib.h>
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "protocol_examples_common.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

/*----------------Macros-----------------*/
#define ROW1_VALVE_OFF      (0)
#define ROW1_VALVE_ON       (1)
#define ROW2_VALVE_OFF      (2)
#define ROW2_VALVE_ON       (3)

/*------------------Function Prototypes-----------------*/
esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_app_start(void);
void mqtt_init(void);

/*-----------------Global variables-----------------*/
extern esp_mqtt_client_handle_t client;
extern uint8_t valve_state;

#endif //USR_MQTT.H