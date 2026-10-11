#include "uart.h"
#include "test.h"
#include "shell.h"

void kernel_main(void)
{
  uart_init();
  uart_puts("Welcome to Poulp OS\n\n");

  shell_run();
}
