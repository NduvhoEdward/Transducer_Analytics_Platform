#pragma once
#include <Arduino.h>

extern volatile bool pumping;
extern float sampling_rate_hz;
extern uint32_t sampling_rate_ms; 

extern unsigned long lastDebounceTime;
extern unsigned long debounceDelay;

void IRAM_ATTR start_stop_pumping();
void pump();
