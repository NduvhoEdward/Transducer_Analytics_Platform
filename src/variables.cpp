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
float f_sensor_k_factor_ppg = 197.77; // -------------???????
float tank_max_height = 735; // --------------??????

float sampling_rate_hz = 2; 
uint32_t sampling_rate_ms = static_cast<uint32_t>((1 / sampling_rate_hz) * 1000);

float v_at_min_cur = transmitter_min_cur * p_shunt_resistor;
float v_at_max_cur = transmitter_max_cur * p_shunt_resistor;

float vp_slope = (p_at_max_cur - p_at_min_cur) / (v_at_max_cur - v_at_min_cur);
float yIntercept = p_at_min_cur - vp_slope * v_at_min_cur;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 1000;

// const char *ssid = "IMINING HO";
// const char *password = "1Min!Ng010";
const char *ssid = "OPPO_6B717F_2.4G";
const char *password = "Mbofho#23971*";
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

JSONVar readings;
unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

float total_volume_gal = 0.0;
