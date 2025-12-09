// firmware/src/wifi_ws.h
#pragma once
#include <Arduino.h>

// Placeholder for Phase 3 - WiFi and WebSocket functionality
// For now, events are sent via Serial
// Later this will handle:
// - WiFi connection
// - WebSocket client to ws://<LAPTOP_IP>:8765/glove
// - Sending JSON events over WebSocket instead of Serial

class WiFiWSHandler {
public:
  bool begin(const char* ssid, const char* password, const char* serverIP, int port);
  void sendEvent(const char* jsonEvent);
  void loop();
  bool isConnected() const { return connected; }

private:
  bool connected = false;
  // WebSocket client will be added here later
};

