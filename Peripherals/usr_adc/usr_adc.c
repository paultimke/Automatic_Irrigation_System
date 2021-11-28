#include "usr_adc.h"

/*Local helper function to find mode of multiple adc samples*/
static uint32_t mode(uint32_t arr[]);     

/*Declaration of adc_characteristics*/
esp_adc_cal_characteristics_t *adc_characteristics;

void usr_adc_init(void)
{
    //Initial setup
    adc1_config_width(ADC_WIDTH_BIT_12);
    
    //Calibration
    adc_characteristics = (esp_adc_cal_characteristics_t*) calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, 
                              ADC_VREF, adc_characteristics);  

    //Configure attenuation levels for all channels used
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_0);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_0);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_0);

    return;
}


uint32_t usr_adc_getResult(adc_channel_t channel)
{
   //Declare array to store the samples of the readings of adc
    uint32_t adc_samples[ADC_SAMPLE_COUNT], adc_reading;

    //Collect samples from ADC
    adc_power_acquire();
    for(uint8_t i = 0; i<ADC_SAMPLE_COUNT; i++){
        adc_samples[i] = adc1_get_raw(channel);
        vTaskDelay(1/ portTICK_PERIOD_MS);
    }
    adc_power_release();

    //Calculate mode to describe result more accurately
    adc_reading = mode(adc_samples);

    //Convert raw result to voltage in mV
    #ifdef ADC_CALIBRATION_ON
      uint32_t result = esp_adc_cal_raw_to_voltage(adc_reading, adc_characteristics);
    #else
      uint32_t result = adc_reading * ADC_VREF / ADC_MAX_VALUE;
    #endif
    printf("\nVoltage: %d\n", result);

    return adc_reading;
}


static uint32_t mode(uint32_t arr[])
{
   uint32_t maxValue = 0, maxCount = 0, i, j, n = ADC_SAMPLE_COUNT;

   for (i = 0; i < n; ++i) {
      int count = 0;
      
      for (j = 0; j < n; ++j) {
         if (arr[j] == arr[i])
         ++count;
      }
      if (count > maxCount) {
         maxCount = count;
         maxValue = arr[i];
      }
   }
   return maxValue;
}



