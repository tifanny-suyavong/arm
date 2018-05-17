# ARM

Something for ARM STM32F429 microcontroller.

Use `arm-none-eabi-objdump -D linker` to check the generated data.

## Instructions
### Compilation
Use `make` to compile.

Use `make copy` to generate `startup.bin`.

### Flashing
`st-flash --format binary write startup.bin 0x08000000`

### Debugging
``` bash
st-util # start GDB server
arm-none-eabi-gdb # in gdb use `target remote 0.0.0.0:4242` to connect to the GDB server
```

## Useful links

* [STM32F4 Reference Manual](http://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf)
* [STM32F429 Discovery Kit](http://www.st.com/content/ccc/resource/technical/document/user_manual/6b/25/05/23/a9/45/4d/6a/DM00093903.pdf/files/DM00093903.pdf/jcr:content/translations/en.DM00093903.pdf)

## Authors

* Tifanny Suyavong - <suyavo_t@epita.fr>
* Guillaume Taquet-Gasperini - <taquet_g@epita.fr>
* Grégoire Verdier - <verdie_g@epita.fr>
