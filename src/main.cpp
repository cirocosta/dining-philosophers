#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <cstdlib>

sem_t mutex_count;
unsigned long counter = 0;

// a cv must have a 'friend' mutex variable.
pthread_cond_t cv;
pthread_mutex_t cv_mutex;

typedef struct char_print_params
{
  unsigned int tid;
} Params;

void* func (void* params)
{
  Params* p = (Params*) params;
  unsigned int i = 1000;

  sem_wait(&mutex_count);
  printf("THREAD %d in!\n", p->tid);
  while (i --> 0)
    counter++;
  sem_post(&mutex_count);

  pthread_exit(NULL);
}

int main()
{
  int N = 10;
  int i;

  Params* thread_params = (Params*)malloc(sizeof(*thread_params)*N);
  pthread_t* threads = (pthread_t*)malloc(sizeof(*threads)*N);
  pthread_cond_init(&cv, NULL);
  pthread_attr_t attr;

  sem_init(&mutex_count, 0, 1);
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  i = N;
  while (i --> 0) {
    thread_params[i].tid = i;
    pthread_create(&threads[i], &attr, &func, (void*)&thread_params[i]);
  }

  pthread_attr_destroy(&attr);

  while (N --> 0)
    pthread_join(threads[N], NULL);

  printf("Counter: %lu\n", counter);

  free(threads);
  free(thread_params);

  return 0;
}
