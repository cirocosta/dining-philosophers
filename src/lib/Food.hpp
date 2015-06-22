#ifndef FOOD_HPP
#define FOOD_HPP

#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>

typedef std::unique_ptr<std::condition_variable> CvPtr;

class Food
{
  std::mutex m_EntryMutex;
  std::vector<unsigned> m_Forks;
  std::vector<CvPtr> m_ForksCv;
  unsigned m_ForksN;
  unsigned m_Amount;
public:
  Food(unsigned forks, unsigned amount);
  ~Food() {}

  void acquireForksOdd (unsigned id);
  void acquireForks (unsigned id);
  void eat (unsigned id);
  void releaseForks (unsigned id);
  bool hasFood ();
private:
  void wait(unsigned id, std::unique_lock<std::mutex>& lk);
  void signal(unsigned id);
};

#endif
