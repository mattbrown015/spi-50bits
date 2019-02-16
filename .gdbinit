# By default the STM32L4 on the NUCLEO-L433RC-P dev board is connected to the ST-LINK/V2 in-circuit debugger.
# Hence it is easier to use OpenOCD as the gdb server, rather than JLinkGDBServer.
# To run OpenOCD cd to its installation folder and start it using:
#  bin\openocd.exe  --file scripts\interface\stlink.cfg --file scripts\board\st_nucleo_l4.cfg
#
# Download GNU MCU Eclipse OpenOCD from https://gnu-mcu-eclipse.github.io/openocd/. At the time of writing
# I downloaded 'gnu-mcu-eclipse-openocd-0.10.0-11-20190118-1134-win64.zip' and installed it by unzipping
# into "C:\Program Files".
# By default this configuration is listening on 2331 as opposed to 3333 used by JLinkGDBServer.
#
# To debug using J-Link start the gdb server using either:
#  "C:\Program Files (x86)\SEGGER\JLink_V640\JLinkGDBServer.exe" -device STM32L442KC -if SWD -speed 4000
# or:
#  "C:\Program Files (x86)\SEGGER\JLink_V640\JLinkGDBServerCL.exe" -device STM32L442KC -if SWD -speed 4000
#
# Start gdb using the following to so that this .gdbinit will be run:
#  "C:\Program Files (x86)\GNU Tools ARM Embedded\8 2018-q4-major\bin\arm-none-eabi-gdb.exe" -iex "add-auto-load-safe-path .gdbinit" BUILD\PENGUIN\GCC_ARM\temperature-sensor_application.elf

target remote localhost:3333
