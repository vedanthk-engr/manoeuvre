// firmware/include/events.h
#pragma once
#include <Arduino.h>

void sendCursorMove(int dx, int dy);
void sendAction(const char* name);
void sendCommand(const char* name);
unsigned long getTimestampMs();

