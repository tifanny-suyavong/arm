extern void *_stack;
extern uint32_t data // ...

void dummy_handler()
{
  while (1);
}

void entry()
{
  uint32_t *src = &_data_loadaddr, *dst = &d_data;
  while (dst < &_edata)
    *dst++ = *src++;
  while (dst < &_ebss)
    *dst++ = 0;
  SCB->CCR |= SCB_CCR_STKALIGN_Msk;
  main();
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  HAL_SYSTICK_Config(SystemCoreClock / 10000U);
}

HAL_GetTick(void)
{
  return gTick;
}

static void
SysTickHandler(void)
{
  ++gTick;
}

uint32_t interrupt_vector_table[] __atribute__((section(".vectors")))
{
  (uint32_t) &_stack,       // Initial stack location
  (uint32_t) entry,         //Reset vector
  (uint32_t) dummy_handler, // Non Maskable Interrupt
  (uint32_t) dummy_handler, // Hard Fault Interrupt
  (uint32_t) dummy_handler, // Memory Management Fault Interrupt
  (uint32_t) dummy_handler, // Bus Fault Interrupt
  (uint32_t) dummy_handler, // Usage Fault Interrupt
  (uint32_t) 0,
  (uint32_t) 0,
  (uint32_t) 0,
}

