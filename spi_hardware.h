/*Módulo de vinculación del hardware spi.
  Permite elegir el CS con el mux.
  Define una función de lectura de entradas y otra de escritura de salidas. Ambas manejan un byte
*/
#pragma once

#include <SPI.h>
#include <Arduino.h>

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

extern SPIClass *vspi;

void spi_init();
void spi_mux_set(byte mux);
uint8_t spi_transf_digitalInput(SPIClass *spi);
void spi_transf_digitalOutput(SPIClass *spi, uint8_t tx);