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
  Food(unsigned forks, unsigned amount) :
    m_ForksN(forks), m_Amount(amount)
  {
    for (unsigned i = 0; i < forks; i++) {
      m_Forks.push_back(0);
      m_ForksCv.push_back(CvPtr(new std::condition_variable()));
    }
  }

  ~Food() {}

  void acquireForksOdd (unsigned id)
  {
    std::unique_lock<std::mutex> lk (m_EntryMutex);
    unsigned id1 = (id+1)%m_ForksN;

    while (m_Forks[id1])
      wait(id1, lk);
    m_Forks[id1] = 1;

    while (m_Forks[id])
      wait(id, lk);
    m_Forks[id] = 1;
    lk.unlock();

    printf("P%d\tacquired\n", id);
  }

  void acquireForks (unsigned id)
  {
    std::unique_lock<std::mutex> lk (m_EntryMutex);
    unsigned id1 = (id+1)%m_ForksN;

    while (m_Forks[id])
      wait(id, lk);
    m_Forks[id] = 1;

    while (m_Forks[id1])
      wait(id1, lk);
    m_Forks[id1] = 1;
    lk.unlock();

    printf("P%d\tacquired\n", id);
  }

  void eat (unsigned id)
  {
    auto start = std::chrono::steady_clock::now();
    std::unique_lock<std::mutex> lk (m_EntryMutex);

    if (m_Amount > 0)
      m_Amount--;
    lk.unlock();

    auto finish = std::chrono::steady_clock::now();

    printf("P%d\teat:\tstart:%ld\tfinish:%ld\n",
      id, start.time_since_epoch().count(),
          finish.time_since_epoch().count());
  }

  void releaseForks (unsigned id)
  {
    unsigned id1 = (id+1) % m_ForksN;
    std::unique_lock<std::mutex> lk (m_EntryMutex);

    m_Forks[id] = 0;
    signal(id);
    m_Forks[id1] = 0;
    signal(id1);
    lk.unlock();

    printf("P%d\treleased\n", id);
  }

  bool hasFood ()
  {
    std::lock_guard<std::mutex> guard (m_EntryMutex);
    return m_Amount > 0;
  }
private:
  void wait(unsigned id, std::unique_lock<std::mutex>& lk) {
    m_ForksCv[id]->wait(lk,[=]{
      return m_Forks[id] == 0;
    });
  }

  void signal(unsigned id){
    m_ForksCv[id]->notify_all();
  }
};


#endif
