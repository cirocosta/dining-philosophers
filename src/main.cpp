#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <random>
#include <chrono>

using std::cout;
using std::endl;

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
    std::unique_lock<std::mutex> guard (m_EntryMutex);

    while (m_Forks[(id+1) % m_ForksN]) {
      m_ForksCv[(id+1) % m_ForksN]->wait(guard,[=]{
        return m_Forks[(id+1) % m_ForksN] == 0;
      });
    }
    m_Forks[(id+1) % m_ForksN] = 1;

    while (m_Forks[id]) {
      m_ForksCv[id]->wait(guard,[=]{
        return m_Forks[id] == 0;
      });
    }
    m_Forks[id] = 1;

    printf("P%d\tacquired\n", id);
  }


  void acquireForks (unsigned id)
  {
    std::unique_lock<std::mutex> guard (m_EntryMutex);

    while (m_Forks[id]) {
      m_ForksCv[id]->wait(guard,[=]{
        return m_Forks[id] == 0;
      });
    }
    m_Forks[id] = 1;

    while (m_Forks[(id+1) % m_ForksN]) {
      m_ForksCv[(id+1) % m_ForksN]->wait(guard,[=]{
        return m_Forks[(id+1) % m_ForksN] == 0;
      });
    }
    m_Forks[(id+1) % m_ForksN] = 1;

    printf("P%d\tacquired\n", id);
  }

  void eat (unsigned id)
  {
    auto start = std::chrono::steady_clock::now();
    std::unique_lock<std::mutex> guard (m_EntryMutex);

    if (m_Amount > 0)
      m_Amount--;
    guard.unlock();

    printf("P%d\t\teat:%ld\n", id, start.time_since_epoch().count());
  }

  void releaseFork (unsigned id)
  {
    std::unique_lock<std::mutex> lk (m_EntryMutex);

    m_Forks[id] = 0;
    m_ForksCv[id]->notify_all();
    m_Forks[(id+1) % m_ForksN] = 0;
    m_ForksCv[(id+1) % m_ForksN]->notify_all();

    printf("P%d\treleased\n", id);
  }

  bool hasFood ()
  {
    std::lock_guard<std::mutex> guard (m_EntryMutex);
    return m_Amount > 0;
  }
private:
  void wait(unsigned i) {}
  void signal(unsigned i){}
};

Food food (10, 100);
std::mt19937_64 eng{std::random_device{}()};

void philosopher(unsigned i)
{
  while (food.hasFood()) {
    if (i%2)
      food.acquireForks(i);
    else
      food.acquireForksOdd(i);
    std::uniform_int_distribution<> dist{50, 100};
    std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
    food.eat(i);
    food.releaseFork(i);
  }
}

int main(const int argc, const char** argv)
{
  std::vector<std::thread> threads;

  for (unsigned i = 0; i < 5; i++) {
      threads.push_back(std::thread(philosopher, i));
  }

  for (unsigned i = 0; i < 5; i++)
    threads[i].join();

  return 0;
}
