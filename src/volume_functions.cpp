#include "volume_functions.h"
#include "variables.h"

uint32_t getTotalPulses() {
    pcnt_get_counter_value(PCNT_UNIT_0, &pulseCount);
    uint32_t total_pls = maxCountsCycle * PCNT_UPPER_LIM + pulseCount;
    return total_pls;
}

float getCurrentVolume() {
    return totalPulseCounts / f_sensor_k_factor_ppg;
}
