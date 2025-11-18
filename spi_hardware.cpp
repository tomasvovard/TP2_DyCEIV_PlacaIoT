#include "spi_hardware.h"

SPIClass *vspi = nullptr;

void spi_init(){
  /*Inicialización SPI*/
  vspi = new SPIClass(VSPI);
  vspi->begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_SS_PIN);

  /*Inicialización pines mux*/
  pinMode(CS_A0_PIN, OUTPUT);
  pinMode(CS_A1_PIN, OUTPUT);
  pinMode(CS_A2_PIN, OUTPUT);
  spi_mux_set(CS_NONE);  
}

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
    CPOL = 1
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
  spi->beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE2));
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