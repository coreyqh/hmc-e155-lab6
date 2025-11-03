// DS1722.h
// Corey Hickson
// chickson@hmc.edu
// 10/29/2025
// Header for SPI drivers

#ifndef DS1722_H
#define DS1722_H

#include <stdint.h>

/*
typedef struct
{
  __IO uint32_t CR1;         // SPI Control register 1,                              Address offset: 0x00 
  __IO uint32_t CR2;         // SPI Control register 2,                              Address offset: 0x04 
  __IO uint32_t SR;          // SPI Status register,                                 Address offset: 0x08 
  __IO uint32_t DR;          // SPI data register,                                   Address offset: 0x0C 
  __IO uint32_t CRCPR;       // SPI CRC polynomial register,                         Address offset: 0x10 
  __IO uint32_t RXCRCR;      // SPI Rx CRC register,                                 Address offset: 0x14 
  __IO uint32_t TXCRCR;      // SPI Tx CRC register,                                 Address offset: 0x18 
} SPI_TypeDef;

*/

#define CS_PIN   PB1
#define MOSI_PIN PB5
#define MISO_PIN PB4
#define SCK_PIN  PB3

#define CONFIG_WADDR 0x80
#define CONFIG_RADDR 0x00
#define TMPMSB_RADDR 0x02
#define TMPLSB_RADDR 0x01

#define RESOLUTION_8  0xE0
#define RESOLUTION_9  0xE2
#define RESOLUTION_10 0xE4
#define RESOLUTION_11 0xE6
#define RESOLUTION_12 0xE8


void     spiInit(uint32_t brc, uint32_t cpol, uint32_t cpha /* other params???*/);
uint8_t  spiSendReceive(uint8_t send);
uint16_t spiSendReceive16(uint16_t data);
void     spiWrite(uint8_t address, uint8_t data);
uint8_t  spiRead(uint8_t address);

#endif // DS1722_H
