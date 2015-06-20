#include "Monitor.hpp"

Monitor::Monitor(unsigned phil)
  : m_PhilCount(phil)
{
  m_Forks[phil] = new bool[phil];

  while (phil --> 0)
    m_Forks[phil] = false;
  sem_init(&m_Entrance, 0, 1);
}

Monitor::~Monitor()
{
  delete[] m_Forks;
  sem_destroy(&m_Entrance);
}

/**
 * libera a exclusão mútua do monitor e envia o
 * processo para a fila da variável de condição
 * 'cv' até que ele acorde com um signal.
 *
 * - Cada 'cv' possui uma fila cvDelay
 * - Cada cv possui um contador, cvN
 * - cvDelay inicia vazia (cvN = 0)
 *
 *  wait(cv) implica em incremento em cvN e
 *  adição do descritor do processo para o final
 *  da fila cvDelay.
 */
void Monitor::wait(unsigned id)
{
}

void Monitor::signal(unsigned id)
{
}

void Monitor::pick(unsigned id)
{
  while (!m_Forks[id])
    wait(id);
  m_Forks[id] = true;

  while (!m_Forks[(id+1) % m_PhilCount])
    wait((id+1) % m_PhilCount);
  m_Forks[(id+1) % m_PhilCount] = true;
}

void Monitor::release(unsigned id)
{
  m_Forks[id] = false;
  signal(id);
  m_Forks[(id+1) % m_PhilCount] = false;
  signal((id+1) % m_PhilCount);
}
