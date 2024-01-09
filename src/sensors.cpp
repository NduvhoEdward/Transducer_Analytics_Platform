#include "sensors.h"
#include "variables.h"
#include "pump.h"

String getSensorReadings() {
    readings["height"] = String(current_height);
    readings["volume"] = String(total_volume_gal);
    readings["density"] = String(density);
    readings["k_factor"] = String(f_sensor_k_factor_ppg);
    readings["max_height"] = String(tank_max_height);
    readings["sampling_rate"] = String(sampling_rate_f);

    String jsonString = JSON.stringify(readings);
    return jsonString;
}

void initSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
    }
    Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

void notifyClients(String sensorReadings) {
    ws.textAll(sensorReadings);
}


void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        String message = (char *)data;

        if (strcmp((char *)data, "getReadings") == 0) {
            String sensorReadings = getSensorReadings();
            notifyClients(sensorReadings);
        } else if (strcmp((char *)data, "clear") == 0) {
            current_height = 0; 
            maxCountsCycle = 0;
            totalPulseCounts = 0; 
            pumping = false;
        } else if (strcmp((char *)data, "reset") == 0) {
            ESP.restart();
        } else if (strcmp((char *)data, "start_stop") == 0) {
            start_stop_pumping();
        } else {
            handleNumericMessage(message);
        }
    }
}
void handleNumericMessage(String message) {
    // Split the message into type and value
    int separatorIndex = message.indexOf(':');
    if (separatorIndex != -1) {
        String variableName = message.substring(0, separatorIndex);
        float variableValue = message.substring(separatorIndex + 1).toFloat();

        // Handle different variable types
        if (variableName == "density") {
            density = variableValue;
        } else if (variableName == "max_height") {
            tank_max_height = variableValue;
        } else if (variableName == "k_factor") {
            f_sensor_k_factor_ppg = variableValue;
        } else if (variableName == "sampling_rate") {
            sampling_rate_f = variableValue;
        } else {
            Serial.println("Unknown variable type");
        }
    } else {
        Serial.println("Invalid numeric message format");
    }
    notifyClients(getSensorReadings());
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
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
