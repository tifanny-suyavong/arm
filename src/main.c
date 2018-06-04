#define RCC_AHB1ENR  (0x40023800 + 0x30)         // Reset and clock controller

#define IDR_OFF      0x10
#define ODR_OFF      0x14

// User button : pin A0
#define GPIOA_MODER  0x40020000
#define GPIOA_IDR    (GPIOA_MODER + IDR_OFF)

// Led LD3 : pin G13
#define GPIOG_MODER  0x40021800
#define GPIOG_ODR    (GPIOG_MODER + ODR_OFF)

#define SET_MODER_INPUT(A, POS)                         \
    (A & ~(1 << (POS * 2 + 1))) & ~(1 << (POS * 2))
#define SET_MODER_OUTPUT(A, POS)                        \
    (A & ~(1 << (POS * 2 + 1))) | (1 << (POS * 2))

int main(void)
{
    volatile int *rcc = (volatile int *) RCC_AHB1ENR;
    volatile int *gpioa_mode = (volatile int *) GPIOA_MODER;
    volatile int *gpiog_mode = (volatile int *) GPIOG_MODER;
    volatile int *button_input_data = (volatile int *) GPIOA_IDR;
    volatile int *led_output_data = (volatile int *) GPIOG_ODR;

    // Enable all the GPIOs
    *rcc = *rcc | 0x1FF;

    // Set GPIO mode
    *gpioa_mode = SET_MODER_INPUT(*gpioa_mode, 0);
    *gpiog_mode = SET_MODER_OUTPUT(*gpiog_mode, 13);

    while (1)
    {
        if (*button_input_data & 0x1)
            *led_output_data = *led_output_data | (1 << 13);
        else
            *led_output_data = *led_output_data & ~(1 << 13);
    }
    return 0;
}

