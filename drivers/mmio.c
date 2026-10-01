#include "mmio.h"

void mmio_write(unsigned long addr, unsigned int val)
{
  *(volatile unsigned int *)addr = val;
}

unsigned int mmio_read(unsigned long addr)
{
  return *(volatile unsigned int *) addr;
}
