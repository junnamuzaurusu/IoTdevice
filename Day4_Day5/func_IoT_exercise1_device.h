#ifndef DAY4_DAY5_H
#define DAY4_DAY5_H

#include <ESP8266WiFi.h>

void initDisplay();

bool setupWiFi(const char *ssid, const char *password);

bool syncNTPTime(const char *ntp_server);

char *getCurrentTime();
int getDIPSWStatus();
int getIlluminance();
bool getMDStatus();

bool connectToServer(WiFiClient &client, const char *host, const int port);
void displayCurrentTCPStatus(WiFiClient &client);

#endif // DAY4_DAY5_H