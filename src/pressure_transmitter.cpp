#include "pressure_transmitter.h"

uint16_t p_adc_value = 0;
const float p_adc_vref = 3.3;
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
float ptransmitter_min_cur = pt_min_cur_in_mA / 1000;
float ptransmitter_max_cur = pt_max_cur_in_mA / 1000;
float p_at_min_cur = 0; // ----------------------------------???????
float p_at_max_cur = 250; // ------------------------------???????

float density = 999; // -----------????????????????????
float tank_max_height = 735; // --------------??????

float pv_at_min_cur = ptransmitter_min_cur * p_shunt_resistor;
float pv_at_max_cur = ptransmitter_max_cur * p_shunt_resistor;

float pv_slope = (p_at_max_cur - p_at_min_cur) / (pv_at_max_cur - pv_at_min_cur);
float pv_yintercept = p_at_min_cur - pv_slope * pv_at_min_cur; 


Transmitter pressure_transmitter(ADC1_CHANNEL_6, p_shunt_resistor, pt_min_cur_in_mA, pt_max_cur_in_mA, p_at_min_cur, p_at_max_cur);

void configure_p_transmitter() {
    // Transmitter pressure_transmitter(p_shunt_resistor, pt_min_cur_in_mA, pt_max_cur_in_mA, p_at_min_cur, p_at_max_cur);
    pressure_transmitter.adc_config();
    // For adc pin 6
    int adc_value_1 = 337;
    int adc_value_2 = 2288;
    float measured_v_1 = 0.400;
    float measured_v_2 = 2.008;
    pressure_transmitter.calib_adc_to_volt(adc_value_1, measured_v_1, adc_value_2, measured_v_2);
}

