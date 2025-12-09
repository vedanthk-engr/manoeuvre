// firmware/include/wifi_ws.h
#pragma once
#include <Arduino.h>

// WiFi and WebSocket handler for Phase 3
// Connects to backend at ws://<LAPTOP_IP>:8765/glove

class WiFiWSHandler {
public:
  bool begin(const char* ssid, const char* password, const char* serverIP, int port);
  void sendEvent(const char* jsonEvent);
  void loop();
  bool isConnected() const { return connected; }
  
  // Get local IP for debugging
  String getLocalIP() const { return localIP; }

private:
  bool connected = false;
  String localIP = "";
  void* wsClient = nullptr;  // WebSocketsClient pointer (opaque to avoid include)
};
