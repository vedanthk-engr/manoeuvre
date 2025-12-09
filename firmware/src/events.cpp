// firmware/src/events.cpp
#include "events.h"
#include "config.h"
#include "wifi_ws.h"

// External reference to WiFi/WebSocket handler (defined in main.cpp)
extern WiFiWSHandler* wifiWS_ptr;

unsigned long getTimestampMs() {
  return millis();
}

// Helper to build JSON and send via WebSocket or Serial
void sendJSONEvent(const char* type, const char* jsonBody) {
  char buffer[256];
  unsigned long ts = getTimestampMs();
  
  // Build JSON: {"type":"...",...,"timestamp":...}
  snprintf(buffer, sizeof(buffer), "{\"type\":\"%s\",%s,\"timestamp\":%lu}", type, jsonBody, ts);
  
  // Send via WebSocket if enabled and connected, otherwise Serial
  if (ENABLE_WIFI && wifiWS_ptr && wifiWS_ptr->isConnected()) {
    wifiWS_ptr->sendEvent(buffer);
  } else {
    Serial.println(buffer);
  }
}

void sendCursorMove(int dx, int dy) {
  char jsonBody[64];
  snprintf(jsonBody, sizeof(jsonBody), "\"dx\":%d,\"dy\":%d", dx, dy);
  sendJSONEvent("cursor_move", jsonBody);
}

void sendAction(const char* name) {
  char jsonBody[64];
  snprintf(jsonBody, sizeof(jsonBody), "\"name\":\"%s\"", name);
  sendJSONEvent("action", jsonBody);
}

void sendCommand(const char* name) {
  char jsonBody[64];
  snprintf(jsonBody, sizeof(jsonBody), "\"name\":\"%s\"", name);
  sendJSONEvent("command", jsonBody);
}

