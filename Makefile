CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
CFLAGS=-Os -W -Wall -Wno-deprecated-declaration -Wno-unused-parameter -Wundef\
-ffunction-sections -mcpu=cortex-m4 -fno-exceptions -g3 -mfloat-abi=hard -mthumb \
-fno-omit-frame-pointer

LD_SCRIPT=ld.lds

SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
ELF=startup.elf
BIN=startup.bin

all:
	$(CC) -c $(SRC)
	$(LD) -T $(LD_SCRIPT) $(OBJS) -o $(ELF)

copy:
	$(OBJCOPY) -O binary -S $(ELF) $(BIN)

clean:
	$(RM) $(BIN) $(ELF) *.o *~ *.swp
