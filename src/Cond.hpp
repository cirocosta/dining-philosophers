#ifndef COND_H
#define COND_H

#include <queue>
#include <semaphore.h>
#include <utility>

class Cond
{
  sem_t* m_Priv;
  std::queue<std::pair<unsigned int, sem_t*>> m_Delay;
public:
    Cond();
    ~Cond();
};

#endif
