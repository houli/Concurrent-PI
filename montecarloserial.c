#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int is_in_circle(double x, double y) {
  double distance = x * x + y * y;

  if (distance > 1.0) {
    return 0;
  } else {
    return 1;
  }
}

int main(int argc, char * argv[]) {
  int simulations = atoi(argv[1]);
  int in_circle = 0;
  for (int i = 0; i < simulations; i++) {
    double x = (double) rand() / RAND_MAX;
    double y = (double) rand() / RAND_MAX;

    if (is_in_circle(x, y)) {
      in_circle++;
    }
  }
  double pi = ((double) in_circle / simulations) * 4.0;
  printf("%.10f\n", pi);

  return 0;
}
