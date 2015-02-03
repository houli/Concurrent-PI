#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int is_in_circle(double x, double y) {
  double distance = x * x + y * y;

  if (distance > 1.0) {
    return 0;
  } else {
    return 1;
  }
}

void *simulate(void *arg) {
  struct drand48_data rbuffer;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand48_r(tv.tv_usec, &rbuffer); // seed thread-safe PRNG

  long int simulations = (long int) arg;
  long int count = 0;

  double x, y;
  for (int i = 0; i < simulations; i++) {
    drand48_r(&rbuffer, &x);
    drand48_r(&rbuffer, &y);

    if (is_in_circle(x, y)) {
      count++;
    }
  }
  return (void*) count;
}

int main(int argc, char * argv[]) {
  long int total = 0;
  void* ret;

  int simulations = atoi(argv[1]);
  int num_threads = atoi(argv[2]);

  pthread_t threads[num_threads];

  int rc;
  long int simulations_per_thread = simulations / num_threads;
  long int simulations_last_thread = simulations_per_thread + simulations % num_threads;

  for (int i = 0; i < num_threads; i++) {
    if (i == num_threads - 1) {
      rc = pthread_create(&threads[i], NULL, simulate, (void *) simulations_last_thread);
    } else {
      rc = pthread_create(&threads[i], NULL, simulate, (void *) simulations_per_thread);
    }

    if (rc) {
      printf("ERROR return code from pthread_create(): %d\n", rc);
      exit(-1);
    }
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], &ret);
    total += (long int) ret;
  }

  double pi = ((double) total / simulations) * 4.0;
  printf("%.10f\n", pi);

  return 0;
}
