#include "kernel.h"
#include "display/display.h"

void main()
{
  print("Hello, World!\n");
  print_number(109);

  // Infinite loop
  while (1) {}
}