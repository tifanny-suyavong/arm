#include "gpios.h"
#include "flash.h"

void write_to_flash(const char *buf, long size, volatile char *addr)
{
  volatile int *flash_sr = (volatile int*)FLASH_SR;
  volatile int *flash_cr = (volatile int*)FLASH_CR;
  volatile int *flash_keyr = (volatile int*)FLASH_KEYR;
  *flash_keyr = FLASH_KEY1;
  *flash_keyr = FLASH_KEY2;

  for (long i = 0; i < size; ++i)
  {
    // Check BSY bit in FLASH_SR
    while (*flash_sr & (1 << FLASH_SR_BSY_I))
      continue;

    // Set PG in FLASH_CR
    SET_BIT(flash_cr, FLASH_CR_PG_I);

    addr[i] = buf[i];
  }

  SET_BIT(flash_cr, FLASH_CR_LOCK_I);
}
