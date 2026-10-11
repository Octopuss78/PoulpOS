#ifndef IO_H
#define IO_H

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);
void uart_puthex(unsigned long v);
void uart_puthex32(unsigned int v);

#endif
