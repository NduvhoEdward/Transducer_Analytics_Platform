// Declares functions and configurations related to General Purpose Input/Output (GPIO).
#pragma once 
#include <Arduino.h> 

void configureGPIOs();
void pulseCounterInit();
void adcInit(); 
static void IRAM_ATTR pulseCounterISR(void *arg); 