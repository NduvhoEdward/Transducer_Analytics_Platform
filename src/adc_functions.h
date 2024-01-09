// Declares functions for handling Analog-to-Digital 
// Converter (ADC) operations and calibration.
#pragma once
#include <stdint.h> 

uint16_t get_p_adc_value();
void calib_adc_and_v();
float p_adc_to_volts();
float get_p_from_v();
