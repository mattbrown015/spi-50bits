ENTRY(reset_handler)

_estack = 0x2000C000;    /* end of SRAM1 */

MEMORY
{
    SRAM1 (xrw) : ORIGIN = 0x20000000, LENGTH = 48K
    SRAM2 (xrw) : ORIGIN = 0x10000000, LENGTH = 16K
    FLASH (rx) : ORIGIN = 0x8000000, LENGTH = 256K
}

SECTIONS
{
    .interrupt_vectors :
    {
        . = ALIGN(8);
        KEEP(interrupt-vectors.o(.interrupt_vectors))
        . = ALIGN(8);
    } > FLASH

    .text :
    {
        . = ALIGN(8);
        reset-handler.o(.text.reset_handler)
        interrupt-vectors.o(.text.default_handler)
        *(.text)
        . = ALIGN(8);
        _etext = .;
    } > FLASH

    /* used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    .data : 
    {
        . = ALIGN(8);
        _sdata = .;
        *(.data)
        . = ALIGN(8);
        _edata = .;
    } > SRAM1 AT > FLASH

    . = ALIGN(4);
    .bss :
    {
        _sbss = .;
        *(.bss)
        . = ALIGN(4);
        _ebss = .;
    } > SRAM1
}
