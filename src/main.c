#define RCC_AHB1ENR  (0x40023800 + 0x10) // Reset and clock controller

// User button
#define GPIOA_MODER  0x40020000 
#define GPIOA_IDR    (GPIOA_MODER + 0x10)

// Led LD3
#define GPIOG_MODER  0x40021800
#define GPIOG_ODR    (GPIOG_MODER + 0x14)

#define PORTA_MODE    0x0
#define PORTG_MODE    0x1

int main(void)
{
    volatile int *rcc = (volatile int *) RCC_AHB1ENR;
    volatile int *gpioa_mode = (volatile int *) GPIOA_MODER;
    volatile int *gpiog_mode = (volatile int *) GPIOG_MODER;
    volatile int *button_input_data = (volatile int *) GPIOA_IDR;
    volatile int *led_output_data = (volatile int *) GPIOG_ODR;

    // Enable all the GPIOs
    *rcc = *rcc | ((1 << 11) - 1);
  
    // TODO: Find gpio to enable write mode for ODR
    *gpioa_mode = *gpioa_mode | PORTA_MODE;
    *gpiog_mode = *gpiog_mode | PORTG_MODE;
    
    // TODO: Find value for pushed button
    if (*button_input_data & 0x1)
        // TODO: Find value to enable led
        *led_output_data = *led_output_data & (1 << 12);
    else
        *led_output_data = *led_output_data & ~(1 << 12);
    return 0;
}
