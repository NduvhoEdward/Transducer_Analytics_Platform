#pragma once
#include <Arduino.h>
#include <Arduino_JSON.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include <ESPmDNS.h>

extern const char *ssid;
extern const char *password;
extern AsyncWebServer server;
extern AsyncWebSocket ws;

extern JSONVar readings;

String getSensorReadings();
void initSPIFFS();
void initWiFi();
void notifyClients(String sensorReadings);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void handleNumericMessage(String message);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
