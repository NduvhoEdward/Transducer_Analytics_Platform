#include "adc_functions.h"
#include "variables.h"

uint16_t get_p_adc_value() {
    return adc1_get_raw(ADC1_CHANNEL_6);
}

void calib_adc_and_v() {
    int adc_value_1 = 337;
    int adc_value_2 = 2288;
    float measure_v_1 = 0.400;
    float measure_v_2 = 2.008;

    adc_v_slope = (measure_v_2 - measure_v_1) / (adc_value_2 - adc_value_1);
    adc_volts_yIntercept = measure_v_1 - adc_v_slope * adc_value_1;
}

float p_adc_to_volts() {
    return (adc_v_slope * p_adc_value + adc_volts_yIntercept);
}

float get_p_from_v() {
    return (vp_slope * p_adc_voltage + yIntercept);
}
