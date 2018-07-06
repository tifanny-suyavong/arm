# ARM

Something for ARM STM32F429 microcontroller.

Use `arm-none-eabi-objdump -D linker` to check the generated data.

## Instructions
### Compilation

Use `make` to compile.

### Flashing
`st-flash --format binary write startup.bin 0x08000000`

### Debugging
``` bash
st-util # start GDB server in another terminal
make gdb # start gdb, connect, flash and break on main
```

## Useful links

* [STM32 Primer - Use the Standard Peripheral Library](http://pandafruits.com/stm32_primer/stm32_primer_lib.php)
* [STM32F4 Reference Manual](http://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf)
* [STM32F429 Discovery Kit](http://www.st.com/content/ccc/resource/technical/document/user_manual/6b/25/05/23/a9/45/4d/6a/DM00093903.pdf/files/DM00093903.pdf/jcr:content/translations/en.DM00093903.pdf)
* [STM32F429 Data Sheet](http://www.st.com/content/ccc/resource/technical/document/datasheet/03/b4/b2/36/4c/72/49/29/DM00071990.pdf/files/DM00071990.pdf/jcr:content/translations/en.DM00071990.pdf)

## Authors

* Tifanny Suyavong - <suyavo_t@epita.fr>
* Guillaume Taquet-Gasperini - <taquet_g@epita.fr>
* Grégoire Verdier - <verdie_g@epita.fr>


## UART (misc)

Plusieurs modules UART (6 ou 7). Choisir un UART. Savoir sur quelle pin utiliser l'UART.
Dans la datasheet y'a un tableau de fonction alternative (gros tableau).

1) config IO
2) config module UART

UART 2 pins (RX, TX).
conf module UART (param clock, baudrate)

On choisir AF7 avec PB6/PB7 pour l'USART1_TX/RX

pour le baudrate, faut pas qu'il soit fixe, il doit etre set sur le diviseur de
la clock. Lire chapitre RCC sur les clocks.

STM32F4xx                   STM32F429-Discovery
MAX CLOCK in MHz            180
APB1 CLOCK in MHz           45
APB2 CLOCK in MHz           90
DEFAULT PLL INPUT CLOCK     HSE 8MHz
PLL_M                       8
PLL_N                       360
PLL_P                       2

Blanc : RX

Vert : TX


# HEY

Pour envoyer payload.bin :

``` bash
sudo stty -F /dev/ttyUSB0 115200 cs8 -cstopb -parenb -echo -cooked -parenb min 1 time 1
cat payload.bin > /dev/ttyUSB0
```

```
