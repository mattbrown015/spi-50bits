# To debug using J-Link start the gdb server using either:
#  "C:\Program Files (x86)\SEGGER\JLink_V640\JLinkGDBServer.exe" -device STM32L442KC -if SWD -speed 4000
# or:
#  "C:\Program Files (x86)\SEGGER\JLink_V640\JLinkGDBServerCL.exe" -device STM32L442KC -if SWD -speed 4000
#
# Start gdb using the following to so that this .gdbinit will be run:
#  "C:\Program Files (x86)\GNU Tools ARM Embedded\7 2017-q4-major\bin\arm-none-eabi-gdb.exe" -iex "add-auto-load-safe-path .gdbinit" spi-50bits.elf

target remote localhost:2331
