#include "control.h"
#include "networking.h" 
#include "gpios.h" 

#include "pressure_transmitter.h" 
#include "flow_sensor.h" 

void setup() {
    Serial.begin(115200);
    configureGPIOs();
    adcInit();
    calib_adc_and_v();
    pulseCounterInit();

    attachInterrupt(START_PUMP_BUTTON, start_stop_pumping, HIGH);

    initWiFi();
    initSPIFFS();
    initWebSocket();
    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    server.serveStatic("/", SPIFFS, "/");
    server.begin();  // Start server

    digitalWrite(CIRCUIT_READINESS_LED, HIGH);

    digitalWrite(PUMP_OFF_LIGHT_PIN, HIGH);
    digitalWrite(PUMP_ON_LIGHT_PIN, LOW);
    digitalWrite(VALVE_CONTROL_PIN, LOW);

    current_height = 0;
    total_volume_gal = 0;
    pcnt_counter_clear(PCNT_UNIT_0);
}

void loop() {
    Serial.println("\n\nValve closed..... (Not pumping) \n\n");

    while (!pumping) {
        p_adc_value = get_p_adc_value();
        p_adc_voltage = p_adc_to_volts();
        pressure_mbars = get_p_from_v();
        current_height = (pressure_mbars * bars_to_pa_multiplier) * m_to_mm_multiplier / (density * grav_acc);
        current_height -= zero_height; 

        String sensorReadings = getSensorReadings();
        notifyClients(sensorReadings);
        delay(3000);
    }

    pump();
}
