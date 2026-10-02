#include "string.h"

size_t strlen(const char *s)
{
  size_t i = 0;
  while(s[i])
    i++;
  return i;
}

void *memset(void *dst, int val, size_t count)
{
  unsigned char *ptr = (unsigned char *) dst;
  unsigned char c = (unsigned char) val;

  for(size_t i = 0; i < count; i++)
    ptr[i] = c;

  return dst;
}

void *memcpy(void *dst, const void *src, size_t size)
{
  unsigned char *ptr_d = (unsigned char *)dst;
  const unsigned char *ptr_s = (const unsigned char *)src;

  for(size_t i = 0; i < size; i++)
    ptr_d[i] = ptr_s[i];

  return dst;
}

int memcmp(const void *a, const void *b, size_t n)
{
  const unsigned char *ptr_a = (const unsigned char *) a;
  const unsigned char *ptr_b = (const unsigned char *) b;

  for(size_t i = 0; i<n; i++)
    if(ptr_a[i] != ptr_b[i])
      return (int) (ptr_a[i]-ptr_b[i]);
  return 0;
}


int strncmp(const char *a, const char *b, size_t n)
{
  for(size_t i =0; i < n; i++)
  {
    if(!a[i] || (a[i]!= b[i]))
      return (int) ((unsigned char)a[i]-(unsigned char)b[i]);
  }
  return 0;
}

char *strncpy(char *dst, const char *src, size_t n)
{
  size_t i = 0;
  while(i < n && src[i])
  {
    dst[i] = src[i];
    i++;
  }
  while(i < n)
  {
    dst[i] = 0;
    i++;
  }
  return dst;
}

static unsigned char hex2c(char x)
{
  //[0-9]
  if (x >= '0' && x <= '9')
    return x - 0x30;
  //[A-F]
  if(x >= 'A' && x <= 'F')
    return 10 + (x - 0x41);
  //[a-f]
  if( x >= 'a' && x <= 'f')
    return 10 + (x - 0x61);
  //invalid char
  return 0xff;
}

unsigned long parse_hex(const char *s, int *ok)
{
  const char *x = s;
  unsigned long res = 0;
  *ok = 0;
  //If starts with 0x or 0X skip
  if(!strncmp(x, "0x", 2) || !strncmp(x, "0X", 2))
    x = x+2;
  //Emptry after 0x
  if(!x[0])
    return 0;

  for(size_t i=0; x[i]; i++)
  {
    //More then 16 numbers
    if(i == 16)
      return 0;
    unsigned char c = hex2c(x[i]);

    //Not hex char
    if(c == 0xff)
    {
      *ok = 0;
      return 0;
    }
    res = res << 4;
    res += c;
  }
  *ok = 1;
  return res;
}
