#ifndef PHILOSOPHER_HPP
#define PHILOSOPHER_HPP

#include "Forks.hpp"

#include <chrono>
#include <random>
#include <thread>

std::mt19937_64 eng{std::random_device{}()};

class EvenPhilosopher;
class OddPhilosopher;

class Philosopher
{
protected:
  unsigned m_id;
  unsigned m_eaten;
  unsigned m_weight;
public:
  void think()
  {
    std::uniform_int_distribution<> dist{50, 100};
    std::this_thread::sleep_for(
      std::chrono::milliseconds{dist(eng)}
    );
  }

  Philosopher generate (unsigned i)
  {
    EvenPhilosopher phil (i);

    return phil;
  }
};

class EvenPhilosopher: public Philosopher
{
public:
  EvenPhilosopher(){}

  void operator()(Forks& forks)
  {
    forks.acquireForks(m_id);
  }
};

class OddPhilosopher : public Philosopher
{
  OddPhilosopher(){}

  void operator()(Forks& forks)
  {
    forks.acquireForksOdd(m_id);
  }
};


#endif
