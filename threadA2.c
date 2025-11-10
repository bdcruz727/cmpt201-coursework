#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int cnt = 0;

void *thread_function(void *arg) {
  for (int i = 0; i < 10000000; i++) {
    cnt++;
  }
  return NULL;
}

int main(void) {
  pthread_t t1;
  pthread_t t2;

  int t1S;
  int t2S;

  t1S = pthread_create(&t1, NULL, thread_function, NULL);

  t2S = pthread_create(&t2, NULL, thread_function, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("Count: %d\n", cnt);
}
