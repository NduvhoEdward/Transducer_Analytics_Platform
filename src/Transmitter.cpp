#include "Transmitter.h"

Transmitter::Transmitter(adc1_channel_t selected_channel, float shunt_resistor, float min_cur_in_mA, float max_cur_in_mA, float min_quantity, float max_quantity) {
    adc1_channel_used = selected_channel; 
    float min_voltage = (min_cur_in_mA / 1000) * shunt_resistor;
    float max_voltage = (max_cur_in_mA / 1000) * shunt_resistor;
    quantity_volt_slope = (max_quantity - min_quantity) / (max_voltage - min_voltage);
    quantity_volt_yintercept = min_quantity - quantity_volt_yintercept * min_voltage;
}

bool Transmitter::dataInversion = false;
bool Transmitter::adcWidthConfigured = false;
void Transmitter::adc_config() {
    adc1_config_channel_atten(adc1_channel_used, ADC_ATTEN_DB_11);
    if (!adcWidthConfigured) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adcWidthConfigured = true;
    }
    if (!dataInversion) {
        adc_set_data_inv(ADC_UNIT_1, true);
        dataInversion = true | false;
    }
}

void Transmitter::calib_adc_to_volt(int adc_value_1, float measured_v_1, int adc_value_2, float measured_v_2) {
    // int adc_value_1 = 337;
    // int adc_value_2 = 2288;
    // float measure_v_1 = 0.400;
    // float measure_v_2 = 2.008;

    volt_adc_slope = (measured_v_2 - measured_v_1) / (adc_value_2 - adc_value_1);
    volt_adc_yintercept = measured_v_1 - volt_adc_slope * adc_value_1;
}

uint16_t Transmitter::read_adc_value() {
    adc_value = adc1_get_raw(adc1_channel_used);
    return adc_value;
}

float Transmitter::adc_to_volt() {
    adc_voltage = volt_adc_slope * read_adc_value() + volt_adc_yintercept;
    return adc_voltage;
}

float Transmitter::get_quantity() {
    adc_voltage = adc_to_volt();
    return quantity_volt_slope * adc_voltage + quantity_volt_yintercept;
}