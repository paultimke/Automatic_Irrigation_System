#include "main.h"

//Task Handles
TaskHandle_t flow_handle = NULL;
TaskHandle_t humidity_handle = NULL;
TaskHandle_t valve_row1_handle = NULL;
TaskHandle_t valve_row2_handle = NULL;
TaskHandle_t nodered_handle = NULL;
TaskHandle_t display_task_handle = NULL;
TaskHandle_t display_off_task_handle = NULL;

//Queues
static xQueueHandle gpio_evt_queue = NULL;

//Global Variables
float flow_rate_s1, flow_rate_s2;
float row1_humidity, row2_humidity;
char str_is_valve1_on[10], str_is_valve2_on[10];
volatile uint8_t timer_overflow = 0;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    bool isButtonPushed = true;
    xTaskResumeFromISR(display_task_handle);
    xQueueSendFromISR(gpio_evt_queue, &isButtonPushed, NULL);
}

void IRAM_ATTR timer_isr_handler(void* arg)
{
    //Timer overflows each second
    timer_overflow++;
    TIMERG0.int_clr_timers.t0 = 1; //clear interrupt bit
    if(timer_overflow > 20){
        /** Enviar queue para apagar display;
         * Recibir queue que se mande desde task de display como acknowledge
         * de que ya se apago
        */
        xTaskResumeFromISR(display_off_task_handle);
        vTaskSuspend(display_task_handle);
    }
    TIMERG0.hw_timer[0].config.alarm_en = TIMER_ALARM_EN;   //re-enable alarm for next interrupt
}

void flow_task(void* arg)
{
    hal_flowsensor_init();

    while(1){
        //CODIGO DE SENSOR DE FLUJO
        flow_rate_s1 = hal_flowsensor_read(FLOW_SENSOR_1);
        flow_rate_s2 = hal_flowsensor_read(FLOW_SENSOR_2);
        printf("Flow rate 1: %5.2f L/min\n",flow_rate_s1);
        printf("Flow rate 2: %5.2f L/min\n",flow_rate_s2);

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void humidity_task(void* arg)
{
    hal_humidity_sensor_init();
    float water_percent_s1, water_percent_s2, water_percent_s3, water_percent_s4;

    while(1){

        //INICIA CODIGO DE SENSOR DE HUMEDAD
        vTaskDelay(500 / portTICK_PERIOD_MS);
        water_percent_s1 = hal_humidity_get_vwc(EC5_NUM_1);
        water_percent_s2 = hal_humidity_get_vwc(EC5_NUM_2);
        water_percent_s3 = hal_humidity_get_vwc(EC5_NUM_3);
        water_percent_s4 = hal_humidity_get_vwc(EC5_NUM_4);
        row1_humidity = (water_percent_s1 + water_percent_s2)/2;
        row2_humidity = (water_percent_s3 + water_percent_s4)/2;
        printf("Row 1 Humidity: %f\n", row1_humidity);
        printf("Row 2 Humidity: %f\n", row2_humidity);

        printf("timer overflow = %d\n", timer_overflow);

        vTaskDelay(2000/portTICK_PERIOD_MS);
        
    }
}

void valve_row1_task(void* arg)
{
    while(1){
        printf("********TASK ACTIVA VALVE 1********\n\n");

        if (row1_humidity < 40){
            hal_evalve_on(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 1.0);

        }
        else {
            hal_evalve_off(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 0.0);
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void valve_row2_task(void* arg)
{
    while(1){
        printf("********TASK ACTIVA VALVE 2********\n\n");
        if (row2_humidity < 40){
            hal_evalve_on(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 1.0);

        }
        else {
            hal_evalve_off(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 0.0);
        }

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void nodered_task(void* arg)
{

	char str_humidity_1[10], str_humidity_2[10];
    char str_flow_1[10], str_flow_2[10];
    bool isRow1_active = false, isRow2_active = false;

    while(1){
        // Prepare data to transmit
        sprintf(str_humidity_1,"%.1f", row1_humidity);
        sprintf(str_humidity_2,"%.1f", row2_humidity);
        sprintf(str_flow_1,"%.1f", flow_rate_s1);
        sprintf(str_flow_2,"%.1f", flow_rate_s2);

        // Publish message
        esp_mqtt_client_publish(client, "/riego2/sensorhumedad1", str_humidity_1, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/sensorhumedad2", str_humidity_2, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/sensorflujo1", str_flow_1, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/sensorflujo2", str_flow_2, 0, 1, 0);
        vTaskDelay(300/portTICK_PERIOD_MS);

        printf("Valve state: %d\n", valve_state);

        if((valve_state == ROW1_VALVE_OFF) && (isRow1_active == true)){
            vTaskResume(valve_row1_handle);
            hal_evalve_off(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 0.0);
            isRow1_active = false;
        }
        else if((valve_state == ROW1_VALVE_ON) && (isRow1_active == false)){
            vTaskSuspend(valve_row1_handle);
            hal_evalve_on(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 1.0);
            isRow1_active = true;
        }
        else if((valve_state == ROW2_VALVE_ON) && (isRow2_active == false)){
            vTaskSuspend(valve_row2_handle);
            hal_evalve_on(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 1.0);
            isRow2_active = true;
        }
        else if((valve_state == ROW2_VALVE_OFF) && (isRow2_active == true)){
            vTaskResume(valve_row2_handle);
            hal_evalve_off(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 0.0);
            isRow2_active = false;
        }
        else{
            ESP_LOGE(MQTT_TAG, "Mqtt error");
        }

        
        //Safety
        if((row1_humidity >70) || (row2_humidity >70)){
            vTaskResume(valve_row1_handle);
            vTaskResume(valve_row2_handle);
            isRow1_active = false;
            isRow2_active = false;
        }

        printf("Is ROW 1 ACTIVE: %d\n", (int)isRow1_active);
        printf("IS ROW 2 ACTIVE: %d\n", (int)isRow2_active);

        esp_mqtt_client_publish(client, "/riego2/estadovalvula1", str_is_valve1_on, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/estadovalvula2", str_is_valve2_on, 0, 1, 0);

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void display_off_task(void* arg)
{
    while(1){
        hal_OLED_clear();
        vTaskSuspend(NULL); //Suspend current task
    }
}

void display_task(void* arg)
{
    bool isButtonPushed;
    while(1){
        hal_OLED_print("Que rollillo", 2, 10);
        if(xQueueReceiveFromISR(gpio_evt_queue, &isButtonPushed, NULL) == pdTRUE)
        {
            //--Debouncing for button press--
            vTaskDelay(50/portTICK_PERIOD_MS);    //50ms of debounce time
            if((isButtonPushed == true) && (gpio_get_level(BTN_0)))
            {
                printf("***Entro, Var = 1***\n");
                timer_start(TMR_GROUP_0, TMR_NUM_0);
                timer_set_counter_value(TMR_GROUP_0, TMR_NUM_0, 0);
                timer_overflow = 0;
                isButtonPushed = false;
            }
            else {isButtonPushed = false;}
        }
    
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    //Initializing
    printf("Inicializando...\n");
    gpio_init();
    usr_timer_init();
    hal_OLED_init();
    mqtt_init();
    mqtt_app_start();

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    //Create Display Task which updates data on screen, and then suspends immediately
    xTaskCreate(display_task, "display_task", 8142, NULL, 10, &display_task_handle);
    vTaskSuspend(display_task_handle);

    //Create rest of tasks
    xTaskCreate(display_off_task, "display_off_task", 2048, NULL, 10, &display_off_task_handle);
    xTaskCreate(flow_task, "flow_task", 2048, NULL, 5, &flow_handle);
    xTaskCreate(humidity_task, "humidity_task", 2048, NULL, 5, &humidity_handle);
    xTaskCreate(valve_row1_task, "valve_row1_task", 2048, NULL, 5, &valve_row1_handle);
    xTaskCreate(valve_row2_task, "valve_row2_task", 2048, NULL, 5, &valve_row2_handle);
    xTaskCreatePinnedToCore(nodered_task, "nodered_task", 4096, NULL, 5, &nodered_handle, 1);

    timer_isr_register(0, 0, &timer_isr_handler, NULL, ESP_INTR_FLAG_IRAM, NULL);

    //ISR install for button
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(BTN_0, gpio_isr_handler, (void*) BTN_0);
                                                                                               
}

