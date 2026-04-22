#include <stdio>
#include "hardware/adc.h"
#include "pico/stdlib.h"

int int main () 
{
  stdio_init_all();
  while (1) 
  {
    for (int i = 0; i < 10; i++) 
    {
      std::cout << "Hola we!";
    }
  }
  
  return 0;
}
