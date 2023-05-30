#include "kernel.h"
#include "display/display.h"

void main()
{
  print("Hello, World!\n");
  print_number(109);

  // Infinite loop
  while (1) {}
}

void panic(char* message)
{
  print("!!!KERNEL PANIC!!!");
  print(message);
  while (1) {}
}