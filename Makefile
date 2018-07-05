CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
CFLAGS=-O0 -W -Wall -Wno-deprecated-declaration -Wno-unused-parameter -Wundef\
-ffunction-sections -mcpu=cortex-m4 -fno-exceptions -mfloat-abi=hard -mthumb \
-fno-omit-frame-pointer -Wextra -g

LD_SCRIPT=ld.lds

SRC_DIR=src

SRC=$(addprefix src/,main.c interrupts.c gpios.c entry.c flash.c uart.c)
OBJS=$(SRC:.c=.o)
ELF=startup.elf
BIN=startup.bin
PAYLOAD_NAME=payload

.PHONY: all clean flash gdb

all: startup payload

startup:
	$(CC) $(CFLAGS) -c $(SRC)
	mv *.o $(SRC_DIR)
	$(LD) -T $(LD_SCRIPT) $(OBJS) -o $(ELF)
	$(OBJCOPY) -O binary -S $(ELF) $(BIN)

payload: ELF=$(PAYLOAD_NAME).elf
payload: BIN=$(PAYLOAD_NAME).bin
payload: SRC=$(addprefix src/,blink.c interrupts.c gpios.c)
payload: OBJS=$(SRC:.c=.o)
payload:
#	$(CC) $(CFLAGS) $(SRC) -o $(BIN)
	$(CC) $(CFLAGS) -c $(SRC)
	mv *.o $(SRC_DIR)
	$(LD) -T $(LD_SCRIPT) $(OBJS) -o $(ELF)
	$(OBJCOPY) -O binary -S $(ELF) --only-section='.text.*' $(BIN)

flash:
	st-flash --format binary write $(BIN) 0x08000000

flash_payload: BIN=$(PAYLOAD_NAME).bin
flash_payload:
	st-flash --format binary write $(BIN) 0x08004000

gdb:
	arm-none-eabi-gdb startup.elf

clean:
	$(RM) $(BIN) $(ELF) $(OBJS) $(PAYLOAD_NAME).{bin,elf} *~ *.swp
