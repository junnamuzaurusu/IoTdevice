#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <Wire.h>

// External reference to display object defined in .ino file
extern Adafruit_SSD1306 display;

void initDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

bool setupWiFi(const char *ssid, const char *password) {
  display.println("Connecting to ");
  display.println(ssid);
  display.display();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() == WL_IDLE_STATUS || WiFi.status() == WL_DISCONNECTED) {
    delay(1000);
    display.print(".");
    display.display();
    if (display.getCursorY() >= display.height()) {
      display.clearDisplay();
      display.setCursor(0, 0);
    }
  }
  display.clearDisplay();
  display.setCursor(0, 0);
  if (WiFi.status() == WL_CONNECT_FAILED) {
    display.println("Connection failed");
    display.display();
    return false;
  } else if (WiFi.status() == WL_WRONG_PASSWORD) {
    display.println("Wrong password");
    display.display();
    return false;
  } else if (WiFi.status() == WL_DISCONNECTED) {
    display.println("Disconnected");
    display.display();
    return false;
  } else if (WiFi.status() == WL_CONNECTED) {
    display.println("Connection Success");
    display.println("IP address: ");
    display.println(WiFi.localIP());
    display.display();
    return true;
  } else {
    return false;
  }
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
  display.println("Sync to NTP time");
  display.display();
  unsigned long unix_time = getNTPTime(ntp_server);
  if (unix_time == 0UL) {
    display.println("Sync failed");
    display.display();
    return false;
  }
  display.println("Sync success");
  display.display();
  setTime(unix_time);

  delay(1000);

  return true;
}

char *getCurrentTime() {
  static char str_time[30];
  unsigned long t = now();

  sprintf(str_time, "%04d-%02d-%02dT%02d:%02d:%02d", year(t), month(t), day(t),
          hour(t), minute(t), second(t));

  return str_time;
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

int getIlluminance() {
  int illuminance = analogRead(A0);
  return illuminance;
}

bool getMDStatus() {
  int stat = digitalRead(16);
  if (stat == HIGH) {
    return true;
  } else {
    return false;
  }
}

bool connectToServer(WiFiClient &client, const char *host, const int port) {
  display.print("Connecting to ");
  display.print(host);
  display.print(":");
  display.println(port);
  display.display();
  if (!client.connect(host, port)) {
    display.print("..Err\r\n");
    display.display();
    return false;
  }
  display.println("TCP Connected");
  display.display();
  return true;
}

void displayCurrentTCPStatus(WiFiClient &client) {
  // Wait for server response with timeout
  unsigned long timeout = millis() + 5000; // 5 second timeout
  while (!client.available() && millis() < timeout) {
    delay(10);
  }

  if (!client.available()) {
    display.println("...Timeout\r\n");
    display.display();
    return;
  }

  // Read response line (server sends "OK\r\n" or "ERR\r\n")
  String response = client.readStringUntil('\n');
  response.trim(); // Remove \r\n and whitespace

  if (response == "OK") {
    display.println("...OK\r\n");
    display.display();
  } else if (response == "ERR") {
    display.println("...ERR\r\n");
    display.display();
  } else {
    display.print("...Unknown: ");
    display.println(response);
    display.display();
  }
}
