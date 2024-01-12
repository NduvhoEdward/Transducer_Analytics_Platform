// Declares global variables used across multiple source files 
#pragma once

#include <Arduino.h> 
#include <Arduino_JSON.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <driver/pcnt.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include "driver/adc.h"
#include "constants.h"

extern uint16_t p_adc_value;
extern uint32_t totalPulseCounts;
extern int16_t pulseCount;
extern volatile int maxCountsCycle;
extern volatile bool pumping;

extern const float p_adc_vref;
extern float p_adc_voltage;
extern float adc_v_slope;
extern float adc_volts_yIntercept;
extern float current_height;
extern const float grav_acc;
extern const int bars_to_pa_multiplier;

extern const int p_adc_max;
extern float pressure_bars;

extern float p_shunt_resistor;
extern float pt_min_cur_in_mA; 
extern float pt_max_cur_in_mA;
extern float transmitter_min_cur;
extern float transmitter_max_cur;
extern float p_at_min_cur;
extern float p_at_max_cur;

extern float density;
extern float f_sensor_k_factor_ppg;
extern float tank_max_height;
extern float sampling_rate_hz;
extern uint32_t sampling_rate_ms;

extern const float v_at_min_cur;
extern const float v_at_max_cur;

extern const float vp_slope;
extern const float yIntercept;

extern unsigned long lastDebounceTime;
extern unsigned long debounceDelay;

extern const char *ssid;
extern const char *password;
extern AsyncWebServer server;
extern AsyncWebSocket ws;

extern JSONVar readings;

extern unsigned long lastTime;
extern unsigned long timerDelay;

extern float total_volume_gal;
