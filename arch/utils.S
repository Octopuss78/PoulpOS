.section ".text.utils"

.global get_el

get_el:
  // Get Current EL
  mrs x0, CurrentEL
  lsr x0, x0, #2
  ret
