#pragma once 
#include <Arduino.h>
#include <driver/pcnt.h>

#define PCNT_UPPER_LIM INT16_MAX

extern uint32_t totalPulseCounts;
extern int16_t pulseCount;
extern volatile int maxCountsCycle;
extern float f_sensor_k_factor_ppg;
extern float total_volume_gal;

void pulseCounterInit();
static void IRAM_ATTR pulseCounterISR(void *arg); 

uint32_t getTotalPulses();
float getCurrentVolume();
