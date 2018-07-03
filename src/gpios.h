#ifndef _GPIOS_H_
# define _GPIOS_H_

// GPIO bases
#define GPIOA_MODER     0x40020000
#define GPIOB_BASE      0x40020400
#define GPIOB_MODER     GPIOB_BASE
#define GPIOG_MODER     0x40021800

// Offset input/output
#define IDR_OFF         0x10
#define ODR_OFF         0x14

// Offset alternate function port high (15-8) / low (7-0)
#define AFRL_OFF        0x20
#define AFRH_OFF        0x24

// MODER possible values
#define MODER_INPUT     0
#define MODER_OUTPUT    1
#define MODER_ALTERNATE 2

// Needed GPIOS registers
#define GPIOA_IDR       (GPIOA_MODER + IDR_OFF)

#define GPIOB_AFRL      (GPIOB_MODER + AFRL_OFF)

#define GPIOG_ODR       (GPIOG_MODER + ODR_OFF)

#define GPIOB_PUPDR  (GPIOB_BASE + 0x0C)

void set_bit(volatile int *reg, int pos);

void unset_bit(volatile int *reg, int pos);

void set_moder_mode(volatile int *gpio_moder, int pos, int mode);

void set_alternate_function_port(volatile int *gpio_afrx, int pos, int select);

#endif
