#include "networking.h"

#include <iostream>
#include <string>

#include "control.h"
#include "flow_sensor.h"
#include "gpios.h"
#include "pressure_transmitter.h"

const char *ssid = "IMINING HO";
const char *password = "1Min!Ng010";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

char batch_readings[MAX_CSV_BYTES];
size_t batch_readings_size = 0;

size_t getSensorReadings(char *buf, size_t bufferSize) {
    return snprintf(buf, bufferSize, "%.3f, %.3f, %.2f, %.2f, %.3f, %.3f, %.2f\n",
                    current_height,
                    total_volume_gal,
                    density,
                    f_sensor_k_factor_ppg,
                    zero_height,
                    tank_max_height,
                    sampling_rate_hz);
}

void update_buffer(const char *sensor_readings, size_t lineLen) {
    // log_e // log_w // log_i // log_d // log_v

    if (batch_readings_size + lineLen >= MAX_CSV_BYTES) {
        batch_readings[batch_readings_size] = '\0';
        notifyClients();
    }

    memcpy(batch_readings + batch_readings_size, sensor_readings, lineLen);
    batch_readings_size += lineLen;
    batch_readings[batch_readings_size] = '\r';

    for (int i = 0; i < batch_readings_size; i++) {
        Serial.print(batch_readings[i]);
    }

    batch_readings_size++;
}

void initSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
    }
    Serial.println("SPIFFS mounted successfully");
}

void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');

        digitalWrite(CIRCUIT_READINESS_LED, HIGH);
        delay(60);
        digitalWrite(CIRCUIT_READINESS_LED, LOW);
        delay(100);
    }
    Serial.println(WiFi.localIP());

    // Set up mDNS with the hostname "esp32"
    if (!MDNS.begin("esp32analytics")) {
        Serial.println("Error setting up mDNS");
        while (1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");
}

void notifyClients() {
    ws.textAll(batch_readings);

    memset(batch_readings, 0, MAX_CSV_BYTES);
    batch_readings_size = 0;
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        std::string message = (char *)data;

        if (strcmp((char *)data, "getReadings") == 0) {
        } else if (strcmp((char *)data, "clear") == 0) {
            current_height = 0;
            maxCountsCycle = 0;
            totalPulseCounts = 0;
            pumping = false;
        } else if (strcmp((char *)data, "reset") == 0) {
            ESP.restart();
        } else if (strcmp((char *)data, "start_stop") == 0) {
            start_stop_pumping();
        } else if (strcmp((char *)data, "zero") == 0) {
            zero_height = current_height;
        } else {
            handleNumericMessage(message);
        }
    }
}

void handleNumericMessage(std::string message) {
    // Find the position of the separator
    size_t separatorIndex = message.find(':');

    if (separatorIndex != std::string::npos) {
        // Extract variableName and variableValue
        std::string variableName = message.substr(0, separatorIndex);
        std::string valueSubstring = message.substr(separatorIndex + 1);

        try {
            // Convert valueSubstring to float using std::stof
            float variableValue = std::stof(valueSubstring);

            // Handle different variable types
            if (variableName == "density") {
                density = variableValue;
            } else if (variableName == "max_height") {
                tank_max_height = variableValue;
            } else if (variableName == "k_factor") {
                f_sensor_k_factor_ppg = variableValue;
            } else if (variableName == "sampling_rate") {
                sampling_rate_hz = variableValue;
            } else {
                std::cerr << "Unknown variable type: " << variableName << std::endl;
            }
        } catch (const std::invalid_argument &e) {
            std::cerr << "Error converting value to float: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Invalid numeric message format" << std::endl;
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}
