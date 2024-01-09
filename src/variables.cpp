// variables.cpp
#include "variables.h"

uint16_t p_adc_value = 0;
uint32_t totalPulseCounts = 0;
int16_t pulseCount = 0;
volatile int maxCountsCycle = 0;
volatile bool pumping = false;

const float p_adc_vref = 3.3;
float p_adc_voltage = 0;
float adc_v_slope = 0;
float adc_volts_yIntercept = 0;
float current_height = 0;
const float grav_acc = 9.80665;
const int bars_to_pa_multiplier = 100000;

const int p_adc_max = 4095;
float pressure_bars = 0.0;

const float p_shunt_resistor = P_SHUNT_RESISTOR_OHMS;
const float transmitter_min_cur = PT_MIN_CUR_IN_mA / 1000;
const float transmitter_max_cur = PT_MAX_CUR_IN_mA / 1000;
const float p_at_min_cur = P_MIN;
const float p_at_max_cur = P_MAX;

float density = DENSITY;
float f_sensor_k_factor_ppg = F_SENSOR_K_FACTOR_PPG;
float tank_max_height = TANK_MAX_HEIGHT;
float sampling_rate_f = 1 / (static_cast<float>(SAMPLING_RATE_MS) / 1000);
uint32_t sampling_rate_ms = (1 / (sampling_rate_f)) * 1000;

const float v_at_min_cur = transmitter_min_cur * p_shunt_resistor;
const float v_at_max_cur = transmitter_max_cur * p_shunt_resistor;

const float vp_slope = (p_at_max_cur - p_at_min_cur) / (v_at_max_cur - v_at_min_cur);
const float yIntercept = p_at_min_cur - vp_slope * v_at_min_cur;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 1000;

const char *ssid = "IMINING HO";
const char *password = "1Min!Ng010";
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

JSONVar readings;
unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

float total_volume_gal = 0.0;
