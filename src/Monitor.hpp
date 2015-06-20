#ifndef MONITOR_H
#define MONITOR_H

#include <semaphore.h>

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
  sem_t m_Entrance;
public:
  /**
   * Must establish the invariant that procedures
   * will have to maintain.
   */
  Monitor ();
  ~Monitor();

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
  void wait(void* cv);

  /**
   * Awakens the process at the front of the
   * queue (cv).
   */
  void signal(void* cv);
};

#endif
