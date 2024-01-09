// Defines constant values used throughout the program, including pin assignments and configuration parameters.
#pragma once
#include <Arduino.h>

#define PRESSURE_TRANSMITTER_PIN GPIO_NUM_34
#define PULSE_COUNTER_PIN GPIO_NUM_36
#define VALVE_CONTROL_PIN GPIO_NUM_32
#define START_PUMP_BUTTON GPIO_NUM_35
#define ONBOARD_LED_PIN GPIO_NUM_2

#define PUMP_ON_LIGHT_PIN GPIO_NUM_12
#define PUMP_OFF_LIGHT_PIN GPIO_NUM_13

#define PCNT_UPPER_LIM INT16_MAX

#define P_SHUNT_RESISTOR_OHMS 100.0
#define PT_MIN_CUR_IN_mA 4.0
#define PT_MAX_CUR_IN_mA 20.0
#define P_MIN 0.0
#define P_MAX 0.250  // -------------------------------------------------------------??????????????????

#define F_SENSOR_K_FACTOR_PPG 197.77    // --------------------------??????????????????
#define TANK_MAX_HEIGHT 0.64            // ------------------------------------??????????????????
#define DENSITY 999                     // ------------------------------------------??????????????????
#define SAMPLING_RATE_MS 750            // ----------------------------------??????????????????
