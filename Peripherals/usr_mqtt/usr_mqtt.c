#include "usr_mqtt.h"

static const char *TAG = "MQTT_CLIENT";
esp_mqtt_client_handle_t client;
uint8_t valve_state;
uint8_t global_hum_row1, global_hum_row2;

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void mqtt_init(void)
{
	ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());
}

esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    static char strTopic[20];
    static char strData[10];

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/riego2/valvula1", 0);
            ESP_LOGI(TAG, "sent subscribe successful to /riego2/valvula1, msg_id=%d", msg_id);

            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/riego2/valvula2", 0);
            ESP_LOGI(TAG, "sent subscribe successful to /riego2/valvula2, msg_id=%d", msg_id);

            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/riego2/desired_hum1", 0);
            ESP_LOGI(TAG, "sent subscribe successful to /riego2/desired_hum1, msg_id=%d", msg_id);

            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/riego2/desired_hum2", 0);
            ESP_LOGI(TAG, "sent subscribe successful to /riego2/desired_hum2, msg_id=%d", msg_id);

            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/riego2/tiempo_manual", 0);
            ESP_LOGI(TAG, "sent subscribe successful to /riego2/tiempo_manual, msg_id=%d", msg_id);
            
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            //Guardar evento en variables para hacer la comparación
            sprintf(strTopic,"%.*s", event->topic_len, event->topic);
            sprintf(strData,"%.*s", event->data_len, event->data);

            //Valve Activation 1
            if((strcmp(strTopic, "/riego2/valvula1") == 0) && (strcmp(strData, "ON") == 0)){
                valve_state = ROW1_VALVE_ON;     
                printf("ENTRO\n\n");
            }
            else if((strcmp(strTopic, "/riego2/valvula1") == 0) && (strcmp(strData, "OFF") == 0)){
                valve_state = ROW1_VALVE_OFF;
            }

            if((strcmp(strTopic, "/riego2/valvula2") == 0) && (strcmp(strData, "ON") == 0)){
                valve_state = ROW2_VALVE_ON;
            }
            else if((strcmp(strTopic, "/riego2/valvula2") == 0) && (strcmp(strData, "OFF") == 0)){
                valve_state = ROW2_VALVE_OFF;
            }

            //Humidity level set
            if(strcmp(strTopic, "/riego2/desired_hum1") == 0){
                global_hum_row1 = (uint8_t) atoi(strData);
                ESP_LOGI(TAG, "HUM 1 LIMIT UPDATE: %d", global_hum_row1);
            }
            if(strcmp(strTopic, "/riego2/desired_hum2") == 0){
                global_hum_row2 = (uint8_t) atoi(strData);
                ESP_LOGI(TAG, "HUM 1 LIMIT UPDATE: %d", global_hum_row2);
            }

            //Irrigation time set
            if(strcmp(strTopic, "/riego2/tiempo_manual") == 0){
                irrigation_minutes = (uint16_t) atoi(strData);
                printf("\n\nIRRIGATION MINUTES: %d\n\n", irrigation_minutes);
                is_time_task_active = 1;
                vTaskResume(timed_water_task_handle);
            }

            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}
