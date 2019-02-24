ENTRY(reset_handler)

_estack = 0x2000C000;    /* end of SRAM1 */
/* Generate a link error if heap and stack don't fit into RAM */
_min_heap_size = 0x200;  /* required amount of heap */
_min_stack_size = 0x400; /* required amount of stack */

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
        KEEP(*(.interrupt_vectors))
        . = ALIGN(8);
    } > FLASH

    .text :
    {
        . = ALIGN(8);
        *(.text.reset_handler)
        *(.text.default_handler)
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)
        . = ALIGN(8);
        _etext = .;
    } > FLASH

    .rodata :
    {
        . = ALIGN(8);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(8);
    } > FLASH

    .ARM.extab :
    { 
        . = ALIGN(8);
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(8);
    } > FLASH

    .ARM :
    {
        . = ALIGN(8);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(8);
    } > FLASH

    .preinit_array :
    {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(8);
    } > FLASH

    .init_array :
    {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(8);
    } > FLASH

    .fini_array :
    {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(8);
    } > FLASH

    /* used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    .data : 
    {
        . = ALIGN(8);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(8);
        _edata = .;
    } > SRAM1 AT > FLASH

    . = ALIGN(4);
    .bss :
    {
        _sbss = .;
         __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } > SRAM1

    /* user_heap_stack section, used to check that there is enough SRAM1 left */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE (end = .);
        PROVIDE (_end = .);
        . = . + _min_heap_size;
        . = . + _min_stack_size;
        . = ALIGN(8);
    } > SRAM1

    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libc_nano.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 :
    {
        *(.ARM.attributes)
    }
}
