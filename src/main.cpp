#include <Arduino.h>
#include <driver/pcnt.h>

#include "driver/adc.h"

#define PRESSURE_TRANSMITTER_PIN GPIO_NUM_34
#define PULSE_COUNTER_PIN GPIO_NUM_36
#define VALVE_CONTROL_PIN GPIO_NUM_32
#define START_PUMP_BUTTON GPIO_NUM_35
#define ONBOARD_LED_PIN GPIO_NUM_2

#define PUMP_ON_LIGHT_PIN GPIO_NUM_12
#define PUMP_OFF_LIGHT_PIN GPIO_NUM_13

#define PCNT_UPPER_LIM INT16_MAX

#define P_SHUNT_RESISTOR_OHMS 100.0
#define PT_MIN_CUR_IN_mA 4.0
#define PT_MAX_CUR_IN_mA 20.0
#define P_MIN 0.0
#define P_MAX 1.0

#define F_SENSOR_K_FACTOR_PPG 197.77
#define TANK_MAX_HEIGHT 5
#define DENSITY 1000

uint16_t p_adc_value = 0;
uint32_t totalPulseCounts = 0;
int16_t pulseCount = 0;
volatile int maxCountsCycle = 0;
volatile bool pumping = false;

// Pressure reading vars
const float p_adc_vref = 3.3;  // Only used before calib, to get the adc_v values to create a v-adc eq...
float p_adc_voltage = 0;
float adc_v_slope = 0;
float adc_volts_yIntercept = 0;
float current_height = 0;
const float grav_acc = 9.80665;
const int bars_to_pa_multiplier = 100000;

const int p_adc_max = 4095;  // 0xFFF;  // In adc 12bit mode
float pressure_bars = 0.0;

const float p_shunt_resistor = P_SHUNT_RESISTOR_OHMS;
const float transmitter_min_cur = PT_MIN_CUR_IN_mA / 1000;
const float transmitter_max_cur = PT_MAX_CUR_IN_mA / 1000;
const float p_at_min_cur = P_MIN;
const float p_at_max_cur = P_MAX;

const float v_at_min_cur = transmitter_min_cur * p_shunt_resistor;
const float v_at_max_cur = transmitter_max_cur * p_shunt_resistor;

const float vp_slope = (p_at_max_cur - p_at_min_cur) / (v_at_max_cur - v_at_min_cur);
const float yIntercept = p_at_min_cur - vp_slope * v_at_min_cur;

// Button debounce vars
unsigned long lastDebounceTime = 0;  // Last time the button state changed
unsigned long debounceDelay = 1000;  // Debounce time in milliseconds

// Volume reading vars
const float k_factor = F_SENSOR_K_FACTOR_PPG;
float total_volume_gal = 0.0;

// function declarations:
void configureGPIOs();
void pulseCounterInit();
static void IRAM_ATTR pulseCounterISR(void* arg);
uint32_t getTotalPulses();
void adcInit();
uint16_t get_p_adc_value();
float getCurrentMass();
float p_adc_to_volts();
void calib_adc_and_v();
float get_p_from_v();
void IRAM_ATTR start_pumping();
void pump();

void setup() {
    configureGPIOs();
    adcInit();
    calib_adc_and_v();
    pulseCounterInit();

    attachInterrupt(START_PUMP_BUTTON, start_pumping, HIGH);

    Serial.begin(115200);
}

void loop() {
    digitalWrite(PUMP_OFF_LIGHT_PIN, HIGH);
    digitalWrite(PUMP_ON_LIGHT_PIN, LOW);
    digitalWrite(VALVE_CONTROL_PIN, LOW);

    Serial.println("\n\nValve closed..... (Not pumping) \n\n");
    while (!pumping) {
        NOP();
    }
    pump();
}

// function definitions:
void configureGPIOs() {
    pinMode(PRESSURE_TRANSMITTER_PIN, INPUT);
    pinMode(PULSE_COUNTER_PIN, INPUT_PULLDOWN);
    pinMode(START_PUMP_BUTTON, INPUT_PULLDOWN);
    pinMode(VALVE_CONTROL_PIN, OUTPUT);

    pinMode(PUMP_ON_LIGHT_PIN, OUTPUT);
    pinMode(PUMP_OFF_LIGHT_PIN, OUTPUT);
    pinMode(ONBOARD_LED_PIN, OUTPUT);
}

void adcInit() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    adc_set_data_inv(ADC_UNIT_1, true | false);
}

uint16_t get_p_adc_value() {
    return adc1_get_raw(ADC1_CHANNEL_6);
}

void calib_adc_and_v() {
    int adc_value_1 = 337;
    int adc_value_2 = 2288;
    float measure_v_1 = 0.400;
    float measure_v_2 = 2.008;

    adc_v_slope = (measure_v_2 - measure_v_1) / (adc_value_2 - adc_value_1);
    adc_volts_yIntercept = measure_v_1 - adc_v_slope * adc_value_1;
}

float p_adc_to_volts() {
    // return p_adc_value * p_adc_vref / p_adc_max;
    return (adc_v_slope * p_adc_value + adc_volts_yIntercept);
}

float get_p_from_v() {
    return (vp_slope * p_adc_voltage + yIntercept);
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

static void IRAM_ATTR pulseCounterISR(void* arg) {
    maxCountsCycle++;
}

uint32_t getTotalPulses() {
    pcnt_get_counter_value(PCNT_UNIT_0, &pulseCount);
    uint32_t total_pls = maxCountsCycle * PCNT_UPPER_LIM + pulseCount;
    return total_pls;
}

float getCurrentMass() {
    return totalPulseCounts / k_factor;
}

void IRAM_ATTR start_pumping() {
    unsigned long currentMillis = millis();

    // Check if enough time has passed since the last interrupt
    if (currentMillis - lastDebounceTime >= debounceDelay) {
        lastDebounceTime = currentMillis;

        if (!pumping) {
            pumping = true;
        } else {
            pumping = false;
        }
    }
}

void pump() {
    Serial.println("\\n\nValve open..... (Pumping)\n\n");
    digitalWrite(VALVE_CONTROL_PIN, HIGH);
    digitalWrite(PUMP_OFF_LIGHT_PIN, LOW);
    digitalWrite(PUMP_ON_LIGHT_PIN, HIGH);

    // Print headers
    Serial.println("ADC Value(dec)\tADC Voltage(V)\tPressure(mBars)\tHeight(m)\tPulse Counter Value\tTotal Volume(gal)");

    while (current_height < 0.95 * TANK_MAX_HEIGHT && pumping == true) {
        p_adc_value = get_p_adc_value();
        p_adc_voltage = p_adc_to_volts();
        pressure_bars = get_p_from_v();
        current_height = (pressure_bars * bars_to_pa_multiplier) / (DENSITY * grav_acc);

        totalPulseCounts = getTotalPulses();
        total_volume_gal = getCurrentMass();

        // Data
        Serial.print(p_adc_value);
        Serial.print("\t\t");
        Serial.print(p_adc_voltage);
        Serial.print("\t\t");
        Serial.print(pressure_bars * 1000);
        Serial.print("\t\t");
        Serial.print(current_height);
        Serial.print("\t\t");
        Serial.print(static_cast<int>(totalPulseCounts));
        Serial.print("\t\t");
        Serial.println(total_volume_gal);

        delay(1000);  // Can be lowered to get more readings / data points
        // current_height += 10;
    }
    pumping = false;
}
