#include "interrupts.h"
#include "uart.h"

// MODER possible values
#define MODER_INPUT 0
#define MODER_OUTPUT 1
#define MODER_ALTERNATE 2

// Reset and clock controller
#define RCC_BASE     0x40023800
#define RCC_AHB1ENR  (RCC_BASE + 0x30)
#define RCC_APB2ENR  (RCC_BASE + 0x44)

// Offset input/output
#define IDR_OFF      0x10
#define ODR_OFF      0x14

// User button : pin A0
#define GPIOA_MODER  0x40020000
#define GPIOA_IDR    (GPIOA_MODER + IDR_OFF)

// Led LD3 : pin G13
#define GPIOG_MODER  0x40021800
#define GPIOG_ODR    (GPIOG_MODER + ODR_OFF)

void power_led(int on)
{
  volatile int *led_output_data = (volatile int *)GPIOG_ODR;

  if (on)
    *led_output_data = *led_output_data | (1 << 13);
  else
    *led_output_data = *led_output_data & ~(1 << 13);
}

void interrupt_handler(void)
{
  static int led_status = 0;
  led_status = !led_status;

  volatile int *exti_pr = (volatile int *)EXTI_PR;
  *exti_pr |= 1;
  volatile int *nvic_icpr0 = (volatile int *)NVIC_ICPR0;
  *nvic_icpr0 |= 1 << 6;

  power_led(led_status);
}

static void set_moder_mode(volatile int *gpio_moder, int pos, int mode)
{
  *gpio_moder = (*gpio_moder & ~(3 << (pos * 2))) | (mode << (pos * 2));
}

static void enable_usart(void)
{
    // Set alternate mode (AF7 = 0b0111) for PB6 and PB7
    volatile int *gpiob_afrl = (volatile int *)GPIOB_AFRL;
    *gpiob_afrl = (*gpiob_afrl | 0x77000000);

  volatile int *usart1_cr1 = (volatile int *)USART1_CR1;

  // Enable USART
  // Set UE and M (8 bits)
  *usart1_cr1 = (*usart1_cr1 | 0x2000) & 0xFFFFEFFF;

  // Disable parity
  *usart1_cr1 &= 0xFFFFFBFF;

  // Set 1 stop bit
  volatile int *usart1_cr2 = (volatile int *)USART1_CR2;
  *usart1_cr2 &= 0xFFFFCFFF;

  // Disable DMAT
  volatile int *usart1_cr3 = (volatile int *)USART1_CR3;
  *usart1_cr3 &= 0xFFFFFF7F;
  
  // Configure baud rate for 115.2KBps
  // 8Mhz, Over8=0, USARTDIV=4.3125
  // USARTDIV = DIV_Mantissa + DIV_Fraction/16
  // DIV_Mantisa=4; DIV_Fraction=0.3125*16=5
  volatile int *usart1_brr = (volatile int *)USART1_BRR;
  *usart1_brr = (*usart1_brr & 0xFFFF0000) | (4 << 4) | 5;
}

static void enable_usart_transmitter(void)
{
  // Set TE bit to send idle frame as first transmission
  volatile int *usart1_cr1 = (volatile int *)USART1_CR1;
  *usart1_cr1 |= 0x8;
}

static void enable_usart_receiver(void)
{
  // Set RE bit
  volatile int *usart1_cr1 = (volatile int *)USART1_CR1;
  *usart1_cr1 |= 0x4;
}

void send_usart_data(const char *buf, long len)
{
  volatile int *usart_sr = (volatile int *)USART1_SR;

  for (; len > 0; --len, ++buf)
  {
    // Wait for TXE=1
    while (!(*usart_sr & 0x80))
      continue;

    // Write data
    volatile int *usart_dr = (volatile int *)USART1_DR;
    *usart_dr = (*usart_dr & 0xFFFFFF00) | *buf;
  }

  // Wait for TC=1
  while (!(*usart_sr & 0x40))
    continue;
}

int main(void)
{
    // Enable GPIO[A, B, G] and CRC clock
    volatile int *rcc_ahb1enr = (volatile int *)RCC_AHB1ENR;
    *rcc_ahb1enr = (*rcc_ahb1enr | 0x1043) & 0xFFFFF843;

    // Enable USART1
    volatile int *rcc_apb2enr = (volatile int *)RCC_APB2ENR;
    *rcc_apb2enr = *rcc_apb2enr | 0x10;

    // Set GPIO mode
    set_moder_mode((volatile int *)GPIOA_MODER, 0, MODER_INPUT);
    set_moder_mode((volatile int *)GPIOB_MODER, 6, MODER_ALTERNATE);
    set_moder_mode((volatile int *)GPIOB_MODER, 7, MODER_ALTERNATE);
    set_moder_mode((volatile int *)GPIOG_MODER, 13, MODER_OUTPUT);

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

    // Enable interrupts in Cortex m4
    volatile int *nvic_iser0 = (volatile int *)NVIC_ISER0;
    *nvic_iser0 |= 1 << 6;

    // Enable UART
    enable_usart();

    enable_usart_receiver();

    enable_usart_transmitter();
    char buf[] = "Hello World\n";
    while (1)
        send_usart_data(buf, sizeof (buf));
    //send_usart_data("\xFF", 1);
    
    // Read char (should be done with interrupt)
    volatile int *usart_dr = (volatile int *)USART1_DR;
    volatile int *usart_sr = (volatile int *)USART1_SR;
    while (!(*usart_sr & 0x20))
      continue;

    while (1)
      continue;

    return 0;
}
