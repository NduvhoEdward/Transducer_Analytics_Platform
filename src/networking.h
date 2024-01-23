#pragma once
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>

#include <string>

#include "SPIFFS.h"

#define MAX_CSV_BYTES 512

extern const char *ssid;
extern const char *password;
extern AsyncWebServer server;
extern AsyncWebSocket ws;
extern char batch_readings[];

size_t getSensorReadings(char *buf, size_t bufferSize);
void initSPIFFS();
void initWiFi();
void notifyClients();
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void handleNumericMessage(std::string message);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
void update_buffer(const char *sensor_readings, size_t lineLen);
