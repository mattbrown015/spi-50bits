// Minimal, and probably therefore incomplete, implementation of syscalls
// required by newlib, and weakly defined in libnosys.a, so that the
// NUCLEO-L433RC-P virtual COM port can be used for stdio.

#include <stm32l4xx_ll_lpuart.h>

int _read(int file, char *ptr, int len) {
    // Assuming _read won't be called with 'ptr == 0' or 'len == 0'.
    while ((LPUART1->ISR & LL_LPUART_ISR_RXNE) != LL_LPUART_ISR_RXNE);
    *ptr = LL_LPUART_ReceiveData8(LPUART1);
    return 1;
}

int _write(int file, const char *ptr, const int len) {
    for (int i = 0; i < len; ++i) {
        while ((LPUART1->ISR & LL_LPUART_ISR_TXE) != LL_LPUART_ISR_TXE);
        LL_LPUART_TransmitData8(LPUART1, *ptr);
        ++ptr;
    }
    return len;
}
