#include "pressure_transmitter.h"

uint16_t p_adc_value = 0;
const float p_adc_vref = 3.3;
float p_adc_voltage = 0;
float adc_v_slope = 0;
float adc_volts_yIntercept = 0;
float zero_height = 0; 
float current_height = 0.0;
const float grav_acc = 9.80665;
const int bars_to_pa_multiplier = 100;
const int m_to_mm_multiplier = 1000;

float pressure_mbars = 0.0;

float p_shunt_resistor = 100; 
float pt_min_cur_in_mA = 4.0; 
float pt_max_cur_in_mA = 20.0;
float transmitter_min_cur = pt_min_cur_in_mA / 1000;
float transmitter_max_cur = pt_max_cur_in_mA / 1000;
float p_at_min_cur = 0; // ----------------------------------???????
float p_at_max_cur = 250; // ------------------------------???????

float density = 999; // -----------????????????????????
float tank_max_height = 735; // --------------??????

float v_at_min_cur = transmitter_min_cur * p_shunt_resistor;
float v_at_max_cur = transmitter_max_cur * p_shunt_resistor;

float vp_slope = (p_at_max_cur - p_at_min_cur) / (v_at_max_cur - v_at_min_cur);
float yIntercept = p_at_min_cur - vp_slope * v_at_min_cur;


void adcInit() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    adc_set_data_inv(ADC_UNIT_1, true | false);
} 

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
