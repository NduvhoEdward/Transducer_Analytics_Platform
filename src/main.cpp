#include "variables.h"
#include "gpios.h"
#include "pump.h"
#include "sensors.h" 
#include "adc_functions.h"
#include "volume_functions.h"

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
}

void loop() {
    digitalWrite(PUMP_OFF_LIGHT_PIN, HIGH);
    digitalWrite(PUMP_ON_LIGHT_PIN, LOW);
    digitalWrite(VALVE_CONTROL_PIN, LOW);

    Serial.println("\n\nValve closed..... (Not pumping) \n\n");
    current_height = 0;
    total_volume_gal = 0;

    while (!pumping) {
        NOP();
    }
    pcnt_counter_clear(PCNT_UNIT_0);
    pump();
}

