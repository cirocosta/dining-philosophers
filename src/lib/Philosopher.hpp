#ifndef PHILOSOPHER_HPP
#define PHILOSOPHER_HPP

#include "Food.hpp"

#include <chrono>
#include <random>
#include <thread>
#include <memory>

class Philosopher;
typedef std::unique_ptr<Philosopher> PhilPtr;

class Philosopher
{
protected:
  unsigned m_id;
  unsigned m_eaten;
  bool m_isOdd;
  unsigned m_amountToEat;
public:
  Philosopher (unsigned id, unsigned amountToEat);

  void operator()(Food& food, Philosopher& phil);
  void think();
  void print();

  inline bool hungry () { return m_amountToEat > 0; }
};

#endif
