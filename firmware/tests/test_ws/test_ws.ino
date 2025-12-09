#include <WiFi.h>
#include <WebSocketsClient.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";

WebSocketsClient ws;

void onMessage(WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    Serial.print("Received: ");
    Serial.println((char*)payload);
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Replace with your backend IP
  ws.begin("192.168.0.100", 8765, "/glove");
  ws.onEvent(onMessage);
}

void loop() {
  ws.loop();
  ws.sendTXT("{\"test\":\"hello\"}");
  delay(1000);
}



