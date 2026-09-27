#include "io.h"

void main(void)
{
  uart_init();
  uart_puts("Hello world\n");
  while(1);
}
