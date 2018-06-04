CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
CFLAGS=-O0 -W -Wall -Wno-deprecated-declaration -Wno-unused-parameter -Wundef\
-ffunction-sections -mcpu=cortex-m4 -fno-exceptions -g -mfloat-abi=hard -mthumb \
-fno-omit-frame-pointer -Wextra

LD_SCRIPT=ld.lds

SRC_DIR=src

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(SRC:.c=.o)
ELF=startup.elf
BIN=startup.bin

.PHONY: all clean

all:
	$(CC) $(CFLAGS) -c $(SRC)
	mv *.o $(SRC_DIR)
	$(LD) -T $(LD_SCRIPT) $(OBJS) -o $(ELF)
	$(OBJCOPY) -O binary -S $(ELF) $(BIN)

clean:
	$(RM) $(BIN) $(ELF) $(OBJS) *~ *.swp
