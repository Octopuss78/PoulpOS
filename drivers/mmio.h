#ifndef MMIO_H
#define MMIO_H

enum {
      PERIPHERAL_BASE_ADDR = 0x3F000000
};

void mmio_write(unsigned long addr, unsigned int val);
unsigned int mmio_read(unsigned long addr);

#endif
