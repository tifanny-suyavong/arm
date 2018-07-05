#ifndef _FLASH_H
# define _FLASH_H

#define FLASH_BASE      0x40023C00

#define FLASH_KEYR      (FLASH_BASE + 0x04)
#define FLASH_SR        (FLASH_BASE + 0x0C)
#define FLASH_CR        (FLASH_BASE + 0x10)

#define FLASH_SR_BSY_I  16
#define FLASH_CR_PG_I   0
#define FLASH_CR_LOCK_I 31

#define FLASH_KEY1      0x45670123
#define FLASH_KEY2      0xCDEF89AB

void unlock(void);

void lock(void);

void erase_sector(int sector);

void write_to_flash(const char *buf, long size, char *addr);

#endif
