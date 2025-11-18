#include <SPI.h>
#include "soc/gpio_reg.h"

#define SPI_SCK_PIN  18
#define SPI_MISO_PIN 19
#define SPI_MOSI_PIN 23
#define SPI_SS_PIN    5

#define SPI_CLK 1000000 // 1 MHz

#define CS_A0_PIN 5
#define CS_A1_PIN 15
#define CS_A2_PIN 13
#define CS_A0_MASK 0b00000001 //001
#define CS_A1_MASK 0b00000010 //010
#define CS_A2_MASK 0b00000100 //100

#define CS_IN_LD      0x00 //000
#define CS_IN_CE      0x05 //101
#define CS_OUT_LATCH  0x01 //001
#define CS_ADC_SAR    0x03 //011
#define CS_ADC_SD     0x02 //010
#define CS_DAC_SYNC   0x04 //100
#define CS_NONE       0x07 //111

SPIClass *vspi = nullptr;

void spi_mux_set(byte mux){

  /*Máscara con los pines de Ao..A2*/
  const uint32_t mask_all = (1u << CS_A0_PIN) | (1u << CS_A1_PIN) | (1u << CS_A2_PIN);

  /*Pines a setear*/
  uint32_t mask_set = 0;
  if(CS_A0_MASK & mux) mask_set |= (1u << CS_A0_PIN);
  if(CS_A1_MASK & mux) mask_set |= (1u << CS_A1_PIN);
  if(CS_A2_MASK & mux) mask_set |= (1u << CS_A2_PIN);

  /*Pines a resetear*/
  uint32_t mask_clear = mask_all & ~mask_set;

  /*Actualizar pines*/
  if(mask_clear) REG_WRITE(GPIO_OUT_W1TC_REG, mask_clear); //sólo se ejecuta cuando hay pines a resetear.
  if(mask_set) REG_WRITE(GPIO_OUT_W1TS_REG, mask_set); //sólo se ejecuta cuando hay pines a setear.
}

uint8_t spi_transf_digitalInput(SPIClass *spi){
  /* SN65HVS882
  Modo:
    CPHA = 0
    CPOL = 0
    MSB first
  Temporización:
    FCLK < 100MHZ
    tW2 (ancho de pulso de LD) > 6ns
    tREC (tiempo de recup de LD a CLK) > 2ns
    tSU2 (setup de CE a CLK) > 4ns
    En principio las llamadas a funciones serían más lentas que los tiempos requeridos por lo que no sería necesario un delay para garantizar LD
  */

  /*Carga del registro*/
  spi_mux_set(CS_IN_LD);
  spi_mux_set(CS_NONE);

  /*Comunicación serie*/
  spi->beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE0));
  spi_mux_set(CS_IN_CE); //cs en bajo
  uint8_t rx = spi->transfer(0x00);
  spi_mux_set(CS_NONE); //cs en alto 
  spi->endTransaction(); 

  return rx;
}

void spi_transf_digitalOutput(SPIClass *spi, uint8_t tx){
  /* DRV8804
  Modo:
    CPHA = 0
    CPOL = 0
    MSB first
  Temporización:
    FCLK < 16,13MHz (TCLK > 62ns)
  */
  spi->beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE0));
  spi_mux_set(CS_OUT_LATCH);
  spi->transfer(tx);
  spi_mux_set(CS_NONE);
  spi->endTransaction();
}

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

void setup() {

  Serial.begin(115200);

  /*Inicialización SPI*/
  vspi = new SPIClass(VSPI);
  vspi->begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_SS_PIN);

  /*Inicialización pines mux*/
  pinMode(CS_A0_PIN, OUTPUT);
  pinMode(CS_A1_PIN, OUTPUT);
  pinMode(CS_A2_PIN, OUTPUT);
  spi_mux_set(CS_NONE);  
}

void loop() {

  static uint8_t sal = 0x01;
  test_digitalIO(sal);
  sal <<= 1;
  if(sal==0) sal=0x01;
  delay(500);
}
