#include "Day4_Day5.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <Wire.h>

const char ssid[] = "bld2-guest";
const char password[] = "MatsumotoWay";
const char tcp_server_host[] = "iot.hongo.wide.ad.jp";
const int tcp_server_port = 10250;

const char *ntp_server = "ntp.nict.jp";
unsigned long last_sync_time = 0;
unsigned long last_observed_time = 0;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 2
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(A0, INPUT);

  initDisplay();

  bool isSetupWiFi = setupWiFi(ssid, password);
  if (!isSetupWiFi) {
    display.println("WiFi setup failed!");
    display.display();
    while (1) {
      delay(1000);
    } // Halt on error
  }
  display.println("");
  display.display();

  bool isSyncNTPTime = syncNTPTime(ntp_server);
  if (!isSyncNTPTime) {
    display.println("NTP sync failed!");
    display.display();
    while (1) {
      delay(1000);
    }
  }
  char *str_time = getCurrentTime();
  display.println(str_time);
  display.display();
}

void loop() {

  if (now() / 10 != last_observed_time / 10) {
    int id = getDIPSWStatus();
    char *str_time = getCurrentTime();
    int illuminance = getIlluminance();
    bool md_stat = getMDStatus();

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(id);
    display.println(str_time);
    display.println(illuminance);
    display.println(md_stat);
    display.display();

    WiFiClient client;
    bool isConnected =
        connectToServer(client, tcp_server_host, tcp_server_port);
    if (!isConnected) {
      return;
    }

    char send_buffer[100];
    sprintf(send_buffer, "%d,%s,%d,%d\r\n", id, str_time, illuminance, md_stat);
    client.print(send_buffer);

    displayCurrentTCPStatus(client);

    client.stop();
    last_observed_time = now();
  }

  if (now() / 300 != last_sync_time / 300) {
    syncNTPTime(ntp_server);
    last_sync_time = now();
  }
  delay(1000);
}
