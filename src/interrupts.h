#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#define EXTI_IMR       0x40013C00          // Interrupt mask register
#define EXTI_RSTR      (EXTI_IMR + 0x08)   // Rising trigger
#define EXTI_FSTR      (EXTI_IMR + 0x0C)   // Falling trigger
#define EXTI_PR        (EXTI_IMR + 0x14)   // Pending

#define SYSCFG_BASE    0x40013800          // System config controller
#define SYSCFG_EXTICR1 (SYSCFG_BASE + 0x8) // External interrupt config

#define NVIC_ISER0     0xE000E100          // Interrupt Set-enable registers
#define NVIC_ICPR0     0xE000E280          // Interrupt Clear pending

#include "gpios.h"

void enable_button_interrupt(void);

void interrupt_handler(void);

void power_led(int on);

#endif
