.syntax unified
.cpu cortex-m4
.thumb

.section .text.reset_handler
.global reset_handler
.type reset_handler, %function
reset_handler:
    ldr sp, =_estack

    movs  r1, #0
    b loop_copy_data_init

copy_data_init:
    ldr r3, =_sidata
    ldr r3, [r3, r1]
    str r3, [r0, r1]
    adds r1, r1, #4

loop_copy_data_init:
    ldr r0, =_sdata
    ldr r3, =_edata
    adds r2, r0, r1
    cmp r2, r3
    bcc copy_data_init
    ldr r2, =_sbss
    b loop_fill_zero_bss

fill_zero_bss:
    movs r3, #0
    str r3, [r2], #4

loop_fill_zero_bss:
    ldr r3, = _ebss
    cmp r2, r3
    bcc fill_zero_bss

    bl main

infinite_loop:
    b infinite_loop
