#include <iostream>
#include <thread>
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <random>
#include <chrono>
#include "./lib/Input.hpp"

using std::cout;
using std::endl;
using std::cerr;

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

    while (m_Forks[(id+1) % m_ForksN]) {
      m_ForksCv[(id+1) % m_ForksN]->wait(lk,[=]{
        return m_Forks[(id+1) % m_ForksN] == 0;
      });
    }
    m_Forks[(id+1) % m_ForksN] = 1;

    while (m_Forks[id]) {
      m_ForksCv[id]->wait(lk,[=]{
        return m_Forks[id] == 0;
      });
    }
    m_Forks[id] = 1;
    lk.unlock();

    printf("P%d\tacquired\n", id);
  }

  void acquireForks (unsigned id)
  {
    std::unique_lock<std::mutex> lk (m_EntryMutex);

    while (m_Forks[id]) {
      m_ForksCv[id]->wait(lk,[=]{
        return m_Forks[id] == 0;
      });
    }
    m_Forks[id] = 1;

    while (m_Forks[(id+1) % m_ForksN]) {
      m_ForksCv[(id+1) % m_ForksN]->wait(lk,[=]{
        return m_Forks[(id+1) % m_ForksN] == 0;
      });
    }
    m_Forks[(id+1) % m_ForksN] = 1;
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
    std::unique_lock<std::mutex> lk (m_EntryMutex);

    m_Forks[id] = 0;
    m_ForksCv[id]->notify_all();
    m_Forks[(id+1) % m_ForksN] = 0;
    m_ForksCv[(id+1) % m_ForksN]->notify_all();
    lk.unlock();

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

std::mt19937_64 eng{std::random_device{}()};

class Philosopher
{
protected:
  unsigned m_id;
  unsigned m_eaten;
  bool m_isOdd;
  unsigned m_amountToEat;
public:
  Philosopher (unsigned id, unsigned amountToEat) :
    m_id(id), m_eaten(0), m_amountToEat(amountToEat)
  {
    m_isOdd = id % 2 != 0;
  }

  inline bool hungry () { return m_amountToEat > 0; }

  void operator()(Food& food, Philosopher& phil)
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
  void think()
  {
    std::uniform_int_distribution<> dist{100, 300};
    std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
  }

  friend std::ostream& operator<<(std::ostream& os, const Philosopher& phil)
  {
    os << "P" << phil.m_id << "," << phil.m_eaten;
    return os;
  }
};


typedef std::unique_ptr<Philosopher> PhilPtr;
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
    cout << (*philosophers[i]) << endl;

  return 0;
}

