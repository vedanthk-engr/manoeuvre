// firmware/src/wifi_ws.cpp
// WiFi and WebSocket implementation for Phase 3
// Connects ESP32 to backend server via WebSocket

#include "wifi_ws.h"
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

WebSocketsClient webSocket;

// WebSocket event handler
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("[WS] Disconnected");
      break;
    case WStype_CONNECTED:
      Serial.print("[WS] Connected to: ");
      Serial.println((char*)payload);
      break;
    case WStype_TEXT:
      // Backend can send commands back if needed
      Serial.print("[WS] Received: ");
      Serial.println((char*)payload);
      break;
    case WStype_ERROR:
      Serial.print("[WS] Error: ");
      Serial.println((char*)payload);
      break;
    default:
      break;
  }
}

bool WiFiWSHandler::begin(const char* ssid, const char* password, const char* serverIP, int port) {
  Serial.println("========================================");
  Serial.println("Initializing WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  // Wait for connection with timeout
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println();
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Connection failed!");
    connected = false;
    return false;
  }
  
  localIP = WiFi.localIP().toString();
  Serial.println("[WiFi] Connected!");
  Serial.print("[WiFi] IP address: ");
  Serial.println(localIP);
  
  // Initialize WebSocket client
  Serial.println("========================================");
  Serial.println("Connecting to backend WebSocket...");
  Serial.print("Server: ");
  Serial.print(serverIP);
  Serial.print(":");
  Serial.println(port);
  
  webSocket.begin(serverIP, port, "/glove");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);  // Reconnect every 5 seconds if disconnected
  
  // Give it a moment to connect
  delay(1000);
  
  connected = true;
  Serial.println("[WS] WebSocket client initialized");
  Serial.println("========================================");
  
  return true;
}

void WiFiWSHandler::sendEvent(const char* jsonEvent) {
  if (connected && webSocket.isConnected()) {
    webSocket.sendTXT(jsonEvent);
  } else {
    // Fallback to Serial if WebSocket not connected
    Serial.println(jsonEvent);
  }
}

void WiFiWSHandler::loop() {
  if (connected) {
    webSocket.loop();
    
    // Check connection status
    if (!webSocket.isConnected() && WiFi.status() == WL_CONNECTED) {
      // WiFi connected but WebSocket disconnected - will auto-reconnect
      static unsigned long lastReconnectMsg = 0;
      unsigned long now = millis();
      if (now - lastReconnectMsg > 5000) {
        Serial.println("[WS] Disconnected, attempting to reconnect...");
        lastReconnectMsg = now;
      }
    }
  }
}
