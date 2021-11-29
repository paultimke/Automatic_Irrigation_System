#include "APP_display.h"

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
                timer0_overflow = 0;
                isButtonPushed = false;
            }
            else {isButtonPushed = false;}
        }

        sprintf(flow1_string, "F1: %.2f L/min", flow_rate_s1);
        sprintf(flow2_string, "F2: %.2f L/min", flow_rate_s2);
        sprintf(hum1_string, "F1: %.2f %%", row1_humidity);
        sprintf(hum2_string, "F2: %.2f %%", row2_humidity);

        if(timer0_overflow < 10){
            hal_OLED_print("Humedad", 1, OLED_TEXT_CENTER(strlen("Humedad")));
            hal_OLED_print(hum1_string, 3, 1);
            hal_OLED_print(hum2_string, 5, 1);
        }
        else if((timer0_overflow >= 10) && (timer0_overflow <20)){
            if(disp_cleared == false){
                hal_OLED_clear();
                vTaskDelay(25/portTICK_PERIOD_MS);
            }
            disp_cleared = true;
            hal_OLED_print("Flujo", 1, OLED_TEXT_CENTER(strlen("Flujo")));
            hal_OLED_print(flow1_string, 3, 1);
            hal_OLED_print(flow2_string, 5, 1);
        }
        else if(timer0_overflow == 20){
            hal_OLED_clear();
        }
        else{
            hal_OLED_disp_image(granja_hogar_glcd_bmp, GRANJA_HOGAR_GLCD_WIDTH, GRANJA_HOGAR_GLCD_HEIGHT, 2, 40);
            disp_cleared = false;
        }
        

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void display_off_task(void* arg)
{
    while(1){
        timer_pause(TMR_GROUP_0, TMR_NUM_0);
        timer0_overflow = 0;
        hal_OLED_clear();   //Clear display so that when display_task is suspended, no pixels are left on
        vTaskSuspend(NULL); //Suspend current task
    }
}