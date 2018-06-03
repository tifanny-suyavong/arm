#define RCC_AHB1ENR 0x40023800 + 0x10 // Reset and clock controller

#define GPIO_MODER  0x0               // TODO: Find gpio base
#define GPIO_IDR    GPIO_MODER + 0x10
#define GPIO_ODR    GPIO_MODER + 0x14

void break_here(void)
{
  return;
}

int main(void)
{
  while (1)
    break_here();

  volatile int *rcc = (volatile int*) RCC_AHB1ENR;
  volatile int *gpio_mode = (volatile int*) GPIO_MODER;
  volatile int *gpio_input_data = (volatile int*) GPIO_IDR;
  volatile int *gpio_output_data = (volatile int*) GPIO_ODR;

  // Enable all the GPIOs
  *rcc = *rcc | 2047;
  // TODO: Find gpio to enable write mode for ODR
  *gpio_mode = *gpio_mode | 0x0;
  // TODO: Find value for pushed button
  if (*gpio_input_data & 0x0)
      // TODO: Find value to enable led
      *gpio_output_data = *gpio_output_data & 0x0;
  return 0;
}
