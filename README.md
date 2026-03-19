Exactly the same as my old tester just a V2 that adds GPS as well for the esp32

// -------------------- BUTTONS --------------------
BTN_NRF   32
BTN_CC    25
BTN_RST   33
BTN_GPS   26

// -------------------- NRF24 --------------------
NRF_CE 4
NRF_CS 5
RF24 radio(NRF_CE, NRF_CS);

// -------------------- CC1101 --------------------
CC_CS 17

// -------------------- SPI shared by both radios -----
SPI_SCK  18
SPI_MISO 19
SPI_MOSI 23

// -------------------- GPS --------------------
#define GPS_RX_PIN 13
#define GPS_TX_PIN 27
#define GPS_BAUD 9600
