#include "uart.h"
#include "test.h"
#include "shell.h"

void kernel_main(void)
{
  uart_init();
  uart_puts("Hello world\n");

  shell_run();
}
