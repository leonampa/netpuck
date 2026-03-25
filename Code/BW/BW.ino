/*
 * BW16 5GHz Coprocessor Firmware
 * Role: UART Slave Scanner 
 */
#include <WiFi.h>

void setup() {
  // Serial uses PA7 (TX) and PA8 (RX) by default on BW16
  Serial.begin(115200);
  
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi_FAIL");
    while (true);
  }
  
  Serial.println("BW16_READY");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    
    if (cmd == "SCAN") {
      // Ameba scanNetworks returns the number of networks found
      int n = WiFi.scanNetworks();
      
      if (n <= 0) {
        Serial.println("NO_NETWORKS");
      } else {
        // Send up to 8 networks back to the ESP32
        int limit = (n > 8) ? 8 : n; 
        for (int i = 0; i < limit; ++i) {
          // Format: SSID|RSSI
          Serial.print(WiFi.SSID(i));
          Serial.print("|");
          Serial.println(WiFi.RSSI(i));
        }
      }
      Serial.println("END_SCAN");
    }
  }
}