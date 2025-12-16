#include "function_list.h"
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

void setBZ(bool on) {
  if (on == true) {
    digitalWrite(14, HIGH);
  } else if (on == false) {
    digitalWrite(14, LOW);
  } else {
    Serial.println("Invaild Input!");
    exit(1);
  }
}

bool getPushSWStatus() {
  int stat = digitalRead(2);
  if (stat == LOW) {
    return true;
  } else if (stat == HIGH) {
    return false;
  } else {
    Serial.println("Invaild Input!");
    exit(1);
  }
}

int prev_stat = HIGH;
bool detectPushSWON() {
  int stat = digitalRead(2);
  if (stat == LOW && prev_stat == HIGH) {
    prev_stat = stat;
    return true;
  } else {
    prev_stat = stat;
    return false;
  }
}

int getDIPSWStatus() {
  int stat = 0;
  int bit1 = digitalRead(12);
  int bit0 = digitalRead(13);
  if (bit0 == LOW) {
    stat |= 0x01;
  }
  if (bit1 == LOW) {
    stat |= 0x02;
  }
  return stat;
}

bool getMDStatus() {
  int stat = digitalRead(16);
  if (stat == HIGH) {
    return true;
  } else if (stat == LOW) {
    return false;
  } else {
    Serial.println("Invaild Input!");
    exit(1);
  }
}

int getIlluminance() {
  int illuminance = analogRead(A0);
  return illuminance;
}

unsigned long getNTPTime(const char *ntp_server) {
  WiFiUDP udp;
  udp.begin(8888);
  unsigned long unix_time = 0UL;

  byte packet[48];
  memset(packet, 0, sizeof(packet));
  packet[0] = 0b11100011;
  packet[1] = 0;
  packet[2] = 6;
  packet[3] = 0xEC;
  packet[12] = 49;
  packet[13] = 0x4E;
  packet[14] = 49;
  packet[15] = 52;

  udp.beginPacket(ntp_server, 123);
  udp.write(packet, sizeof(packet));
  udp.endPacket();

  for (int i = 0; i < 100; i++) {
    delay(500);
    if (udp.parsePacket()) {
      udp.read(packet, sizeof(packet));
      unsigned long highWord = word(packet[40], packet[41]);
      unsigned long lowWord = word(packet[42], packet[43]);
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      const unsigned long seventyYears = 2208988800UL;
      unix_time = secsSince1900 - seventyYears + 32400UL; //+32400UL for JST
      return unix_time;
    }
  }
  return unix_time;
}

bool syncNTPTime(const char *ntp_server) {
  unsigned long unix_time = getNTPTime(ntp_server);
  if (unix_time == 0UL) {
    return false;
  }
  setTime(unix_time);
  return true;
}