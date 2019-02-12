#include <stm32l4xx_hal.h>

#include <stm32l4xx_ll_pwr.h>
#include <stm32l4xx_ll_rcc.h>
#include <stm32l4xx_ll_system.h>

// Some of the examples like to do the clock initialization
// from the reset handler before calling main. I'm not sure
// what value it adds except ensuring it really is the first
// thing to be done. I did think the SYSCLK could be set
// before doing the C data initialization and then that would
// go faster.
static void sysclk_init(void) {
    // MSI will be enabled and running at 4 MHz after reset as it is the default clock src.
    while (!LL_RCC_MSI_IsReady());

    // The LSE is used to /calibrate/ the MSI via the MSI PLL and improve MSI output accuracy
    // i.e. it will be a /more accurate/ 4 MHz.
    // LSE must be ready before enabling the MSI clock PLL.
    // LSEON is in backup power domain so this is only necessary after a POR.
    if (!LL_RCC_LSE_IsReady()) {
        __HAL_RCC_PWR_CLK_ENABLE();

        LL_PWR_EnableBkUpAccess();
        while (!LL_PWR_IsEnabledBkUpAccess());

        // Low xtal drive is the default.
        LL_RCC_LSE_Enable();
        while (!LL_RCC_LSE_IsReady());

        LL_PWR_DisableBkUpAccess();
        __HAL_RCC_PWR_CLK_DISABLE();
    }

    LL_RCC_MSI_EnablePLLMode();
    while (!LL_RCC_MSI_IsReady());

    // The main PLL is used to multiple the MSI's output.
    // PLL must be disabled when changing its config.
    while (READ_BIT(RCC->CR, RCC_CR_PLLON) != 0);

    // Configure the PLL to generate the 80 MHz.
    // The PLL's input is 4 MHz. PLLM divisor is 1 so VCO input frequency is 4 MHz.
    // PLLN, the VCO multiplier is 40 so VCO output is 160 MHz.
    // PLLR, the PLL divisor for R PLLR output, is 2, because it is the smallest divisor,
    // so the PLLR output is (4 * 40) / 2 = 80 MHz.
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);

    // PLL is configured so enable it and wait for it to lock.
    LL_RCC_PLL_Enable();
    while(!LL_RCC_PLL_IsReady());

    // Enable the PLLR output, also known as 'domain SYS' because it can be selected as the SYSCLK source.
    LL_RCC_PLL_EnableDomain_SYS();

    // Change the flash latency just before switching SYSCLK to 80 MHz.
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);

    // Change SYSCLK source to PLL rather than MSI.
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
}

int main(void) {
    sysclk_init();

    while (1) {
    }
}
