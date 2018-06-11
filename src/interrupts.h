#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

// DISCLAIMER : Only working for STM32F42xxx and STM32F43xxx
// See section 12.2.4 in the manual

#define EXTI_IMR       0x40013C00          // Interrupt mask register
#define EXTI_RSTR      (EXTI_IMR + 0x08)   // Rising trigger
#define EXTI_FSTR      (EXTI_IMR + 0x0C)   // Falling trigger
#define EXTI_PR        (EXTI_IMR + 0x14)   // Pending

#define SYSCFG_BASE    0x40013800          // System config controller
#define SYSCFG_EXTICR1 (SYSCFG_BASE + 0x8) // External interrupt config

#define NVIC_ISER0     0xE000E100          // Interrupt Set-enable registers
#define NVIC_ICPR0     0xE000E280          // Interrupt Clear pending

#endif
