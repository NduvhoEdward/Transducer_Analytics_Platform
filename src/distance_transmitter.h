#pragma once
#include <stdint.h> 
#include "driver/adc.h"
#include "Transmitter.h"

extern uint16_t d_adc_value; 
extern const float d_adc_vref;
extern float d_adc_voltage;
extern float adc_d_slope;
extern float adc_d_yIntercept;
extern float zero_distance; 
extern float current_distance;
extern const int m_to_mm_multiplier;

extern float p_shunt_resistor;
extern float dt_min_cur_in_mA; 
extern float dt_max_cur_in_mA;
extern float transmitter_min_cur;
extern float transmitter_max_cur;
extern float d_at_min_cur;
extern float d_at_max_cur;

extern float v_at_min_cur;
extern float v_at_max_cur;

extern float dv_slope;
extern float dv_yintercept;

extern Transmitter distance_transmitter;
void configure_dis_transmitter(); 
