#ifndef STRING_H
#define STRING_H

#include "types.h"

size_t strlen(const char *s);
void *memset(void *dst, int val, size_t count);
void *memcpy(void *dst, const void *src, size_t size);
int memcmp(const void *a, const void *b, size_t n);
int strncmp(const char *a, const char *b, size_t n);
char *strncpy(char *dst, const char *src, size_t n);
unsigned long parse_hex(const char *s, int *ok);

#endif
