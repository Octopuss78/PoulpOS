#include "cmds.h"
#include "uart.h"

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

const struct Cmd cmds[] = 
  {
    {"echo", &cmd_echo, "basic echo"},
    {"help", &cmd_help, "display this help message"}
  };
  
const int cmd_count = sizeof(cmds) / sizeof(cmds[0]);

