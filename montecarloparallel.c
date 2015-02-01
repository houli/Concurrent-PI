#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct t_struct {
  int index;
  int num_simulations;
  unsigned int* seeds;
};

#define SIMULATIONS 100000000

int is_in_circle(double x, double y) {
  double distance = sqrt((x * x) + (y * y));

  if (distance > 1.0) {
    return 0;
  } else {
    return 1;
  }
}

void *calculate(void *arg) {
  struct t_struct* info = (struct t_struct *) arg;
  int simulations = info->num_simulations;
  long int count = 0;
  for (int i = 0; i < simulations; i++) {
    double x = (double) rand_r(&info->seeds[info->index]) / RAND_MAX;
    double y = (double) rand_r(&info->seeds[info->index]) / RAND_MAX;

    if (is_in_circle(x, y)) {
      count++;
    }
  }
  return (void*) count;
}

int main() {
  long int total = 0;
  void* ret;
  int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);

  unsigned int thread_seeds[num_cpus];
  for (int i = 0; i < num_cpus; i++) {
    thread_seeds[i] = rand();
  }

  pthread_t threads[num_cpus];

  int rc;
  int simulations_per_cpu = SIMULATIONS / num_cpus;
  int simulations_last_cpu = simulations_per_cpu + SIMULATIONS % num_cpus;

  for (int i = 0; i < num_cpus; i++) {
    struct t_struct* info = malloc(sizeof(struct t_struct));
    info->index = i;
    info->seeds = thread_seeds;

    if (i == num_cpus - 1) {
      info->num_simulations = simulations_last_cpu;
    } else {
      info->num_simulations = simulations_per_cpu;
    }

    rc = pthread_create(&threads[i], NULL, calculate,(void *) info);

    if (rc) {
      printf("ERROR return code from pthread_create(): %d\n", rc);
      exit(-1);
    }
    free(info);
  }

  for (int i = 0; i < num_cpus; i++) {
    pthread_join(threads[i], &ret);
    total += (long int) ret;
  }

  double pi = ((double) total / SIMULATIONS) * 4.0;
  printf("%.10f\n", pi);

  return 0;
}
