// Minimal, and probably therefore incomplete, initialisation of LPUART1
// so that the NUCLEO-L433RC-P virtual COM port can be used for stdio.

#include <stm32l4xx_ll_bus.h>
#include <stm32l4xx_ll_gpio.h>
#include <stm32l4xx_ll_lpuart.h>

void stdio_uart_init(void) {
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    // LPUART1 TX PA2
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_8);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
    // LPUART1 RX PA3
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_8);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_NO);  // Connected to the ST-LINK/V2 MCU so pull not required

    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1);

    LL_LPUART_Disable(LPUART1);
    LL_LPUART_SetTransferDirection(LPUART1, LL_LPUART_DIRECTION_TX_RX);
    LL_LPUART_ConfigCharacter(LPUART1, LL_LPUART_DATAWIDTH_8B, LL_LPUART_PARITY_NONE, LL_LPUART_STOPBITS_1);
    LL_LPUART_SetHWFlowCtrl(LPUART1, LL_LPUART_HWCONTROL_NONE);

    const uint32_t periphclk = 80000000;
    LL_LPUART_SetBaudRate(LPUART1, periphclk, 115200);

    LL_LPUART_Enable(LPUART1);
}
