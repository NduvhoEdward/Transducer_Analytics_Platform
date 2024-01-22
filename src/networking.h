#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>

#include "SPIFFS.h"

extern const char *ssid;
extern const char *password;
extern AsyncWebServer server;
extern AsyncWebSocket ws;

extern JsonDocument readings;

String getSensorReadings();
void buffer_handling(String sensor_readings);
void initSPIFFS();
void initWiFi();
void notifyClients(String sensorReadings);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void handleNumericMessage(String message);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
