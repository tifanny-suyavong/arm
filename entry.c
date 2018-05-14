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
