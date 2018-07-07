#include <stdint.h>
#include <sys/types.h>

extern void *_stack;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;
extern uint32_t _data_loadaddr;

extern int main(void);

void dummy_handler0(void)
{
  return;
}

void dummy_handler1(unsigned long *hardfault_args)
{
  /* volatile unsigned long stacked_r0 ; */
  /* volatile unsigned long stacked_r1 ; */
  /* volatile unsigned long stacked_r2 ; */
  /* volatile unsigned long stacked_r3 ; */
  /* volatile unsigned long stacked_r12 ; */
  /* volatile unsigned long stacked_lr ; */
  /* volatile unsigned long stacked_pc ; */
  /* volatile unsigned long stacked_psr ; */
  /* volatile unsigned long _CFSR ; */
  /* volatile unsigned long _HFSR ; */
  /* volatile unsigned long _DFSR ; */
  /* volatile unsigned long _AFSR ; */
  /* volatile unsigned long _BFAR ; */
  /* volatile unsigned long _MMAR ; */

  /* stacked_r0 = ((unsigned long)hardfault_args[0]) ; */
  /* stacked_r1 = ((unsigned long)hardfault_args[1]) ; */
  /* stacked_r2 = ((unsigned long)hardfault_args[2]) ; */
  /* stacked_r3 = ((unsigned long)hardfault_args[3]) ; */
  /* stacked_r12 = ((unsigned long)hardfault_args[4]) ; */
  /* stacked_lr = ((unsigned long)hardfault_args[5]) ; */
  /* stacked_pc = ((unsigned long)hardfault_args[6]) ; */
  /* stacked_psr = ((unsigned long)hardfault_args[7]) ; */

  /* // Configurable Fault Status Register */
  /* // Consists of MMSR, BFSR and UFSR */
  /* _CFSR = (*((volatile unsigned long *)(0xE000ED28))) ; */

  /* // Hard Fault Status Register */
  /* _HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ; */

  /* // Debug Fault Status Register */
  /* _DFSR = (*((volatile unsigned long *)(0xE000ED30))) ; */

  /* // Auxiliary Fault Status Register */
  /* _AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ; */

  /* // Read the Fault Address Registers. These may not contain valid values. */
  /* // Check BFARVALID/MMARVALID to see if they are valid values */
  /* // MemManage Fault Address Register */
  /* _MMAR = (*((volatile unsigned long *)(0xE000ED34))) ; */
  /* // Bus Fault Address Register */
  /* _BFAR = (*((volatile unsigned long *)(0xE000ED38))) ; */

  /* __asm("BKPT #0\n") ; // Break into the debugger */
  return;
}

void dummy_handler2(void)
{
  return;
}

void dummy_handler3(void)
{
  return;
}

void dummy_handler4(void)
{
  return;
}

extern void interrupt_handler(void);

void entry(void)
{
  for (uint32_t *bss = &_bss; bss < &_ebss; bss++)
    *bss = 0;
  
  uint32_t *src = &_data_loadaddr, *dst = &_data;
  while (dst < &_edata)
    *dst++ = *src++;
  while (dst < &_ebss)
    *dst++ = 0;
  main();
}

uint32_t interrupt_vector_table[107] __attribute__((section(".vectors"))) =
{
  (uint32_t) &_stack,       // Initial stack location
  (uint32_t) entry,         // Reset vector
  (uint32_t) dummy_handler0, // Non Maskable Interrupt
  (uint32_t) dummy_handler1, // Hard Fault Interrupt
  (uint32_t) dummy_handler2, // Memory Management Fault Interrupt
  (uint32_t) dummy_handler3, // Bus Fault Interrupt
  (uint32_t) dummy_handler4, // Usage Fault Interrupt
  [22] = (uint32_t) interrupt_handler,
};
