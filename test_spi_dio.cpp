#include "test_spi_dio.h"
#include "spi_hardware.h"

void test_printHex8(uint8_t val) {
    Serial.print("0x");
    if (val < 0x10) Serial.print('0');
    Serial.print(val, HEX);
}

void test_printBin8(uint8_t val){
  Serial.print("0b");
  for (int i = 7; i >= 0; --i) {
    Serial.print((val >> i) & 0x01);
  }
}

void test_digitalIO(uint8_t outputs){

  /*Escribo las salidas*/
  spi_transf_digitalOutput(vspi, outputs);

  /*Leo las entradas*/
  uint8_t inputs = spi_transf_digitalInput(vspi);

  /*Muestro resultados*/
  Serial.print("Salidas = ");
  test_printHex8(outputs);
  Serial.print(" = ");
  test_printBin8(outputs);

  Serial.print(" | Entradas = ");
  test_printHex8(inputs);
  Serial.print(" = ");
  test_printBin8(inputs);

  Serial.println();

}

void test_sequenceDigitalIO(){

  static uint8_t sal = 0x01;
  test_digitalIO(sal);
  sal <<= 1;
  if(sal==0) sal=0x01;
  delay(500);
}