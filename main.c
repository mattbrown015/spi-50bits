#include <stm32l4xx_ll_bus.h>
#include <stm32l4xx_ll_gpio.h>
#include <stm32l4xx_ll_spi.h>

#include <stdio.h>
#include <string.h>

// With data frame data size set to 10-bits the buffer format is:
// | hword |                 0                 |                 1                 |                 2                 |                 3                 |                 4                 |                 5                 |                 6                 |
// | byte  |         0       |         1       |         2       |         3       |         4       |         5       |         6       |         7       |         8       |         9       |         a       |         b       |         c       |         d       |
// | bit   | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 | 0 1 2 3 4 5 6 7 |
// | sent  | - - - - - - - - | - - - x x x x x | - - - - - - - - | - - - x x x x x | - - - - - - - - | - - - x x x x x | - - - - - - - - | - - - x x x x x | - - - - - - - - | - - - x x x x x | - - - - - - - - | - - - x x x x x | - - - - - - - - | - - - x x x x x |
// The 4 MSBs are written as 1s to the TXFIFO but they are not seen in the SPI transaction.
uint16_t tx_buffer[6] = { 0xfc55, 0xfc55, 0xfc55, 0xfc55, 0xfc55, 0xfc55 };
#define TX_BUFFER_LENGTH (sizeof(tx_buffer) / sizeof(tx_buffer[0]))

int main(void) {
    puts("spi-50bits " __DATE__ " " __TIME__);

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);  // GPIOA will already be enabled because of stdio UART is initialised
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

    // MOSI PA12
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_12, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_12, LL_GPIO_OUTPUT_PUSHPULL);
    // MISO PA6
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_6, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);  // Pull down as I don't have a slave connected
    // SCLK PA5
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL);
    // NSS PA4
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_4, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_4, LL_GPIO_OUTPUT_OPENDRAIN);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_4, LL_GPIO_PULL_UP);

    LL_SPI_Disable(SPI1);
    LL_SPI_SetTransferDirection(SPI1, LL_SPI_FULL_DUPLEX);
    LL_SPI_SetMode(SPI1, LL_SPI_MODE_MASTER);
    LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_10BIT);
    LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_LOW);
    LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_1EDGE);
    LL_SPI_SetNSSMode(SPI1, LL_SPI_NSS_HARD_OUTPUT);
    // SPI1 is on the APB2 bus and its clock is PCLK2 which is derived from SYSCLK.
    // There are various PCLK2 prescalers but I believe they all default to 1
    // hence PCLK2 is SYSCLK which 80 MHz.
    // So this divider is dividing 80 MHz and /32 gives a 2.5 MHz SCLK.
    LL_SPI_SetBaudRatePrescaler(SPI1, LL_SPI_BAUDRATEPRESCALER_DIV32);
    LL_SPI_SetTransferBitOrder(SPI1, LL_SPI_MSB_FIRST);
    LL_SPI_DisableNSSPulseMgt(SPI1);

    while (1) {
        if(!LL_SPI_IsEnabled(SPI1))
        {
            LL_SPI_Enable(SPI1);
        }

        for (int i = 0; i < TX_BUFFER_LENGTH; ++i) {
            while (!LL_SPI_IsActiveFlag_TXE(SPI1));
            // Send bytes over the SPI
            LL_SPI_TransmitData16(SPI1, tx_buffer[i]);
            // Wait until the transmission is complete
            while (LL_SPI_IsActiveFlag_BSY(SPI1));
        }

        LL_SPI_Disable(SPI1);

        volatile int count = 0xff;
        while (count > 0) --count;
    }
}
