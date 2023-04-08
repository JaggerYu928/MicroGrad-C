#include "../layer.h"

int main() {
  // srand(time(NULL));
  // Create a layer with 2 inputs and 3 outputs
  Layer *layer = create_layer(2, 3);

  // Define the input
  double xs[][2] = {{2.0, 3.0}};

  // Prepare the input as Value objects
  Value *x[2];
  for (int i = 0; i < 2; i++) {
    x[i] = create_value(xs[0][i], NULL, 0, '\0', "");
  }

  // Calculate the layer output
  layer_call(layer, x);

  // Print the output
  printf("Layer output:\n");
  for (int i = 0; i < layer->nout; i++) {
    printf("Output %d: %.8f\n", i, layer->neurons[i]->allocated_values[layer->neurons[i]->allocated_count - 1]->data);
  }

  // Free allocated memory
  for (int i = 0; i < 2; i++) {
    free_value(x[i]);
  }
  free_layer(layer);

  return 0;
}
