#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_INPUTS 3
#define NUM_HIDDEN 1
#define NUM_OUTPUTS 1
#define NUM_SAMPLES 4
#define NUM_ITERATIONS 20

double random_uniform(double min, double max) {
    return (double)rand() / RAND_MAX * (max - min) + min;
}

void tanh_activation(double input, double *output, double *grad) {
    double ex = exp(2 * input);
    *output = (ex - 1) / (ex + 1);
    *grad = 1 - pow(*output, 2);
}



int main()
{

	return 0;
}
