#include "gpios.h"
#include "flash.h"

void unlock(void)
{
    volatile int *flash_keyr = (volatile int*)FLASH_KEYR;

    *flash_keyr = FLASH_KEY1;
    *flash_keyr = FLASH_KEY2;
}

void lock(void)
{
    volatile int *flash_cr = (volatile int*)FLASH_CR;
    set_bit(flash_cr, FLASH_CR_LOCK_I);
}

void erase_sector(int sector)
{
    volatile int *flash_sr = (volatile int*)FLASH_SR;
    volatile int *flash_cr = (volatile int*)FLASH_CR;

    while ((*flash_sr & (1 << FLASH_SR_BSY_I)))
        continue;

    // Set SER to
    SET_BIT(flash_cr, 1);
    // Set SND to sector value
    *flash_cr = (*flash_cr & ~(31 << 3)) | (sector << 3);
    // Set STRT
    SET_BIT(flash_cr, 16);

    while ((*flash_sr & (1 << FLASH_SR_BSY_I)))
      continue;
}

void write_to_flash(const char *buf, long size, char *addr)
{
  volatile int *flash_sr = (volatile int*)FLASH_SR;
  volatile int *flash_cr = (volatile int*)FLASH_CR;

  for (long i = 0; i < size; ++i)
  {
      // Check BSY bit in FLASH_SR
      while ((*flash_sr & (1 << FLASH_SR_BSY_I)))
          continue;

    // Set PG in FLASH_CR
    SET_BIT(flash_cr, FLASH_CR_PG_I);

    addr[i] = buf[i];
  }

  while (*flash_sr & (1 << FLASH_SR_BSY_I))
      continue;
}
