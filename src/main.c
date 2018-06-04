#define RCC_AHB1ENR  (0x40023800 + 0x30) // Reset and clock controller

// User button
#define GPIOA_MODER  0x40020000
#define GPIOA_IDR    (GPIOA_MODER + 0x10)

// Led LD3
#define GPIOG_MODER  0x40021800
#define GPIOG_ODR    (GPIOG_MODER + 0x14)

#define PORTA_MODE    0x00
#define PORTG_MODE    0x01

#define SET_MODER_OUTPUT(A, POS) (A & ~(1 << (POS * 2 + 1))) | (1 << (POS * 2))

int main(void)
{
  volatile int *rcc = (volatile int *) RCC_AHB1ENR;
  volatile int *gpioa_mode = (volatile int *) GPIOA_MODER;
  volatile int *gpiog_mode = (volatile int *) GPIOG_MODER;
  volatile int *button_input_data = (volatile int *) GPIOA_IDR;
  volatile int *led_output_data = (volatile int *) GPIOG_ODR;

  // Enable all the GPIOs
  *rcc = *rcc | ((1 << 11) - 1);

  // Set GPIO mode
  *gpioa_mode = *gpioa_mode | PORTA_MODE;
  int foo = SET_MODER_OUTPUT(*gpiog_mode, 13);
  int bar = *gpiog_mode;
  int baz = (bar & ~(1 << (13 * 2 + 1)));
  int foobardo = baz | (1 << 13 * 2);
  *gpiog_mode = foobardo;

  while (1)
  {
    /*
     *    // TODO: Find value for pushed button
    if (*button_input_data & 0x1)
      // TODO: Find value to enable led
      *led_output_data = *led_output_data | (1 << 12);
    else
      *led_output_data = *led_output_data & ~(1 << 12);
    */
    *led_output_data = *led_output_data | (1 << 13);
  }
  return 0;
}
