#include "main.h"

//Queues
static xQueueHandle gpio_evt_queue = NULL;

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
    TIMERG0.int_clr_timers.t0 = 1; // Clear interrupt bit
    if(timer_overflow > 25){

        /** Resume a task specifically to clear display before suspending the
         * display task, to ensure that when it is suspended, there are no pixels left on.
         */
        xTaskResumeFromISR(display_off_task_handle);
        vTaskSuspend(display_task_handle);
    }
    TIMERG0.hw_timer[0].config.alarm_en = TIMER_ALARM_EN;   //Re-enable alarm for next interrupt
}

void flow_task(void* arg)
{
    while(1){
        flow_rate_s1 = hal_flowsensor_read(FLOW_SENSOR_1);
        flow_rate_s2 = hal_flowsensor_read(FLOW_SENSOR_2);
        printf("Flow rate 1: %5.2f L/min\n",flow_rate_s1);
        printf("Flow rate 2: %5.2f L/min\n",flow_rate_s2);

        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}

void humidity_task(void* arg)
{
    float water_percent_s1, water_percent_s2, water_percent_s3, water_percent_s4;

    while(1){

        vTaskDelay(500 / portTICK_PERIOD_MS);
        water_percent_s1 = hal_humidity_get_vwc(EC5_NUM_1);
        water_percent_s2 = hal_humidity_get_vwc(EC5_NUM_2);
        water_percent_s3 = hal_humidity_get_vwc(EC5_NUM_3);
        water_percent_s4 = hal_humidity_get_vwc(EC5_NUM_4);
        row1_humidity = (water_percent_s1 + water_percent_s3)/2;
        row2_humidity = (water_percent_s2 + water_percent_s4)/2;
        printf("Row 1 Humidity: %f\n", row1_humidity);
        printf("Row 2 Humidity: %f\n", row2_humidity);

        printf("Number of timer overflows = %d\n", timer_overflow);

        vTaskDelay(20000/portTICK_PERIOD_MS);
        
    }
}

void auto_valve_row1_task(void* arg)
{
    uint8_t desired_hum_row1;

    while(1){
        //printf("********TASK ACTIVA VALVE 1********\n\n");

        desired_hum_row1 = global_hum_row1;
        if(global_hum_row1 == 0){
            desired_hum_row1 = DEFAULT_HUM_LIMIT;
        }

        if (row1_humidity < desired_hum_row1){
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

void auto_valve_row2_task(void* arg)
{
    uint8_t desired_hum_row2;

    while(1){
        //printf("********TASK ACTIVA VALVE 2********\n\n");

        desired_hum_row2 = global_hum_row2;
        if(global_hum_row2 == 0){
            desired_hum_row2 = DEFAULT_HUM_LIMIT;
        }

        if (row2_humidity < desired_hum_row2){
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
            vTaskResume(auto_valve_row1_task_handle);
            hal_evalve_off(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 0.0);
            isRow1_active = false;
        }
        else if((valve_state == ROW1_VALVE_ON) && (isRow1_active == false)){
            vTaskSuspend(auto_valve_row1_task_handle);
            hal_evalve_on(EVALVE_UNIT_0);
            sprintf(str_is_valve1_on, "%f", 1.0);
            isRow1_active = true;
        }
        else if((valve_state == ROW2_VALVE_ON) && (isRow2_active == false)){
            vTaskSuspend(auto_valve_row2_task_handle);
            hal_evalve_on(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 1.0);
            isRow2_active = true;
        }
        else if((valve_state == ROW2_VALVE_OFF) && (isRow2_active == true)){
            vTaskResume(auto_valve_row2_task_handle);
            hal_evalve_off(EVALVE_UNIT_1);
            sprintf(str_is_valve2_on, "%f", 0.0);
            isRow2_active = false;
        }
        
        /** Safety measure to prevent valves being left on indefinetely, in case it
         * is forgotten to turn them off, to avoid flooding.
         */
        if((row1_humidity > MAX_HUM_SAFETY) || (row2_humidity > MAX_HUM_SAFETY)){
            vTaskResume(auto_valve_row1_task_handle);
            vTaskResume(auto_valve_row2_task_handle);
            isRow1_active = false;
            isRow2_active = false;
        }

        esp_mqtt_client_publish(client, "/riego2/estadovalvula1", str_is_valve1_on, 0, 1, 0);
        esp_mqtt_client_publish(client, "/riego2/estadovalvula2", str_is_valve2_on, 0, 1, 0);

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void display_off_task(void* arg)
{
    while(1){
        timer_pause(TMR_GROUP_0, TMR_NUM_0);
        hal_OLED_clear();   //Clear display so that when display_task is suspended, no pixels are left on
        timer_overflow = 0;
        vTaskSuspend(NULL); //Suspend current task
    }
}

void display_task(void* arg)
{
    char flow1_string[15], flow2_string[15], hum1_string[15], hum2_string[15];
    bool isButtonPushed;
    bool disp_cleared = false;

    while(1){
        if(xQueueReceiveFromISR(gpio_evt_queue, &isButtonPushed, NULL) == pdTRUE)
        {
            //--Debouncing for button press--
            vTaskDelay(50/portTICK_PERIOD_MS);    //50ms of debounce time
            if((isButtonPushed == true) && (usr_gpio_read(BUTTON)))
            {
                ESP_LOGI("BUTTON", "Button interrupt generated");
                timer_start(TMR_GROUP_0, TMR_NUM_0);
                timer_set_counter_value(TMR_GROUP_0, TMR_NUM_0, 0);
                timer_overflow = 0;
                isButtonPushed = false;
            }
            else {isButtonPushed = false;}
        }

        sprintf(flow1_string, "F1: %.2f L/min", flow_rate_s1);
        sprintf(flow2_string, "F2: %.2f L/min", flow_rate_s2);
        sprintf(hum1_string, "F1: %.2f %%", row1_humidity);
        sprintf(hum2_string, "F2: %.2f %%", row2_humidity);

        if(timer_overflow < 10){
            hal_OLED_print("Humedad", 1, OLED_TEXT_CENTER(strlen("Humedad")));
            hal_OLED_print(hum1_string, 3, 1);
            hal_OLED_print(hum2_string, 5, 1);
        }
        else if((timer_overflow >= 10) && (timer_overflow <20)){
            if(disp_cleared == false){
                hal_OLED_clear();
                vTaskDelay(25/portTICK_PERIOD_MS);
            }
            disp_cleared = true;
            hal_OLED_print("Flujo", 1, OLED_TEXT_CENTER(strlen("Flujo")));
            hal_OLED_print(flow1_string, 3, 1);
            hal_OLED_print(flow2_string, 5, 1);
        }
        else if(timer_overflow == 20){
            hal_OLED_clear();
        }
        else{
            hal_OLED_disp_image(granja_hogar_glcd_bmp, GRANJA_HOGAR_GLCD_WIDTH, GRANJA_HOGAR_GLCD_HEIGHT, 2, 40);
            disp_cleared = false;
        }
        

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

/*oid timed_water_task(void* arg)
{
    uint8_t row1_water_seconds;
    row1_water_seconds = row1_water_minutes * 60;

    vTaskSuspend

    timer_set_alarm_value(TMR_GROUP_0, TMR_NUM_1, (uint64_t) row1_water_seconds);


}*/


void app_main(void)
{
    //Initialization
    hal_OLED_init();
    //Print image on display while waiting for initialization to be done
    hal_OLED_disp_image(granja_hogar_glcd_bmp, GRANJA_HOGAR_GLCD_WIDTH, GRANJA_HOGAR_GLCD_HEIGHT, 2, 40);

    usr_gpio_init();
    usr_timer_init();
    hal_flowsensor_init();
    hal_humidity_sensor_init();
    //mqtt_init();
    //mqtt_app_start();

    hal_OLED_clear();

    //Create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    //ISR initialization for timer and gpio
    timer_isr_register(TMR_GROUP_0, TMR_NUM_0, &timer_isr_handler, NULL, ESP_INTR_FLAG_IRAM, NULL);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(BUTTON, gpio_isr_handler, (void*) BUTTON);

    //Create Display Task which updates data on screen, and then suspends immediately
    xTaskCreate(display_task, "display_task", 8142, NULL, 10, &display_task_handle);
    vTaskSuspend(display_task_handle);

    //Create rest of tasks
    xTaskCreate(display_off_task, "display_off_task", 2048, NULL, 10, &display_off_task_handle);
    xTaskCreate(flow_task, "flow_task", 2048, NULL, 5, &flow_task_handle);
    xTaskCreate(humidity_task, "humidity_task", 2048, NULL, 5, &humidity_task_handle);
    xTaskCreate(auto_valve_row1_task, "auto_valve_row1_task", 2048, NULL, 5, &auto_valve_row1_task_handle);
    xTaskCreate(auto_valve_row2_task, "auto_valve_row2_task", 2048, NULL, 5, &auto_valve_row2_task_handle);
    //xTaskCreatePinnedToCore(nodered_task, "nodered_task", 4096, NULL, 5, &nodered_task_handle, 1);
    //xTaskCreate(timed_water_task, "timed_water_task", 2048, NULL, 5, &timed_water_task_handle);
                                                                              
}

