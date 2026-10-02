#include "uart.h"
#include "test.h"

void kernel_main(void)
{
  uart_init();
  uart_puts("Hello world\n");

  run_tests();
  while(1)
  {
    uart_putc(uart_getc());
  }
}
