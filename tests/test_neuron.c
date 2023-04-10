#include "../value.h"
#include "../neuron.h"

int main() {
  // srand(time(NULL));
  // Create a neuron with 2 inputs
  Neuron *neuron = create_neuron(2);

  // Define the input
  double xs[][2] = {{2.0, 3.0}};

  // Prepare the input as Value objects
  Value *x[2];
  for (int i = 0; i < 2; i++) {
    x[i] = create_value(xs[0][i], NULL, 0, '\0', "");
  }

  // Calculate the neuron output and keep track of allocated memory
  Value *y_out = neuron_call(neuron, x);

  // Print the output
  printf("Neuron output: %.8f\n", y_out->data);

  // Free allocated memory
  free_value(y_out);
  free_neuron(neuron);
  return 0;
}

