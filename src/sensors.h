// Declarations for functions and logic related to sensor readings. 
#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

String getSensorReadings();
void initSPIFFS();
void initWiFi();
void notifyClients(String sensorReadings);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void handleNumericMessage(String message);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
