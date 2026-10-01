.section ".text.boot"

.global _start

_start:
  // Check that processor ID = 0
  mrs x1, mpidr_el1 //X1 <- mpidr_el1 (read the CPU core number)
  and x1,x1, #0xFF
  cbz x1, 2f

1:
  wfe // wait for event 
  b 1b
2:
  //Main
  //Set stack below
  ldr x1, =_start
  mov sp, x1

  //Init BSS
  ldr x1, =__bss_start
  ldr w2, =__bss_size
3:
  cbz x2, 4f
  str xzr, [x1], #8
  sub w2, w2, #1
  cbnz w2, 3b

  //Jump to main() in our C code
4:
  bl main
  b 1b
  
  
