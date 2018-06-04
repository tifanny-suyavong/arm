#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

// DISCLAIMER : Only working for STM32F42xxx and STM32F43xxx
// See section 12.2.4 in the manual

#define EXTI_IMR       0x40013C00          // Interrupt mask register
#define EXTI_RSTR      (EXTI_IMR + 0x08)   // Rising trigger
#define EXTI_FSTR      (EXTI_IMR + 0x0C)   // Falling trigger

#define SYSFG_BASE     0x40013800          // System config controller
#define SYSFG_EXTICR1  (SYSFG_BASE + 0x8)  // External interrup config
                                           // TODO: Check which CR to use
#endif
