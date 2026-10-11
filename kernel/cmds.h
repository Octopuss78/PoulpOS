#ifndef CMDS_H
#define CMDS_H

typedef void (*Fn)(int argc, char **argv);

struct Cmd 
{
  const char *name;
  Fn fn;
  const char *help;
};

//Commands description and fn ptrs defined in cmd.c
extern const struct Cmd cmds[];
extern const int cmd_count;

#endif
