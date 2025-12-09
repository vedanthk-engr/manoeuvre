// firmware/src/events.cpp
#include "events.h"

unsigned long getTimestampMs() {
  return millis();
}

void sendCursorMove(int dx, int dy) {
  unsigned long ts = getTimestampMs();
  Serial.print("{\"type\":\"cursor_move\",\"dx\":");
  Serial.print(dx);
  Serial.print(",\"dy\":");
  Serial.print(dy);
  Serial.print(",\"timestamp\":");
  Serial.print(ts);
  Serial.println("}");
}

void sendAction(const char* name) {
  unsigned long ts = getTimestampMs();
  Serial.print("{\"type\":\"action\",\"name\":\"");
  Serial.print(name);
  Serial.print("\",\"timestamp\":");
  Serial.print(ts);
  Serial.println("}");
}

void sendCommand(const char* name) {
  unsigned long ts = getTimestampMs();
  Serial.print("{\"type\":\"command\",\"name\":\"");
  Serial.print(name);
  Serial.print("\",\"timestamp\":");
  Serial.print(ts);
  Serial.println("}");
}

