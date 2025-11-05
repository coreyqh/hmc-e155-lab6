#include "stm32l4xx.h"  // CMSIS header for STM32L432KC

#define DS1722_CS_PORT GPIOA
#define DS1722_CS_PIN  GPIO_PIN_4

#define DS1722_READ_TEMP_CMD 0x01  // Read temperature command

// Helper macros to control CS pin
#define DS1722_CS_LOW()  (DS1722_CS_PORT->BSRR = (uint32_t)DS1722_CS_PIN << 16)
#define DS1722_CS_HIGH() (DS1722_CS_PORT->BSRR = DS1722_CS_PIN)

float DS1722_ReadTemperature(SPI_TypeDef *SPIx) {
    uint8_t tx_buf[3] = { DS1722_READ_TEMP_CMD, 0x00, 0x00 };
    uint8_t rx_buf[3] = { 0 };

    // Pull CS low to start transaction
    DS1722_CS_LOW();

    // Send command and receive 2 bytes
    for (int i = 0; i < 3; i++) {
        // Wait until TXE is set
        while (!(SPIx->SR & SPI_SR_TXE));
        *((__IO uint8_t *)&SPIx->DR) = tx_buf[i];

        // Wait until RXNE is set
        while (!(SPIx->SR & SPI_SR_RXNE));
        rx_buf[i] = *((__IO uint8_t *)&SPIx->DR);
    }

    // Pull CS high to end transaction
    DS1722_CS_HIGH();

    // Combine MSB and LSB (12-bit resolution)
    int16_t raw_temp = ((int16_t)rx_buf[1] << 8) | rx_buf[2];
    float temperature = raw_temp / 256.0f;

    return temperature;
}
