#include <iostream>
#include <thread>
#include "./lib/Food.hpp"
#include "./lib/Philosopher.hpp"
#include "./lib/Input.hpp"

using std::cout;
using std::endl;
using std::cerr;

std::vector<PhilPtr> philosophers;
std::vector<std::thread> threads;

int main(const int argc, const char** argv)
{
  Input input;
  input.parse_input(argc, argv);
  Food food (input.philCount, input.foodCount);

  cout << input << endl;

  for (unsigned i = 0; i < input.philCount; i++)  {
    unsigned amountToEat;
    if (input.isUniform) {
      amountToEat = ceil((float)input.foodCount/input.philCount);
    } else {
      amountToEat = ceil(input.foodCount * (float)input.weights[i]/input.sumWeights);
    }
    cout << amountToEat << endl;
    philosophers.push_back(PhilPtr(new Philosopher(i, amountToEat)));
    threads.emplace_back(*(philosophers[i]), std::ref(food),
                         std::ref(*philosophers[i]));
  }

  for (unsigned i = 0; i < input.philCount; i++)
    threads[i].join();

  cout << "END!" << endl;

  for (unsigned i = 0; i < input.philCount; i++)
    (*philosophers[i]).print();

  return 0;
}

