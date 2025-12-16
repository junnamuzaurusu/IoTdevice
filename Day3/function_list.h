#ifndef _FUNCTION_LIST_H
#define _FUNCTION_LIST_H

#include <Arduino.h>

void setBZ(boolean on);
bool getPushSWStatus();
extern int prev_stat;
bool detectPushSWON();
int getDIPSWStatus();
bool getMDStatus();
int getIlluminance();
unsigned long getNTPTime(const char *ntp_server);
bool syncNTPTime(const char *ntp_server);

#endif