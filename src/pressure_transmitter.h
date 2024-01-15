#pragma once
#include <stdint.h> 
#include "driver/adc.h"

extern uint16_t p_adc_value; 
extern const float p_adc_vref;
extern float p_adc_voltage;
extern float adc_v_slope;
extern float adc_volts_yIntercept;
extern float zero_height; 
extern float current_height;
extern const float grav_acc;
extern const int bars_to_pa_multiplier;
extern const int m_to_mm_multiplier;
extern float pressure_mbars;

extern float p_shunt_resistor;
extern float pt_min_cur_in_mA; 
extern float pt_max_cur_in_mA;
extern float transmitter_min_cur;
extern float transmitter_max_cur;
extern float p_at_min_cur;
extern float p_at_max_cur;

extern float density;
extern float tank_max_height; 

extern float v_at_min_cur;
extern float v_at_max_cur;

extern float vp_slope;
extern float yIntercept;


void adcInit(); 
uint16_t get_p_adc_value();
void calib_adc_and_v();
float p_adc_to_volts();
float get_p_from_v();
