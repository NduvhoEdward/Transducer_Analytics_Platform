#include "distance_transmitter.h"

const float d_adc_vref = 3.3;
float d_adc_voltage = 0;
uint16_t d_adc_value = 0;
float adc_d_slope = 0;
float adc_d_yIntercept = 0;
float zero_distance = 0;
float current_distance = 0;

float d_shunt_resistor = 100;
float dt_min_cur_in_mA = 4.0;
float dt_max_cur_in_mA = 20.0;
float dtransmitter_min_cur = dt_min_cur_in_mA / 1000;
float dtransmitter_max_cur = dt_max_cur_in_mA / 1000;
float d_at_min_cur = 40;
float d_at_max_cur = 300;

float dv_at_min_cur = dtransmitter_min_cur * d_shunt_resistor;

float dv_at_max_cur = dtransmitter_max_cur * d_shunt_resistor;

float dv_slope = (d_at_max_cur - d_at_min_cur) / (dv_at_max_cur - dv_at_min_cur);
float dv_yIntercept = d_at_min_cur - dv_slope * dv_at_min_cur;

Transmitter distance_transmitter(ADC1_CHANNEL_5,d_shunt_resistor, dt_min_cur_in_mA, dt_max_cur_in_mA, d_at_min_cur, d_at_max_cur);

void configure_dis_transmitter() {
    distance_transmitter.adc_config();
    // For adc pin 6
    int adc_value_1 = 337;
    int adc_value_2 = 2288;
    float measured_v_1 = 0.400;
    float measured_v_2 = 2.008;
    distance_transmitter.calib_adc_to_volt(adc_value_1, measured_v_1, adc_value_2, measured_v_2); 
}

