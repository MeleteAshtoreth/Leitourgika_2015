#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

#define NLOOPS 100000

sem_t s0, s1, s2, s3;

void *
process_A (void *unused)
{
  int n = NLOOPS;

  while (n--)
    {
      sem_wait (&s0);
      putchar ('A');
      sem_post (&s1);
      putchar ('B');
      sem_post (&s3);
      sem_post (&s2);

      sem_wait (&s0);
      putchar ('A');
      sem_post (&s1);
      sem_wait (&s3);
      putchar ('B');
      sem_post (&s2);
    }

  return 0;
}

void *
process_B (void *unused)
{
  int n = NLOOPS;

  while (n--)
    {
      sem_wait (&s1);
      sem_wait (&s3);
      putchar ('C');
      sem_wait (&s2);
      putchar ('D');
      sem_post (&s0);

      sem_wait (&s1);
      putchar ('C');
      sem_post (&s3);
      sem_wait (&s2);
      putchar ('D');
      sem_post (&s0);
    }

  return 0;
}


int
main ()
{
  pthread_t a, b;

  sem_init (&s0, 0, 1);
  sem_init (&s1, 0, 0);
  sem_init (&s2, 0, 0);
  sem_init (&s3, 0, 0);

  pthread_create (&a, 0, process_A, 0);
  pthread_create (&b, 0, process_B, 0);

  pthread_join (a, 0);
  pthread_join (b, 0);

  putchar ('\n');
  return 0;
}