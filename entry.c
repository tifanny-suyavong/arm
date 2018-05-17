#include <stdint.h>
#include <sys/types.h>

extern void *_stack;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _ebss;
extern uint32_t _data_loadaddr;

extern void main();

void dummy_handler()
{
  while (1);
}

void entry()
{
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
  (uint32_t) entry,         //Reset vector
  (uint32_t) dummy_handler, // Non Maskable Interrupt
  (uint32_t) dummy_handler, // Hard Fault Interrupt
  (uint32_t) dummy_handler, // Memory Management Fault Interrupt
  (uint32_t) dummy_handler, // Bus Fault Interrupt
  (uint32_t) dummy_handler, // Usage Fault Interrupt
};
