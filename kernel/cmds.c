#include "cmds.h"
#include "../drivers/uart.h"
#include "../arch/utils.h"
#include "../lib/string.h"
#include "../drivers/mmio.h"

#define MAX_WORDS 0x400

static void cmd_echo(int argc, char **argv)
{
  //skip first word 'echo'
  for(int i = 1; i < argc; i++)
  {
    if(i!=1)
      uart_puts(" ");
    uart_puts(argv[i]);
  }
  uart_puts("\n"); 
}

static void cmd_help(int argc, char **argv)
{
  for(int i = 0; i < cmd_count; i++)
  {
    uart_puts(cmds[i].name);
    uart_puts("\t");
    uart_puts(cmds[i].help);
    uart_puts("\n");
  }
}

static void cmd_el(int argc, char **argv)
{
  uart_puts("Current Exception Level: ");
  //Convert nbr to char
  uart_putc(get_el()+'0');
  uart_puts("\n");
}

static int parse_arg(const char *s, unsigned long *out)
{
  int ok;
  unsigned long v = parse_hex(s, &ok);
  if(!ok)
  {
    uart_puts("Invalid hex value: ");
    uart_puts(s);
    uart_puts("\n");
    return 0;
  }
  *out = v;
  return 1;
}

static void cmd_md(int argc, char **argv)
{
  if(argc < 2 || argc > 3)
  {
    uart_puts("usage: md <addr> [count]\n");
    return;
  }

  unsigned long addr;
  if(!parse_arg(argv[1], &addr))
  {
    uart_puts("\n");
    return;
  }
  if((addr%4) != 0)
  {
    uart_puts("address must be 4-byte aligned\n");
    return;
  }

  unsigned long count = 1;
  if(argc == 3)
  {
    if(!parse_arg(argv[2], &count))
      return;
  }
  if(count > MAX_WORDS || count == 0)
  {
    uart_puts("Please use a number between 1 and ");
    uart_puthex(MAX_WORDS);
    uart_puts("\n");
    return;
  }

  //Avoid int overflows
  if(addr > 0XFFFFFFFFFFFFFFFF - count * 4)
  {
    uart_puts("overflow detected, nice try though\n");
    return;
  }

  for(unsigned long i = 0; i < count; i++)
  {
    if(i%4 == 0)
    {
      uart_puthex(addr+i*4);
      uart_puts(": ");
    }

    uart_puthex32(mmio_read(addr+i*4));
    uart_puts(" ");

    if((i%4 == 3) || (i == count -1))
      uart_puts("\n");
  }
}


const struct Cmd cmds[] = 
  {
    {"echo", &cmd_echo, "basic echo"},
    {"help", &cmd_help, "display this help message"},
    {"el", &cmd_el, "get current exception level"},
    {"md", &cmd_md, "md <addr> [count] : display words from a given address"}
  };
  
const int cmd_count = sizeof(cmds) / sizeof(cmds[0]);
