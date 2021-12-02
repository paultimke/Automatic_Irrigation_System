#include "APP_display.h"

/*void display_task(void* arg)
{
    char flow1_string[15], flow2_string[15], hum1_string[15], hum2_string[15];
    double timer0_seconds;
    bool isButtonPushed;
    bool humidity_cleared = false;
    bool flow_cleared = false;

    while(1){
        if(xQueueReceiveFromISR(gpio_evt_queue, &isButtonPushed, NULL) == pdTRUE)
        {
            //--Debouncing for button press--
            vTaskDelay(10/portTICK_PERIOD_MS);    //50ms of debounce time
            if((isButtonPushed == true) && (usr_gpio_read(BUTTON)))
            {
                ESP_LOGI("BUTTON", "Button interrupt generated");
                timer_start(TMR_GROUP_0, TMR_NUM_0);
                timer_set_counter_value(TMR_GROUP_0, TMR_NUM_0, 0);
                isButtonPushed = false;
            }
            else {isButtonPushed = false;}
        }

        sprintf(flow1_string, "F1: %.2f L/min", flow_rate_s1);
        sprintf(flow2_string, "F2: %.2f L/min", flow_rate_s2);
        sprintf(hum1_string, "F1: %.2f %%", row1_humidity);
        sprintf(hum2_string, "F2: %.2f %%", row2_humidity);

        timer_get_counter_time_sec(TMR_GROUP_0, TMR_NUM_0, &timer0_seconds);

        if(timer0_seconds < 10){
            flow_cleared = false;
            hal_OLED_print("Humedad", 1, OLED_TEXT_CENTER(strlen("Humedad")));
            hal_OLED_print(hum1_string, 3, 1);
            hal_OLED_print(hum2_string, 5, 1);
        }
        else if((timer0_seconds >= 10) && (timer0_seconds <20)){
            if(humidity_cleared == false){
                hal_OLED_clear();
                vTaskDelay(25/portTICK_PERIOD_MS);
            }
            humidity_cleared = true;
            hal_OLED_print("Flujo", 1, OLED_TEXT_CENTER(strlen("Flujo")));
            hal_OLED_print(flow1_string, 3, 1);
            hal_OLED_print(flow2_string, 5, 1);
        }
        else if(timer0_seconds >= 20){
            if(flow_cleared == false){
                hal_OLED_clear();
                vTaskDelay(25/portTICK_PERIOD_MS);
            }
            flow_cleared = true;
            hal_OLED_disp_image(granja_hogar_glcd_bmp, GRANJA_HOGAR_GLCD_WIDTH, GRANJA_HOGAR_GLCD_HEIGHT, 2, 40);
            humidity_cleared = false;
        }
        
        printf("Timer 0 Seconds : %.2f\n", timer0_seconds);
        vTaskDelay(600/portTICK_PERIOD_MS);
    }
}*/

/*void display_off_task(void* arg)
{
    while(1){
        printf("ggggg\n");
        timer_pause(TMR_GROUP_0, TMR_NUM_0);
        hal_OLED_clear();   //Clear display so that when display_task is suspended, no pixels are left on
        vTaskDelay(50/portTICK_PERIOD_MS);
        vTaskSuspend(NULL); //Suspend current task
    }
}*/