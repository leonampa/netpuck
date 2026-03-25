/* NetPuck 5G - Core Firmware (ESP32-S3) */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// --- DISPLAY PINS (ST7735) ---
#define TFT_MOSI  11  // GP11
#define TFT_SCLK  12  // GP12
#define TFT_CS    10  // GP10
#define TFT_DC     8  // GP8
#define TFT_RST    7  // GP7
#define TFT_BL     5  // GP5 (Backlight Control)

// --- W5500 ETHERNET PINS ---
#define W5500_CS   9  // GP9
#define W5500_RST  6  // GP6
#define SPI_MISO  13  // GP13

// --- DIGITAL JOYSTICK PINS ---
#define JOY_MID   34  // GP34
#define JOY_UP    35  // GP35
#define JOY_DOWN  36  // GP36
#define JOY_LEFT  37  // GP37
#define JOY_RIGHT 38  // GP38

// --- BW16 5GHz MODULE PINS ---
#define BW16_EN   14  // GP14
#define BW16_RX   17  // GP17 (ESP32 TX -> BW16 RX)
#define BW16_TX   18  // GP18 (ESP32 RX <- BW16 TX)

// --- UI SETTINGS ---
#define COLOR_BG      0x0000 
#define COLOR_TEXT    0xFFFF 
#define COLOR_ACCENT  0x07E0 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// --- APP STATES ---
enum State { MENU, SCANNING, RESULTS };
State currentState = MENU;

// --- MENU VARIABLES ---
int currentIdx = 0;
const int MAX_ITEMS = 4;
String menuItems[MAX_ITEMS] = {"5GHz SCANNER", "ETHERNET DIAG", "PACKET SNIFFER", "SETTINGS"};
bool redraw = true;

// --- SCAN VARIABLES ---
String scanSSID[8];
String scanRSSI[8];
int netCount = 0;

void setup() {
  // 1. PIN MODES
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Backlight ON

  pinMode(W5500_CS, OUTPUT);
  digitalWrite(W5500_CS, HIGH); // De-select W5500 (Crucial!)

  pinMode(BW16_EN, OUTPUT);
  digitalWrite(BW16_EN, HIGH); 

  // 2. JOYSTICK
  pinMode(JOY_UP, INPUT_PULLUP);
  pinMode(JOY_DOWN, INPUT_PULLUP);
  pinMode(JOY_LEFT, INPUT_PULLUP);
  pinMode(JOY_RIGHT, INPUT_PULLUP);
  pinMode(JOY_MID, INPUT_PULLUP);

  // 3. START DISPLAY
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(COLOR_BG);
  
  // 4. START UART
  Serial1.begin(115200, SERIAL_8N1, BW16_RX, BW16_TX); 
}

void loop() {
  if (currentState == MENU) {
    handleMenu();
  } else if (currentState == SCANNING) {
    handleScanning();
  } else if (currentState == RESULTS) {
    handleResults();
  }
}

// --- LOGIC FUNCTIONS ---

void handleMenu() {
  if (redraw) {
    drawMenu();
    redraw = false;
  }
  
  if (digitalRead(JOY_UP) == LOW) {
    currentIdx = (currentIdx <= 0) ? MAX_ITEMS - 1 : currentIdx - 1;
    redraw = true; delay(150);
  }
  if (digitalRead(JOY_DOWN) == LOW) {
    currentIdx = (currentIdx >= MAX_ITEMS - 1) ? 0 : currentIdx + 1;
    redraw = true; delay(150);
  }
  if (digitalRead(JOY_MID) == LOW || digitalRead(JOY_RIGHT) == LOW) {
    if (currentIdx == 0) {
      // Start the Scanner
      currentState = SCANNING;
      tft.fillScreen(COLOR_BG);
      tft.setCursor(10, 60);
      tft.setTextColor(COLOR_ACCENT);
      tft.print("COMMANDING BW16...");
      
      netCount = 0; // Reset network list
      Serial1.println("SCAN"); // Send command to BW16
      delay(200);
    }
  }
}

void handleScanning() {
  // Wait for the BW16 to reply over Serial
  if (Serial1.available()) {
    String line = Serial1.readStringUntil('\n');
    line.trim();

    if (line == "END_SCAN" || line == "NO_NETWORKS") {
      currentState = RESULTS; // Scan finished!
      drawResultsScreen();
    } 
    else if (line.indexOf('|') > 0 && netCount < 8) {
      // Parse the incoming "SSID|RSSI" string
      int split = line.indexOf('|');
      scanSSID[netCount] = line.substring(0, split);
      scanRSSI[netCount] = line.substring(split + 1);
      netCount++;
      
      // Update screen so the user knows it's working
      tft.fillRect(0, 80, 160, 10, COLOR_BG);
      tft.setCursor(10, 80);
      tft.setTextColor(COLOR_TEXT);
      tft.print("Found: " + scanSSID[netCount-1]);
    }
  }
}

void handleResults() {
  // Pressing LEFT goes back to the main menu
  if (digitalRead(JOY_LEFT) == LOW || digitalRead(JOY_MID) == LOW) {
    currentState = MENU;
    redraw = true;
    delay(200);
  }
}

// --- UI DRAWING FUNCTIONS ---

void drawMenu() {
  tft.fillScreen(COLOR_BG);
  tft.fillRect(0, 0, 160, 16, COLOR_ACCENT);
  tft.setTextColor(COLOR_BG);
  tft.setCursor(4, 4);
  tft.print("NETPUCK 5G | READY");

  for (int i = 0; i < MAX_ITEMS; i++) {
    int y = 30 + (i * 18);
    if (i == currentIdx) {
      tft.fillRect(0, y - 4, 160, 16, 0x18C3);
      tft.setTextColor(COLOR_ACCENT);
      tft.setCursor(5, y);
      tft.print("> " + menuItems[i]);
    } else {
      tft.setTextColor(COLOR_TEXT);
      tft.setCursor(15, y);
      tft.print(menuItems[i]);
    }
  }
}

void drawResultsScreen() {
  tft.fillScreen(COLOR_BG);
  tft.fillRect(0, 0, 160, 16, COLOR_ACCENT);
  tft.setTextColor(COLOR_BG);
  tft.setCursor(4, 4);
  tft.print("5GHz NETWORKS FOUND");

  tft.setTextColor(COLOR_TEXT);
  if (netCount == 0) {
    tft.setCursor(10, 40);
    tft.print("No networks found.");
  } else {
    for (int i = 0; i < netCount; i++) {
      int y = 25 + (i * 12);
      // Print SSID on left, RSSI (signal strength) on right
      tft.setCursor(2, y);
      tft.print(scanSSID[i].substring(0, 15)); // Trim long names
      tft.setCursor(120, y);
      tft.print(scanRSSI[i] + "dB");
    }
  }
  
  tft.setTextColor(0x18C3); // Dim grey
  tft.setCursor(2, 115);
  tft.print("< Press LEFT to exit");
}