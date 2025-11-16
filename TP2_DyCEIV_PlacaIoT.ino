#include <SPI.h>

// Definiciones de pines del bus VSPI
#define SPI_SCK_PIN  18
#define SPI_MISO_PIN 19
#define SPI_MOSI_PIN 23
#define SPI_SS_PIN    5

#define CS_A0 5
#define CS_A1 15
#define CS_A2 13

SPIClass *vspi = nullptr;

hw_timer_t* timer = nullptr;
volatile bool timerReady = false;

void IRAM_ATTR onTimer() {
  timerReady = true;
}

void spi_mux_set(byte mux){

  digitalWrite(CS_A0, 0b00000001 & mux);
  digitalWrite(CS_A1, 0b00000010 & mux);
  digitalWrite(CS_A2, 0b00000100 & mux);

}

uint8_t spi_trasnf_digitalInput(SPIClass *spi, int spiCLK){

  static const int spiClk = 1000000;  // 1 MHz
  spi_mux_set(0x00);
  delayMicroseconds(2000000UL / spiClk); 
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE3));
  //digitalWrite(spi->pinSS(), LOW);  //pull SS slow to prep other end for transfer
  spi_mux_set(0b00000101);
  uint8_t rx = spi->transfer(0x00);
  spi_mux_set(7);
  //digitalWrite(spi->pinSS(), HIGH);  //pull ss high to signify end of data transfer
  spi->endTransaction();
  return rx;
}

uint8_t spi_trasnf_digitalOutput(SPIClass *spi, int spiCLK, uint8_t tx){

  static const int spiClk = 1000000;  // 1 MHz
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE3));
  spi_mux_set(0b00000001);
  spi->transfer(tx);
  spi_mux_set(7);
  spi->endTransaction();
  return rx;
}

void setup() {

  Serial.begin(115200);
  vspi = new SPIClass(VSPI);
  vspi->begin();

  pinMode(CS_A0, OUTPUT);
  pinMode(CS_A1, OUTPUT);
  pinMode(CS_A2, OUTPUT);

  spi_mux_set(7);

  timer = timerBegin(0, 80, true);      
  // timerBegin(timer_num, prescaler, countUp)
  // prescaler=80 → 1 tick = 1 µs  (80 MHz / 80 = 1 MHz)

  timerAttachInterrupt(timer, &onTimer, true);

  
}

void loop() {

  spi_mux_set(7);
  delay(10);
  spi_mux_set(0b00000001);
  delay(10);
}
