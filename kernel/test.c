#include "test.h"
#include "uart.h"
#include "string.h"

static int passed = 0;
static int failed = 0;

static void check(int cond, const char *name)
{
  if (cond) {
    passed++;
    uart_puts("[PASS] ");
  } else {
    failed++;
    uart_puts("[FAIL] ");
  }
  uart_puts(name);
  uart_puts("\n");
}

static void test_string(void)
{
  uart_puts("-- string --\n");

  /* strlen */
  check(strlen("hello") == 5, "strlen(\"hello\") == 5");
  check(strlen("") == 0, "strlen(\"\") == 0");

  /* memset + memcmp */
  unsigned char buf[8];
  unsigned char expected[8] = {0xAB, 0xAB, 0xAB, 0xAB, 0xAB, 0xAB, 0xAB, 0xAB};
  memset(buf, 0xAB, 8);
  check(memcmp(buf, expected, 8) == 0, "memset 0xAB puis memcmp == 0");

  memset(buf, 0x1AB, 1); /* seul l'octet bas (0xAB) doit etre ecrit */
  check(buf[0] == 0xAB, "memset tronque val a un octet");

  /* memcmp : ordre et signe */
  unsigned char lo[3] = {1, 2, 3};
  unsigned char hi[3] = {1, 2, 4};
  unsigned char ff[1] = {0xFF};
  unsigned char one[1] = {0x01};
  check(memcmp(lo, hi, 3) < 0, "memcmp {1,2,3} < {1,2,4}");
  check(memcmp(hi, lo, 3) > 0, "memcmp {1,2,4} > {1,2,3}");
  check(memcmp(ff, one, 1) > 0, "memcmp 0xFF > 0x01 (unsigned)");
  check(memcmp(lo, hi, 0) == 0, "memcmp n=0 == 0");

  /* memcpy */
  unsigned char src[4] = {0xDE, 0xAD, 0xBE, 0xEF};
  unsigned char dst[4] = {0};
  memcpy(dst, src, 4);
  check(memcmp(dst, src, 4) == 0, "memcpy copie 4 octets");

  /* strncmp */
  check(strncmp("peek", "peek", 5) == 0, "strncmp peek == peek");
  check(strncmp("peek", "peekaboo", 5) != 0, "strncmp peek != peekaboo");
  check(strncmp("abc", "abd", 2) == 0, "strncmp abc/abd sur 2 == 0");
  check(strncmp("abc", "abd", 3) < 0, "strncmp abc < abd");
  check(strncmp("abc", "xyz", 0) == 0, "strncmp n=0 == 0");
  check(strncmp("\xFF", "\x01", 1) > 0, "strncmp 0xFF > 0x01 (unsigned)");

  /* strncpy : src courte -> remplissage avec \0 */
  char s1[8];
  memset(s1, 'X', 8);
  strncpy(s1, "ab", 8);
  check(s1[0] == 'a' && s1[1] == 'b', "strncpy copie \"ab\"");
  check(s1[2] == 0 && s1[7] == 0, "strncpy remplit le reste de \\0");

  /* strncpy : src longue -> pas de \0 final */
  char s2[8];
  memset(s2, 'X', 8);
  strncpy(s2, "abcdefghij", 8);
  check(s2[7] == 'h', "strncpy tronque sans \\0 final");
}

static void check_hex(const char *s, int expected_ok, unsigned long expected)
{
  int ok;
  unsigned long v = parse_hex(s, &ok);
  int pass = (ok == expected_ok) && (!expected_ok || v == expected);

  check(pass, s);

  if (!pass) {
    uart_puts("       obtenu ok=");
    uart_putc(ok ? '1' : '0');
    uart_puts(" valeur=");
    uart_puthex(v);
    uart_puts("\n");
  }
}

static void test_parse_hex(void)
{
  uart_puts("-- parse_hex --\n");

  /* valides */
  check_hex("0x80000", 1, 0x80000);
  check_hex("3F200000", 1, 0x3F200000);
  check_hex("0X3f200000", 1, 0x3F200000);
  check_hex("0x0", 1, 0);
  check_hex("0xFFFFFFFFFFFFFFFF", 1, 0xFFFFFFFFFFFFFFFFUL);

  /* invalides */
  check_hex("", 0, 0);
  check_hex("0x", 0, 0);
  check_hex("zz", 0, 0);
  check_hex("0x12g", 0, 0);
  check_hex("0x1FFFFFFFFFFFFFFFF", 0, 0); /* 17 chiffres */
}

void run_tests(void)
{
  uart_puts("\n===== TESTS =====\n");

  test_string();
  test_parse_hex();

  uart_puts("===== passed: ");
  uart_puthex(passed);
  uart_puts("  failed: ");
  uart_puthex(failed);
  uart_puts(" =====\n\n");
}
