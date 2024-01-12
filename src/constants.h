// Defines constant values used throughout the program, including pin assignments and configuration parameters.
#pragma once
#include <Arduino.h>

#define PRESSURE_TRANSMITTER_PIN GPIO_NUM_34
#define PULSE_COUNTER_PIN GPIO_NUM_36
#define VALVE_CONTROL_PIN GPIO_NUM_32
#define START_PUMP_BUTTON GPIO_NUM_35
#define ONBOARD_LED_PIN GPIO_NUM_2 
#define CIRCUIT_READINESS_LED GPIO_NUM_14 

#define PUMP_ON_LIGHT_PIN GPIO_NUM_12
#define PUMP_OFF_LIGHT_PIN GPIO_NUM_13

#define PCNT_UPPER_LIM INT16_MAX

