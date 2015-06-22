#include "Philosopher.hpp"

std::mt19937_64 eng{std::random_device{}()};

Philosopher::Philosopher (unsigned id, unsigned amountToEat) :
    m_id(id), m_eaten(0), m_amountToEat(amountToEat)
{
  m_isOdd = id % 2 != 0;
}


void Philosopher::think()
{
  std::uniform_int_distribution<> dist{10, 80};
  std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
}

void Philosopher::operator()(Food& food, Philosopher& phil)
{
  while (food.hasFood()) {
    if (phil.hungry()) {
      if (phil.m_isOdd)
        food.acquireForksOdd(phil.m_id);
      else
        food.acquireForks(phil.m_id);
      food.eat(phil.m_id);
      phil.m_amountToEat--;
      food.releaseForks(phil.m_id);
      phil.m_eaten++;

      think();
    }
  }
}

void Philosopher::print()
{
  printf("P%d,%d\n", m_id, m_eaten);
}
