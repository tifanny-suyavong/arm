#include "interrupts.h"

void enable_button_interrupt(void)
{
    // Set GPIO A mode to input (Button A0)
    set_moder_mode((volatile int *)GPIOA_MODER, 0, MODER_INPUT);

    // Enable external interrupt for PA0 (user button)
    volatile int *exticr1 = (volatile int *)SYSCFG_EXTICR1;
    *exticr1 &= 0xFFFFFFF0;

    // Unmask interrupt for line 0
    volatile int *extimr = (volatile int *)EXTI_IMR;
    *extimr |= 0x1;

    // Set rising/falling triggers for line 0
    volatile int *extrstr = (volatile int *)EXTI_RSTR;
    volatile int *extfstr = (volatile int *)EXTI_FSTR;
    *extrstr |= 0x1;
    *extfstr |= 0x1;
}

void interrupt_handler(void)
{
    static int led_status = 0;
    led_status = !led_status;

    // 
    volatile int *exti_pr = (volatile int *)EXTI_PR;
    *exti_pr |= 0x1;

    // 
    volatile int *nvic_icpr0 = (volatile int *)NVIC_ICPR0;
    *nvic_icpr0 |= 1 << 6;

    power_led(led_status);
}

void power_led(int on)
{
    volatile int *led_output_data = (volatile int *)GPIOG_ODR;

    if (on)
        *led_output_data = *led_output_data | (1 << 13);
    else
        *led_output_data = *led_output_data & ~(1 << 13);
}
