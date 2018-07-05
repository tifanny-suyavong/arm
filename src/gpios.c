#include "gpios.h"

void set_bit(volatile int *reg, int pos)
{
    *reg = (*reg | (1 << pos));
}

void unset_bit(volatile int *reg, int pos)
{
    *reg = (*reg & ~(1 << pos));
}

void set_moder_mode(volatile int *gpio_moder, int pos, int mode)
{
    *gpio_moder = (*gpio_moder & ~(3 << (pos * 2))) | (mode << (pos * 2));
}

void set_alternate_function_port(volatile int *gpio_afrx, int pos, int select)
{
    *gpio_afrx = (*gpio_afrx& ~(15 << ((pos % 8) * 4))) | (select << (pos % 8) * 4);
}

