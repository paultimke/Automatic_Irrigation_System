#include "main.h"

//Queues
xQueueHandle gpio_evt_queue = NULL;

//Task Handles
TaskHandle_t flow_monitor_task_handle = NULL;
TaskHandle_t humidity_monitor_task_handle = NULL;
TaskHandle_t auto_valve_row1_task_handle = NULL;
TaskHandle_t auto_valve_row2_task_handle = NULL;
TaskHandle_t nodered_task_handle = NULL;
TaskHandle_t display_task_handle = NULL;
TaskHandle_t display_off_task_handle = NULL;
TaskHandle_t timed_water_task_handle = NULL;
int is_time_task_active = 0;


void app_main(void)
{
    //Display Initialization
    hal_OLED_init();
    //Print image on display while waiting for initialization to be done
    hal_OLED_disp_image(granja_hogar_glcd_bmp, GRANJA_HOGAR_GLCD_WIDTH, GRANJA_HOGAR_GLCD_HEIGHT, 2, 40);

    // Hardware and Peripherals Initialization
    usr_timer_init();
    hal_evalve_init();
    hal_flowsensor_init();
    hal_humidity_sensor_init();
    app_nodered_init();

    hal_OLED_clear();

    //Create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    //ISR initialization for timer and gpio
    timer_isr_register(TMR_GROUP_0, TMR_NUM_0, &timer0_isr_hanlder, NULL, ESP_INTR_FLAG_IRAM, NULL);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(BUTTON, gpio_isr_handler, (void*) BUTTON);

    //Create Display Task which updates data on screen, and then suspends immediately
    xTaskCreatePinnedToCore(display_task, "display_task", 8142, NULL, 10, &display_task_handle, 1);
    vTaskSuspend(display_task_handle);

    //Create Task to manually input irrigation time
    xTaskCreate(timed_water_task, "timed_water_task", 4096, NULL, 10, &timed_water_task_handle);
    hal_OLED_clear();

    //Create the rest of the tasks
    xTaskCreatePinnedToCore(display_off_task, "display_off_task", 2048, NULL, 15, &display_off_task_handle, 1);
    xTaskCreate(flow_monitor_task, "flow_monitor_task", 2048, NULL, 5, &flow_monitor_task_handle);
    xTaskCreate(humidity_monitor_task, "humidity_monitor_task", 2048, NULL, 5, &humidity_monitor_task_handle);
    xTaskCreate(auto_valve_row1_task, "auto_valve_row1_task", 2048, NULL, 5, &auto_valve_row1_task_handle);
    xTaskCreate(auto_valve_row2_task, "auto_valve_row2_task", 2048, NULL, 5, &auto_valve_row2_task_handle);
    xTaskCreate(nodered_task, "nodered_task", 4096, NULL, 10, &nodered_task_handle);

}


/*--------------------------Interrupt Service Routines (ISR)---------------------------*/

/* GPIO interrupt serves to turn the display on after each button press */
void IRAM_ATTR gpio_isr_handler(void* arg)
{
    bool isButtonPushed = true;
    xTaskResumeFromISR(display_task_handle);
    xQueueSendFromISR(gpio_evt_queue, &isButtonPushed, NULL);
}


/* Timer 0 serves to set the amount of time the display will be on after each button press,
 * It is turned off when the alarm value is reached and the interrupt is triggered. */
void IRAM_ATTR timer0_isr_hanlder(void* arg)
{
    TIMERG0.int_clr_timers.t0 = 1; // Clear interrupt bit

        /** Resume a task specifically to clear display before suspending the
         * display task, to ensure that when it is suspended, there are no pixels left on. */
        xTaskResumeFromISR(display_off_task_handle);
        vTaskSuspend(display_task_handle);

    TIMERG0.hw_timer[0].config.alarm_en = TIMER_ALARM_EN;   //Re-enable alarm for next interrupt
}
