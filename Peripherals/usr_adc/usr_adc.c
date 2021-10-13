#include "usr_adc.h"

uint32_t mode(uint32_t a[]);     //Local function to find mode of multiple adc samples

esp_adc_cal_characteristics_t *usr_adc_init(void)
{
    //Initial setup
    adc1_config_width(ADC_WIDTH_BIT_12);
    
    //Calibration
    esp_adc_cal_characteristics_t *characteristics = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, 
                              ADC_VREF, characteristics);

    //Configure attenuation levels for all channels used
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0);

    return characteristics;
}


uint32_t usr_adc_getResult(adc_channel_t channel, esp_adc_cal_characteristics_t *adc_calib_chars)
{
    uint32_t adc_samples[ADC_SAMPLE_COUNT], adc_reading;

    //Collect samples from ADC
    adc_power_acquire();
    for(int i = 0; i<ADC_SAMPLE_COUNT; i++){
        adc_samples[i] = adc1_get_raw(channel);
        vTaskDelay(1/ portTICK_PERIOD_MS);
    }
    adc_power_release();

    //Calculate mode to describe result more accurately
    adc_reading = mode(adc_samples);

    //Convert raw result to voltage in mV
    #ifdef ADC_CALIBRATION_ON
      uint32_t result = esp_adc_cal_raw_to_voltage(adc_reading, adc_calib_chars);
    #else
      uint32_t result = adc_reading * ADC_VREF / ADC_MAX_VALUE;
    #endif

    return result;
}


uint32_t mode(uint32_t a[])
{
   uint32_t maxValue = 0, maxCount = 0, i, j, n = ADC_SAMPLE_COUNT;

   for (i = 0; i < n; ++i) {
      int count = 0;
      
      for (j = 0; j < n; ++j) {
         if (a[j] == a[i])
         ++count;
      }
      if (count > maxCount) {
         maxCount = count;
         maxValue = a[i];
      }
   }

   return maxValue;
}



