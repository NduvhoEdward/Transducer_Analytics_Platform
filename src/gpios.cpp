#include "gpios.h"
#include "variables.h"

void configureGPIOs() {
    pinMode(PRESSURE_TRANSMITTER_PIN, INPUT);
    pinMode(PULSE_COUNTER_PIN, INPUT_PULLDOWN);
    pinMode(START_PUMP_BUTTON, INPUT_PULLDOWN);
    pinMode(VALVE_CONTROL_PIN, OUTPUT);

    pinMode(PUMP_ON_LIGHT_PIN, OUTPUT);
    pinMode(PUMP_OFF_LIGHT_PIN, OUTPUT);
    pinMode(ONBOARD_LED_PIN, OUTPUT); 
    pinMode(CIRCUIT_READINESS_LED, OUTPUT);
}

void pulseCounterInit() {
    // Function to initialize the pulse counter
    static pcnt_config_t pcnt_config = {
        .pulse_gpio_num = PULSE_COUNTER_PIN,
        .ctrl_gpio_num = PCNT_PIN_NOT_USED,
        .lctrl_mode = PCNT_MODE_KEEP,
        .hctrl_mode = PCNT_MODE_KEEP,
        .pos_mode = PCNT_COUNT_INC,
        .neg_mode = PCNT_COUNT_DIS,
        .counter_h_lim = PCNT_UPPER_LIM,
        .counter_l_lim = 0,
        .unit = PCNT_UNIT_0,
        .channel = PCNT_CHANNEL_0};
    pcnt_unit_config(&pcnt_config);

    // Configure and enable the input filter
    pcnt_set_filter_value(PCNT_UNIT_0, 500);
    pcnt_filter_enable(PCNT_UNIT_0);

    // To initialize the counter from Zero...
    pcnt_counter_pause(PCNT_UNIT_0);
    pcnt_counter_clear(PCNT_UNIT_0);

    // Register ISR handler and enable interrupts for PCNT unit
    pcnt_isr_service_install(0);
    pcnt_isr_handler_add(PCNT_UNIT_0, pulseCounterISR, NULL);

    pcnt_event_enable(PCNT_UNIT_0, PCNT_EVT_H_LIM);
    pcnt_intr_enable(PCNT_UNIT_0);

    pcnt_counter_resume(PCNT_UNIT_0);
}

void adcInit() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    adc_set_data_inv(ADC_UNIT_1, true | false);
} 

static void IRAM_ATTR pulseCounterISR(void *arg) {
    maxCountsCycle++;
}
