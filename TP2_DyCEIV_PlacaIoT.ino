#include "spi_hardware.h"
#include "test_spi_dio.h"

void setup(){
  Serial.begin(115200);
  spi_init();
}

void loop() {
  test_sequenceDigitalIO();
}
