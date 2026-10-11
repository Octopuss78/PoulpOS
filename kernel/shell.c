#include "../lib/string.h"
#include "uart.h"
#include "shell.h"
#include "cmds.h"

#define MAX_CHARS 128
#define MAX_ARGS 8

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

    //Enter check -> End of line
    if(c=='\n' || c=='\r')
    {
      buf[i] = 0;
      uart_putc('\r');
      uart_putc('\n');
      return i;
    }

    //Backspace check
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

  //Terminate line and avoid off-by-one
  if(i==max)
    buf[max-1]= 0;
  else {
    buf[i] = 0;
  }
  return i;
}

static int tokenize(char *line, char **argv, int max_args)
{
  int argc = 0;
  int is_word = 0;
  int i = 0;
  while(line[i])
  {
    //End word
    if(line[i]==' ')
    {
      line[i] = 0;
      is_word = 0;
    }
    else
    {
      //New word
      if(!is_word)
      {
        //Max args reached -> exit
        if(argc==max_args)
          break;
        //New word
        argv[argc] = &line[i];
        argc++;
        is_word = 1;
      }
    }
    i++;
  }
  return argc;
}

void shell_run(void)
{
  char line[MAX_CHARS];
  char *argv[MAX_ARGS];
  while(1)
  {
    uart_puts("poulp> ");
    readline(line, MAX_CHARS);

    int argc = tokenize(line, argv, MAX_ARGS);
    if(argc==0)
      continue;

    for(int i = 0; i < cmd_count; i++)
    { 
      if(!strncmp(argv[0], cmds[i].name,strlen(cmds[i].name)+1))
      {
        cmds[i].fn(argc,argv);
        break;
      }

      if(i == cmd_count-1)
      {
        uart_puts("unknown command: ");
        uart_puts(argv[0]);
        uart_puts("\nuse \"help\" to list valid commands\n");
      }
    }
  }
}
