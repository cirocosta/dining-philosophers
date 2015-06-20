#include <iostream>
#include <pthread.h>

#include "Input.hpp"
#include "Monitor.hpp"

bool hasFood = true;

void* philosopher()
{
  while (hasFood) {
    std::cout << "Lol" << std::endl;
  }

  pthread_exit(NULL);
}

class Philosopher
{
public:
  Philosopher();
};

int main(const int argc, const char** argv)
{
  Input inp;
  inp.parse_input(argc, argv);

  std::cout << inp << std::endl;

  return 0;
}
