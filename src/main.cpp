#include <iostream>
#include <pthread.h>
#include "Input.hpp"

bool hasFood = true;

void* philosopher()
{
  while (hasFood) {
  }

  pthread_exit(NULL);
}

int main(const int argc, const char** argv)
{
  Input inp;
  inp.parse_input(argc, argv);

  std::cout << inp << std::endl;

  return 0;
}
