#include "io.h"
//Constants

enum {
    PERIPHERAL_BASE_ADDR = 0x3F000000,
    GPFSEL0         = PERIPHERAL_BASE_ADDR + 0x200000,
    GPPUD           = PERIPHERAL_BASE_ADDR + 0x200094,
    GPPUDCLK0       = PERIPHERAL_BASE_ADDR + 0x200098
};

enum {
    GPIO_FUNCTION_ALT5 = 2
};

enum {
    PULL_NONE = 0,
    PULL_DOWN = 1,
    PULL_UP   = 2
};

enum {
    AUX_BASE        = PERIPHERAL_BASE_ADDR + 0x215000,
    AUX_ENABLES     = AUX_BASE + 4,
    AUX_MU_IO_REG   = AUX_BASE + 64,
    AUX_MU_IER_REG  = AUX_BASE + 68,
    AUX_MU_IIR_REG  = AUX_BASE + 72,
    AUX_MU_LCR_REG  = AUX_BASE + 76,
    AUX_MU_MCR_REG  = AUX_BASE + 80,
    AUX_MU_LSR_REG  = AUX_BASE + 84,
    AUX_MU_CNTL_REG = AUX_BASE + 96,
    AUX_MU_BAUD_REG = AUX_BASE + 104,
    AUX_UART_CLOCK = 250000000,
    UART_BAUD_RATE      = 115200,
    AUX_MU_BAUD_VAL = (AUX_UART_CLOCK / (UART_BAUD_RATE * 8)) - 1
};

enum {
    SYSTMR_CLO = PERIPHERAL_BASE_ADDR + 0x3004
};

//FUNCTIONS

static void mmio_write(unsigned long addr, unsigned int val)
{
  *(volatile unsigned int *)addr = val;
}

static unsigned int mmio_read(unsigned long addr)
{
  return *(volatile unsigned int *) addr;
}

static void delay_us(unsigned int us)
{
    unsigned int start = mmio_read(SYSTMR_CLO);
    while (mmio_read(SYSTMR_CLO) - start < us);
}


static void gpio_set_alt5(unsigned int pin)
{
  unsigned int reg = GPFSEL0 + (pin/10)*4;
  unsigned int shift = (pin % 10) *3;
  unsigned int val = mmio_read(reg);
  val = val & ~(7 << shift);
  val = val | (GPIO_FUNCTION_ALT5 << shift);
  mmio_write(reg,val);
}

static void disable_pull(void)
{
  // Write No Pull to GPPUD
  mmio_write(GPPUD,PULL_NONE);
  delay_us(2);

  //Set pin 14 and 15 in GPPUDCLK0
  unsigned int tmp = (1 << 14) | (1 << 15);
  mmio_write(GPPUDCLK0, tmp);
  delay_us(2);

  //Clean both registers
  mmio_write(GPPUD, PULL_NONE);
  mmio_write(GPPUDCLK0, 0);
}

void uart_init(void)
{
  //Select GPIO ALT5 Function 
  gpio_set_alt5(14);
  gpio_set_alt5(15);

  //Disable Pull Resistors
  disable_pull();

  //Enable Mini UART Auxiliaries
  mmio_write(AUX_ENABLES, 1);
    
  //set TX and RX down for init
  mmio_write(AUX_MU_CNTL_REG, 0);

  //Set data format to 8 bit-mode
  mmio_write(AUX_MU_LCR_REG, 3);

  //Setting Baud rate
  mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD_VAL);

  //Making sure IER and MCR are set to zero
  mmio_write(AUX_MU_MCR_REG, 0);
  mmio_write(AUX_MU_IER_REG, 0);

  //Clearing receive and transmit FIFO
  mmio_write(AUX_MU_IIR_REG, 2 | 4);

  //Enable TX and RX
  mmio_write(AUX_MU_CNTL_REG, 3);
  
}


void uart_putc(char c)
{
  //Waiting for FIFO to accept at least 1 byte
  while (!(mmio_read(AUX_MU_LSR_REG) & (1 << 5)));

  //Write char
  mmio_write(AUX_MU_IO_REG, c);
}

void uart_puts(const char *s)
{
  int i = 0;
  while(s[i])
  {
    if(s[i]=='\n')
      uart_putc('\r');
    uart_putc(s[i]);
    i++;
  }
}
