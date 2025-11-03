// DS1722.h
// Corey Hickson
// chickson@hmc.edu
// 10/29/2025
// SPI drivers

#include "STM32L432KC.h"
#include "DS1722.h"

void spiInit(uint32_t brc, uint32_t cpol, uint32_t cpha) {
    // configure GPIO pins for COPI, CIPO, SCK, CS
    pinMode(MISO_PIN, GPIO_ALT);
    pinMode(MOSI_PIN, GPIO_ALT);
    pinMode(SCK_PIN , GPIO_ALT);
    pinMode(CS_PIN  , GPIO_OUTPUT);

    GPIOB->AFR[0] &= ~((0xF << 12) | (0xF << 16) | (0xF << 20)); // using AFRL
    GPIOB->AFR[0] |=  ((5 << 12) | (5 << 16) | (5 << 20));

    // OSPEED for SPI
    GPIOB->OSPEEDR &= ~((3 << 6) | (3 << 8) | (3 << 10));
    GPIOB->OSPEEDR |=  ((3 << 6) | (3 << 8) | (3 << 10));

    // enable clock for SPI1
    RCC->APB2ENR |= _VAL2FLD(RCC_APB2ENR_SPI1EN, 1);


    // configure Baud Rate (BRC field)
    // clock div = 2 ^ (BRC + 1)
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, brc);
    
    // configure CPOL and CPHA (set CPOL & CPHA bits)
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);

    // set software CS handling
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SSM, 1);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SSI, 1);


    // configure as master (set MSTR bit)
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 1);

    // configure data size (8 bits, encoded as 0b0111)
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111);
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 1);

    // enable SPI (write to SPE bit)
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SPE, 1);


    spiWrite(0x80, 0xE0);
    int temp = spiRead(0x00);

}


uint8_t spiSendReceive(uint8_t data) {

    volatile uint8_t rd;

    while (!((SPI1->SR >> 1) & 1)); // wait until TX fifo has space
    *(volatile uint8_t * ) &SPI1->DR = data;                // write to data register
    while (!((SPI1->SR >> 0) & 1)); // wait until RX fifo has something
    rd = (volatile uint8_t) SPI1->DR;                  // read data register

    return rd;

}

uint16_t spiSendReceive16(uint16_t data) {

    uint16_t volatile rec = 0;
    uint8_t char1, char2;

    char1 = spiSendReceive((data & 0xFF00) >> 8);
    char2 = spiSendReceive(data & 0x00FF);
    rec = (char1 << 8) | char2;

    return rec; // Return received character

}

void spiWrite(uint8_t address, uint8_t data) {

    digitalWrite(CS_PIN, GPIO_HIGH); // turn on chip select
    spiSendReceive16(address << 8 | data);
    digitalWrite(CS_PIN, GPIO_LOW); // turn off chip select
    
}

uint8_t spiRead(uint8_t address) {
    
    digitalWrite(CS_PIN, GPIO_HIGH); // turn on chip select
    int rd = (uint8_t) (spiSendReceive16(address << 8 | (1 << 15)) & 0x00FF);
    digitalWrite(CS_PIN, GPIO_LOW); // turn off chip select
    return rd;

}

