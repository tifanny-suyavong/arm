CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy

LD_SCRIPT=ld.lds

SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
BIN=linker

all:
	$(CC) -c $(SRC)
	$(LD) -T $(LD_SCRIPT) $(OBJS) -o $(BIN)

copy:
	$(OBJCOPY) -O binary $(BIN)

clean:
	$(RM) $(BIN) *.o *~ *.swp
