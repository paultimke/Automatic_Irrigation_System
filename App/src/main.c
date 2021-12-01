#include "main.h"

//Queues
xQueueHandle gpio_evt_queue = NULL;

//Tasks
TaskHandle_t flow_monitor_task_handle = NULL;
TaskHandle_t humidity_monitor_task_handle = NULL;
TaskHandle_t auto_valve_row1_task_handle = NULL;
TaskHandle_t auto_valve_row2_task_handle = NULL;
TaskHandle_t nodered_task_handle = NULL;
TaskHandle_t display_task_handle = NULL;
TaskHandle_t display_off_task_handle = NULL;
TaskHandle_t timed_water_task_handle = NULL;


void app_main(void)
{
    //Initialization
    hal_OLED_init();
    //hal_OLED_disp_image(yodita_glcd_bmp, YODITA_GLCD_WIDTH, YODITA_GLCD_HEIGHT, 2, 40);
    //vTaskDelay(10000/portTICK_PERIOD_MS);
    hal_OLED_clear();

    //Print image on display while waiting for initialization to be done
    hal_OLED_disp_image(granja_hogar_glcd_bmp, GRANJA_HOGAR_GLCD_WIDTH, GRANJA_HOGAR_GLCD_HEIGHT, 2, 40);

    usr_gpio_init();
    usr_timer_init();
    hal_flowsensor_init();
    hal_humidity_sensor_init();
    mqtt_init();
    mqtt_app_start();

    hal_OLED_clear();

    //Create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    //ISR initialization for timer and gpio
    timer_isr_register(TMR_GROUP_0, TMR_NUM_0, &timer0_isr_hanlder, NULL, ESP_INTR_FLAG_IRAM, NULL);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(BUTTON, gpio_isr_handler, (void*) BUTTON);

    //Create Display Task which updates data on screen, and then suspends immediately
    xTaskCreate(display_task, "display_task", 8142, NULL, 10, &display_task_handle);
    vTaskSuspend(display_task_handle);

    //Create Task to manually input irrigation time
    xTaskCreate(timed_water_task, "timed_water_task", 4096, NULL, 10, &timed_water_task_handle);

    //Create the rest of the tasks
    xTaskCreate(display_off_task, "display_off_task", 2048, NULL, 15, &display_off_task_handle);
    xTaskCreate(flow_monitor_task, "flow_monitor_task", 2048, NULL, 5, &flow_monitor_task_handle);
    xTaskCreate(humidity_monitor_task, "humidity_monitor_task", 2048, NULL, 5, &humidity_monitor_task_handle);
    xTaskCreate(auto_valve_row1_task, "auto_valve_row1_task", 2048, NULL, 5, &auto_valve_row1_task_handle);
    xTaskCreate(auto_valve_row2_task, "auto_valve_row2_task", 2048, NULL, 5, &auto_valve_row2_task_handle);
    xTaskCreatePinnedToCore(nodered_task, "nodered_task", 4096, NULL, 10, &nodered_task_handle, 1);

}

/*---------------------Interrupt Service Routines (ISR)-------------------*/

void IRAM_ATTR gpio_isr_handler(void* arg)
{
    bool isButtonPushed = true;
    xTaskResumeFromISR(display_task_handle);
    xQueueSendFromISR(gpio_evt_queue, &isButtonPushed, NULL);
   
}


void IRAM_ATTR timer0_isr_hanlder(void* arg)
{
    //Timer overflows each second
    //timer0_overflow++;
    TIMERG0.int_clr_timers.t0 = 1; // Clear interrupt bit
    //if(timer0_overflow > 25){

        /** Resume a task specifically to clear display before suspending the
         * display task, to ensure that when it is suspended, there are no pixels left on.
         */
        xTaskResumeFromISR(display_off_task_handle);
        vTaskSuspend(display_task_handle);
    //}
    TIMERG0.hw_timer[0].config.alarm_en = TIMER_ALARM_EN;   //Re-enable alarm for next interrupt
}
