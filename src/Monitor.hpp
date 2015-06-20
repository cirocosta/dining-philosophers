#ifndef MONITOR_H
#define MONITOR_H

#include <semaphore.h>
#include <pthread.h>
#include <queue>

/**
 * A Monitor implementation using * semaphores.
 *
 * We must ensure that everytime a procedure is
 * called there will be only one process
 * executing a Monitor's procedure.
 *
 * Whenever the procedure ends the entrance must
 * be released.
 *
 * Mutual exclusion is handled implicitly while
 * condition synchronization explicitly through
 * the use of condition variables, which
 * represents a queue of deleyed processes.
 */
class Monitor
{
  // any process that wants to execute a
  // procedure from our monitor must first go to
  // the monitor queue and then wait there till
  // its time.
  std::queue<unsigned> m_EntryQueue;
  bool* m_Forks;
  sem_t m_Entrance;
  pthread_cond_t* m_ForksCv;
  unsigned m_PhilCount;
public:
  /**
   * Must establish the invariant that procedures
   * will have to maintain.
   */
  Monitor(unsigned philosophers);
  ~Monitor();
private:
  /**
   * Verifies whether the queue is empty.
   */
  bool empty(void* cv);

  /**
   * Blocks the process in a condition. Causes
   * the executin process to delay at the rear of
   * the cv's queue and relinquish exclusive
   * access to the monitor.
   */
  void wait(unsigned id);

  /**
   * Awakens the process at the front of the
   * queue (cv).
   */
  void signal(unsigned id);
public:
  void pick(unsigned id);
  void pickOdd(unsigned id);

  void release(unsigned id);
  void releaseOdd(unsigned id);
};

#endif
