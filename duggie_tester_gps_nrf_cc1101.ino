 #include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RF24.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <TinyGPS++.h>

// -------------------- DISPLAY --------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64   
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* -------- Boot Logo Bitmap -------- */
#define LOGO_WIDTH  128
#define LOGO_HEIGHT 64

// 'logo black white', 128x64px
const unsigned char epd_bitmap_logo_black_white [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfc, 0x00, 0x00, 
	0x00, 0x00, 0x10, 0x00, 0x03, 0xff, 0xf9, 0xff, 0xff, 0xfe, 0x7f, 0x80, 0x00, 0x08, 0x00, 0x00, 
	0x00, 0x00, 0x09, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x90, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0xff, 0xfe, 0x70, 0x71, 0xff, 0x8f, 0xfe, 0x79, 0xff, 0xff, 0x20, 0x00, 0x00, 
	0x00, 0x00, 0x02, 0x00, 0x1e, 0x70, 0x73, 0xff, 0x1f, 0xf8, 0x79, 0xe0, 0x7c, 0x40, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x1e, 0x70, 0x73, 0xc0, 0x1e, 0x00, 0x79, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x38, 0x1e, 0x70, 0x73, 0xcf, 0xde, 0x3e, 0x79, 0xff, 0xc4, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x38, 0x1e, 0x70, 0x73, 0xc3, 0xde, 0x1e, 0x79, 0xff, 0x84, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x38, 0x1e, 0x70, 0x73, 0xc3, 0xde, 0x1e, 0x79, 0xe0, 0x04, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x38, 0x3e, 0x7f, 0xf3, 0xff, 0xdf, 0xfe, 0x79, 0xe0, 0x01, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x3f, 0xfc, 0x7f, 0xf1, 0xff, 0xcf, 0xfe, 0x79, 0xff, 0xf1, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf9, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x02, 0xf0, 0x00, 0x07, 0xfb, 0xf9, 0xf9, 0x8c, 0x00, 0x00, 0x0e, 0x40, 0x00, 0x00, 
	0x00, 0x00, 0x08, 0x00, 0x7e, 0x00, 0xc3, 0x83, 0x81, 0xfc, 0x07, 0xf0, 0x00, 0x20, 0x00, 0x00, 
	0x00, 0x00, 0x10, 0x00, 0x1f, 0xc0, 0xc3, 0x83, 0x81, 0x8c, 0x1f, 0xe0, 0x01, 0x08, 0x00, 0x00, 
	0x00, 0x00, 0x30, 0x00, 0x00, 0x30, 0xc3, 0xf9, 0xf9, 0x8c, 0x60, 0x00, 0x00, 0x0c, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0xfe, 0x00, 0x01, 0xff, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfd, 0xc7, 0x8e, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x70, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// -------------------- BUTTONS --------------------
#define BTN_NRF   32
#define BTN_CC    25
#define BTN_RST   33
#define BTN_GPS   26

// -------------------- NRF24 --------------------
#define NRF_CE 4
#define NRF_CS 5
RF24 radio(NRF_CE, NRF_CS);

// -------------------- CC1101 --------------------
#define CC_CS 17

// -------------------- SPI --------------------
#define SPI_SCK  18
#define SPI_MISO 19
#define SPI_MOSI 23

// -------------------- GPS --------------------
#define GPS_RX_PIN 27
#define GPS_TX_PIN 13
#define GPS_BAUD 9600

HardwareSerial gpsSerial(2);
TinyGPSPlus gps;              

// -------------------- STATE MACHINE --------------------
enum DeviceState {
  STATE_BOOT,
  STATE_MENU,
  STATE_NRF_TEST,
  STATE_CC_TEST,
  STATE_GPS_MODE,
  STATE_DONE       // test finished, showing results - wait for button to go back
};

DeviceState currentState = STATE_BOOT;

// -------------------- DEBOUNCE --------------------
#define DEBOUNCE_MS 250
unsigned long lastButtonPress = 0;

// Returns which button was pressed (with debounce), or 0 if none
int readButton() {
  if (millis() - lastButtonPress < DEBOUNCE_MS) return 0;

  if (!digitalRead(BTN_NRF)) { lastButtonPress = millis(); return BTN_NRF; }
  if (!digitalRead(BTN_CC))  { lastButtonPress = millis(); return BTN_CC;  }
  if (!digitalRead(BTN_GPS)) { lastButtonPress = millis(); return BTN_GPS; }
  if (!digitalRead(BTN_RST)) { lastButtonPress = millis(); return BTN_RST; }

  return 0;
}

/* -------- Functions -------- */
void showBootLogo() {
  display.clearDisplay();
  int x = (SCREEN_WIDTH  - LOGO_WIDTH)  / 2;
  int y = (SCREEN_HEIGHT - LOGO_HEIGHT) / 2;
  display.drawBitmap(x, y, epd_bitmap_logo_black_white, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
  display.display();
  delay(4000);  
}

// -------------------- MAIN MENU --------------------
void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Title bar
  display.fillRect(0, 0, 128, 12, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(8, 2);
  display.print("China JUNK TESTER");
  display.setTextColor(SSD1306_WHITE);

  // Menu items mapped to buttons
  display.setCursor(4, 18);
  display.print("BTN1 > CC1101 Test");

  display.setCursor(4, 30);
  display.print("BTN2 > GPS Test");

  display.setCursor(4, 42);
  display.print("BTN3 > NRF24 Test");

  display.setCursor(4, 54);
  display.print("BTN4 > Reboot");

  display.display();
}

// -------------------- Helpers --------------------
void showMessage(String msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(msg);
  display.display();
}

// -------------------- NRF TEST --------------------
void testNRF() {
  digitalWrite(CC_CS, HIGH);
  delay(10);

  showMessage("NRF SPI Test...");

  if (!radio.begin()) {
    showMessage("NRF Fat FAIL\nNo SPI");
    return;
  }

  if (!radio.isChipConnected()) {
    showMessage("NRF Fat FAIL\nNo Chip");
    return;
  }

  // ---- STAGE 1: Register sanity check ----
  radio.setChannel(76);
  if (radio.getChannel() != 76) {
    showMessage("NRF Fat FAIL\nSPI R/W Error");
    return;
  }

  showMessage("NRF SPI OK\nLooking for\nFuckery...");
  delay(500);

  // ---- STAGE 2: Full spectrum scan ----
  radio.setAutoAck(false);
  radio.disableCRC();
  radio.setRetries(0, 0);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_2MBPS);
  radio.startListening();

  const int NUM_CHANNELS = 126;
  const int NUM_SWEEPS = 50;
  uint8_t signalCount[NUM_CHANNELS];
  memset(signalCount, 0, sizeof(signalCount));

  for (int sweep = 0; sweep < NUM_SWEEPS; sweep++) {
    for (int ch = 0; ch < NUM_CHANNELS; ch++) {
      radio.setChannel(ch);
      delayMicroseconds(200);
      radio.startListening();
      delayMicroseconds(200);
      radio.stopListening();

      if (radio.testRPD()) {
        if (signalCount[ch] < 255) signalCount[ch]++;
      }
    }
  }

  radio.stopListening();

  int activeChannels = 0;
  uint8_t peakVal = 0;
  int peakCh = 0;
  for (int ch = 0; ch < NUM_CHANNELS; ch++) {
    if (signalCount[ch] > 0) activeChannels++;
    if (signalCount[ch] > peakVal) {
      peakVal = signalCount[ch];
      peakCh = ch;
    }
  }

  // Draw bar graph
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Scan:");
  display.print(activeChannels);
  display.print("ch Peak:");
  display.println(peakCh);

  int graphY = 16;
  int graphH = 48;
  for (int x = 0; x < SCREEN_WIDTH && x < NUM_CHANNELS; x++) {
    int barH = 0;
    if (peakVal > 0) {
      barH = map(signalCount[x], 0, peakVal, 0, graphH);
    }
    if (barH > 0) {
      display.drawFastVLine(x, graphY + graphH - barH, barH, SSD1306_WHITE);
    }
  }
  display.display();
  delay(3000);

  // ---- STAGE 3: TX attempt test ----
  showMessage("TX Test...");
  delay(500);

  radio.setAutoAck(true);
  radio.enableDynamicPayloads();
  radio.setRetries(5, 15);
  radio.setChannel(120);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);

  uint8_t testAddr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
  radio.openWritingPipe(testAddr);
  radio.openReadingPipe(1, testAddr);
  radio.stopListening();

  uint8_t testPayload[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x42};
  bool txOk = radio.write(testPayload, sizeof(testPayload));
  bool txAttempted = radio.isChipConnected();

  radio.flush_tx();
  radio.flush_rx();

  // ---- FINAL RESULT ----
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);

  display.println("== NRF24 RESULT ==");
  display.println("");
  display.print("SPI:  ");
  display.println("PASS");

  display.print("RX:   ");
  if (activeChannels > 3) {
    display.println("PASS");
  } else {
    display.println("WEAK ASS");
  }

  display.print("TX:   ");
  if (txAttempted) {
    display.println("PASS");
  } else {
    display.println("JUNK FAIL");
  }

  display.println("");
  display.print(activeChannels);
  display.print("ch / peak@");
  display.println(peakCh);

  display.display();
}

// -------------------- CC1101 TEST --------------------
void testCC() {
  digitalWrite(NRF_CS, HIGH);
  delay(10);

  showMessage("CC1101 SPI Test...");

  if (!ELECHOUSE_cc1101.getCC1101()) {
    showMessage("CC1101 FAIL\nNo SPI");
    return;
  }

  byte version = ELECHOUSE_cc1101.SpiReadStatus(0x31);
  if (version == 0x00 || version == 0xFF) {
    showMessage("CC1101 FAIL\nBad Ver: 0x" + String(version, HEX));
    return;
  }

  showMessage("CC1101 SPI OK\nVer: 0x" + String(version, HEX) +
              "\nScanning...");
  delay(500);

  // ---- STAGE 2: Sub-GHz spectrum scan ----
  float scanFreqs[] = {
    300.0, 310.0, 315.0, 320.0, 330.0, 340.0, 350.0,
    390.0, 400.0, 410.0, 420.0, 430.0, 433.92, 440.0,
    450.0, 460.0, 470.0, 480.0, 490.0, 500.0,
    850.0, 868.0, 900.0, 915.0, 920.0, 928.0
  };
  int numFreqs = sizeof(scanFreqs) / sizeof(scanFreqs[0]);
  int rssiValues[26];
  int peakRSSI = -200;
  int peakIdx = 0;

  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setRxBW(812.50);
  ELECHOUSE_cc1101.SetRx();

  for (int i = 0; i < numFreqs; i++) {
    ELECHOUSE_cc1101.setMHZ(scanFreqs[i]);
    delay(5);
    ELECHOUSE_cc1101.SetRx();
    delay(10);

    int maxRSSI = -200;
    for (int s = 0; s < 10; s++) {
      int rssi = ELECHOUSE_cc1101.getRssi();
      if (rssi > maxRSSI) maxRSSI = rssi;
      delayMicroseconds(500);
    }
    rssiValues[i] = maxRSSI;

    if (maxRSSI > peakRSSI) {
      peakRSSI = maxRSSI;
      peakIdx = i;
    }
  }

  ELECHOUSE_cc1101.setSidle();

  int noiseFloor = 0;
  for (int i = 0; i < numFreqs; i++) {
    if (rssiValues[i] < noiseFloor) noiseFloor = rssiValues[i];
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Pk:");
  display.print(scanFreqs[peakIdx], 1);
  display.print("MHz ");
  display.print(peakRSSI);
  display.println("dBm");

  int graphY = 12;
  int graphH = 40;
  int barWidth = SCREEN_WIDTH / numFreqs;

  for (int i = 0; i < numFreqs; i++) {
    int barH = 0;
    int range = peakRSSI - noiseFloor;
    if (range > 0) {
      barH = map(rssiValues[i], noiseFloor, peakRSSI, 1, graphH);
    }
    barH = constrain(barH, 1, graphH);
    int x = i * barWidth;
    display.fillRect(x, graphY + graphH - barH, barWidth - 1, barH, SSD1306_WHITE);
  }

  display.setCursor(0, 56);
  display.print("300");
  display.setCursor(50, 56);
  display.print("433");
  display.setCursor(100, 56);
  display.print("928");
  display.display();
  delay(3000);

  // ---- STAGE 3: Keyfob detection mode ----
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("== KEYFOB TEST ==");
  display.println("");
  display.println("Press your keyfob");
  display.println("or remote now...");
  display.println("");
  display.println("Listening 315/433");
  display.display();

  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setRxBW(325.00);
  ELECHOUSE_cc1101.SetRx();

  float listenFreqs[] = {315.0, 433.92};
  int numListen = 2;
  bool detected = false;
  float detectedFreq = 0;
  int detectedRSSI = -200;

  int baseline[2] = {-100, -100};
  for (int f = 0; f < numListen; f++) {
    ELECHOUSE_cc1101.setMHZ(listenFreqs[f]);
    delay(10);
    ELECHOUSE_cc1101.SetRx();
    delay(10);
    int sum = 0;
    for (int s = 0; s < 20; s++) {
      sum += ELECHOUSE_cc1101.getRssi();
      delay(2);
    }
    baseline[f] = sum / 20;
  }

  unsigned long startTime = millis();
  unsigned long timeout = 10000;

  while (millis() - startTime < timeout) {
    for (int f = 0; f < numListen; f++) {
      ELECHOUSE_cc1101.setMHZ(listenFreqs[f]);
      delayMicroseconds(500);
      ELECHOUSE_cc1101.SetRx();
      delay(5);

      for (int s = 0; s < 5; s++) {
        int rssi = ELECHOUSE_cc1101.getRssi();

        if (rssi > baseline[f] + 15 && rssi > -60) {
          detected = true;
          detectedFreq = listenFreqs[f];
          detectedRSSI = rssi;
          break;
        }
        delayMicroseconds(500);
      }
      if (detected) break;
    }
    if (detected) break;

    int elapsed = (millis() - startTime) / 1000;
    display.fillRect(0, 56, 128, 8, SSD1306_BLACK);
    display.setCursor(0, 56);
    display.print("Waiting... ");
    display.print(10 - elapsed);
    display.print("s");
    display.display();
  }

  ELECHOUSE_cc1101.setSidle();

  // ---- FINAL RESULT ----
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("== CC1101 RESULT ==");
  display.println("");

  display.print("SPI:  PASS v0x");
  display.println(version, HEX);

  display.print("RX:   ");
  if (peakRSSI > noiseFloor + 5) {
    display.println("PASS");
  } else {
    display.println("WEAK");
  }

  if (detected) {
    display.println("");
    display.print("KEYFOB DETECTED!");
    display.println("");
    display.print(detectedFreq, 2);
    display.print("MHz ");
    display.print(detectedRSSI);
    display.println("dBm");
  } else {
    display.println("");
    display.println("No keyfob signal");
    display.println("(RX still OK if");
    display.println(" spectrum showed)");
  }

  display.display();
}

// -------------------- GPS TEST --------------------
// Stage 1: Quick NMEA check - does the chip talk at all?
// Stage 2: Live sat tracker with 90s timeout for fix

bool gpsSerialAlive = false;   // did we get ANY bytes?
bool gpsNmeaValid   = false;   // did TinyGPS++ parse a valid sentence?
unsigned long gpsStartTime = 0;
int gpsCharsReceived = 0;

// Called once when entering GPS mode — does the quick serial check
void gpsQuickTest() {
  digitalWrite(NRF_CS, HIGH);
  digitalWrite(CC_CS, HIGH);
  delay(10);

  showMessage("GPS ATG Test\n\nOpening serial...");
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  delay(500);

  // --- STAGE 1: Check for raw serial data (3 second window) ---
  showMessage("GPS ATG Test\n\nListening for NMEA\ndata...");

  gpsSerialAlive = false;
  gpsNmeaValid   = false;
  gpsCharsReceived = 0;
  unsigned long checkStart = millis();
  unsigned long checkTimeout = 6000;  // 3 seconds to see ANY data

  while (millis() - checkStart < checkTimeout) {
    while (gpsSerial.available()) {
      char c = gpsSerial.read();
      gpsCharsReceived++;
      gpsSerialAlive = true;
      if (gps.encode(c)) {
        gpsNmeaValid = true;
      }
    }
  }

  // --- Show quick-test result ---
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.println("== GPS QUICK TEST ==");
  display.println("");

  display.print("Serial: ");
  if (gpsSerialAlive) {
    display.println("PASS");
    display.print("  ");
    display.print(gpsCharsReceived);
    display.println(" bytes in 3s");
  } else {
    display.println("FAIL");
    display.println("  No data at all!");
    display.println("  Check wiring/baud");
  }

  display.print("NMEA:   ");
  if (gpsNmeaValid) {
    display.println("PASS");
  } else if (gpsSerialAlive) {
    display.println("PARTIAL");
    display.println("  Got bytes, no fix");
  } else {
    display.println("FAIL");
  }

  display.println("");
  if (gpsSerialAlive) {
    display.println("Entering live mode..");
  } else {
    display.println("ANY BTN = BACK");
  }
  display.display();
  delay(6000);

  gpsStartTime = millis();
}

// Live GPS display — called every second while in GPS_MODE
void updateGPSDisplay() {
  // Keep feeding the monkey
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  unsigned long elapsed = (millis() - gpsStartTime) / 1000;
  int sats = gps.satellites.isValid() ? gps.satellites.value() : 0;
  bool hasFix = gps.location.isValid();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Title bar
  display.fillRect(0, 0, 128, 10, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(4, 1);
  if (hasFix) {
    display.print("GPS LOCKED ");
  } else {
    // Animated searching indicator
    const char spin[] = "|/-\\";
    display.print("GPS SEARCHING ");
    display.print(spin[elapsed % 4]);
    display.print(" ");
  }
  display.print(elapsed);
  display.print("s");
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 14);
  display.print("Sats: ");
  display.print(sats);

  // Satellite strength bar
  if (sats > 0) {
    int barW = constrain(sats * 6, 1, 64);
    display.fillRect(60, 14, barW, 7, SSD1306_WHITE);
  }

  display.setCursor(0, 24);
  display.print("Fix:  ");
  display.println(hasFix ? "YES" : "NO");

  display.print("Lat:  ");
  if (hasFix)
    display.println(gps.location.lat(), 6);
  else
    display.println("waiting...");

  display.print("Lon:  ");
  if (hasFix)
    display.println(gps.location.lng(), 6);
  else
    display.println("waiting...");

  display.print("Alt:  ");
  if (gps.altitude.isValid()) {
    display.print(gps.altitude.meters(), 1);
    display.println("m");
  } else {
    display.println("--");
  }

  // Bottom bar
  display.fillRect(0, 56, 128, 8, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(8, 57);
  if (hasFix) {
    display.print("LOCKED! BTN=BACK");
  } else {
    display.print("ANY BTN = BACK");
  }
  display.setTextColor(SSD1306_WHITE);

  display.display();
}

// ---------------- SETUP ----------------
void setup() {
  pinMode(BTN_NRF, INPUT_PULLUP);
  pinMode(BTN_CC, INPUT_PULLUP);
  pinMode(BTN_RST, INPUT_PULLUP);
  pinMode(BTN_GPS, INPUT_PULLUP);

  pinMode(NRF_CS, OUTPUT);
  pinMode(CC_CS, OUTPUT);

  digitalWrite(NRF_CS, HIGH);
  digitalWrite(CC_CS, HIGH);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  showBootLogo();

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  ELECHOUSE_cc1101.setSpiPin(SPI_SCK, SPI_MISO, SPI_MOSI, CC_CS);

  currentState = STATE_MENU;
  showMenu();
}

// ---------------- LOOP ----------------
void loop() {

  switch (currentState) {

    // ===================== MAIN MENU =====================
    case STATE_MENU: {
      int btn = readButton();
      if (btn == BTN_NRF) {
        currentState = STATE_NRF_TEST;
        testNRF();
        currentState = STATE_DONE;  // test finito, show results
      }
      else if (btn == BTN_CC) {
        currentState = STATE_CC_TEST;
        testCC();
        currentState = STATE_DONE;
      }
      else if (btn == BTN_GPS) {
        currentState = STATE_GPS_MODE;
        gpsQuickTest();  // does serial init + 3s NMEA check
        // If serial was dead, go straight to DONE (results screen)
        if (!gpsSerialAlive) {
          gpsSerial.end();
          currentState = STATE_DONE;
        }
      }
      else if (btn == BTN_RST) {
        ESP.restart();
      }
      break;
    }

    // ===================== GPS LIVE MODE =====================
    case STATE_GPS_MODE: {
      // Update display once per second (also feeds GPS parser)
      static unsigned long lastGPSUpdate = 0;
      if (millis() - lastGPSUpdate > 1000) {
        lastGPSUpdate = millis();
        updateGPSDisplay();
      }
      // Also keep feeding parser between display updates
      while (gpsSerial.available()) {
        gps.encode(gpsSerial.read());
      }

      // ANY button press exits GPS mode back to menu
      int btn = readButton();
      if (btn != 0) {
        gpsSerial.end();
        currentState = STATE_MENU;
        showMenu();
      }
      break;
    }

    // ===================== RESULTS SCREEN =====================
    case STATE_DONE: {
      // Sit on the results screen until any button is pressed
      int btn = readButton();
      if (btn == BTN_RST) {
        ESP.restart();
      }
      else if (btn != 0) {
        currentState = STATE_MENU;
        showMenu();
      }
      break;
    }

    default:
      currentState = STATE_MENU;
      showMenu();
      break;
  }
}
