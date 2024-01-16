#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <Arduino.h>

#include "driver/adc.h"

class Transmitter {
   protected:
    float volt_adc_slope;
    float volt_adc_yintercept;
    float quantity_volt_slope;
    float quantity_volt_yintercept;
    uint16_t read_adc_value();

    static bool dataInversion;
    static bool adcWidthConfigured;

    float p_adc_vref;

   public:
    adc1_channel_t adc1_channel_used;
    uint16_t adc_value;
    float adc_voltage;

    Transmitter(adc1_channel_t adc1_channel_used ,float shunt_resistor, float min_cur_in_mA, float max_cur_in_mA, float min_value, float max_value);
    void adc_config();
    void calib_adc_to_volt(int adc_value_1, float measured_v_1, int adc_value_2, float measured_v_2);
    float adc_to_volt();
    float get_quantity();
};

#endif
