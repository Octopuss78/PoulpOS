#include "../lib/string.h"
#include "uart.h"


static int is_valid_char(char c)
{
  return (c >= 0x20 && c <= 0x7E);
}

static int readline(char *buf, int max)
{
  int i = 0;
  while(i < max)
  {
    char c = uart_getc();
    if(c=='\n' || c=='\r')
    {
      buf[i] = 0;
      uart_putc('\r');
      uart_putc('\n');
      return i;
    }
    if((c == 0x7f || c == 0x8) && i > 0)
    {
      i--;
      uart_putc('\b');
      uart_putc(' ');
      uart_putc('\b');
    }
    else if (is_valid_char(c) && i < max-1)
    {
      buf[i] = c;
      uart_putc(c);
      i++;
    }
  }
  if(i==max)
    buf[max-1]= 0;
  else {
    buf[i] = 0;
  }
  return i;
}

