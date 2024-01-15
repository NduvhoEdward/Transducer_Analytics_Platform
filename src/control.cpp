#include "control.h" 
#include "networking.h" 
#include "pressure_transmitter.h" 
#include "flow_sensor.h" 
#include "gpios.h" 


volatile bool pumping = false;
float sampling_rate_hz = 2; 
uint32_t sampling_rate_ms = static_cast<uint32_t>((1 / sampling_rate_hz) * 1000);

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 1000;

void IRAM_ATTR start_stop_pumping() {
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
    Serial.println("\n\nValve open..... (Pumping)\n\n");

    digitalWrite(VALVE_CONTROL_PIN, HIGH);
    digitalWrite(PUMP_OFF_LIGHT_PIN, LOW);
    digitalWrite(PUMP_ON_LIGHT_PIN, HIGH);

    // Print headers
    Serial.println("ADC Voltage(mV)\tPressure(mBars)\tHeight(mm)\tPulse Counter Value\tTotal Volume(gal)");

    while (current_height < 0.95 * tank_max_height && pumping == true) {
        // while (pumping == true) {
        if (current_height > 0.95 * tank_max_height) {
            digitalWrite(PUMP_OFF_LIGHT_PIN, HIGH);
        }
        p_adc_value = get_p_adc_value();
        p_adc_voltage = p_adc_to_volts();
        pressure_mbars = get_p_from_v();

        current_height = (pressure_mbars * bars_to_pa_multiplier) * m_to_mm_multiplier / (density * grav_acc);
        current_height -= zero_height; 

        totalPulseCounts = getTotalPulses();
        total_volume_gal = getCurrentVolume();

        // Data
        Serial.print(p_adc_voltage*1000, 3);
        Serial.print("\t\t");
        Serial.print(pressure_mbars, 3);
        Serial.print("\t\t");
        Serial.print(current_height, 3);
        Serial.print("\t\t");
        Serial.print(static_cast<int>(totalPulseCounts));
        Serial.print("\t\t");
        Serial.print(total_volume_gal, 3);
        Serial.print("\n");

        String sensorReadings = getSensorReadings();
        notifyClients(sensorReadings);

        delay(sampling_rate_ms);  // Can be lowered to get more readings / data points
    }

    digitalWrite(VALVE_CONTROL_PIN, LOW);
    digitalWrite(PUMP_OFF_LIGHT_PIN, HIGH);
    digitalWrite(PUMP_ON_LIGHT_PIN, LOW);

    pumping = false;
}