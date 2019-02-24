ARM_GCC_PATH_ROOT=C:/Program Files (x86)/GNU Tools ARM Embedded/8 2018-q4-major
ARM_GCC_PATH=$(ARM_GCC_PATH_ROOT)/bin

STM32_CUBE_ROOT=C:/Users/matthewb/Documents/work/sandbox/stm32l4/STM32Cube_FW_L4_V1.13.0
STM32l4_HAL_DRIVER=$(STM32_CUBE_ROOT)/Drivers/STM32L4xx_HAL_Driver
CMSIS_DRIVER=$(STM32_CUBE_ROOT)/Drivers/CMSIS
CMSIS_DEVICE=$(CMSIS_DRIVER)/Device/ST/STM32L4xx

AS=$(ARM_GCC_PATH)/arm-none-eabi-gcc.exe
CC=$(ARM_GCC_PATH)/arm-none-eabi-gcc.exe
LD=$(ARM_GCC_PATH)/arm-none-eabi-gcc.exe

VPATH=$(STM32l4_HAL_DRIVER)/Src

SRCS=main.c syscalls.c stdio-uart-init.c sysclk-init.c reset-handler.s interrupt-vectors.s
OBJS=$(SRCS:.c=.o)
OBJS:=$(OBJS:.s=.o)

STM32L433_DIR=stm32l433
OUTPUT_DIR=$(STM32L433_DIR)
OBJS:=$(addprefix $(OUTPUT_DIR)/, $(OBJS))

TARGET=$(OUTPUT_DIR)/spi-50bits.elf

INCLUDE_PATH=-I. -I$(STM32l4_HAL_DRIVER)/inc -I$(CMSIS_DRIVER)/Include -I$(CMSIS_DEVICE)/Include

ARCH_FLAGS=-mcpu=cortex-m4 -mthumb
HAL_MACROS=-DSTM32L433xx
CFLAGS=-Ofast -g3 -Wall -Wpedantic $(ARCH_FLAGS) $(HAL_MACROS) $(INCLUDE_PATH)

all: prep $(TARGET)

$(TARGET): 256kflash-48kram.ld $(OBJS)
	$(LD) $(ARCH_FLAGS) -Wl,--script=$< -Wl,-Map=$(basename $@).map $(OBJS) -lc_nano -lnosys -o $@

$(OUTPUT_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(OUTPUT_DIR)/%.o: %.s
	$(AS) -c $(ARCH_FLAGS) -o $@ $<

.PHONY: prep
prep:
	@if not exist $(OUTPUT_DIR) mkdir $(OUTPUT_DIR)

.PHONY: clean
clean:
	del *.elf "$(OUTPUT_DIR)/*.o" *.map
