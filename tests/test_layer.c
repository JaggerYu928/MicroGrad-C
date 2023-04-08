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
  Value **y_out = layer_call(layer, x);

  // Print the output
  printf("Layer output:\n");
  for (int i = 0; i < layer->nout; i++) {
    printf("Output %d: %.8f\n", i, y_out[i]->data);
  }

  // Free allocated memory
  for (int i = 0; i < 2; i++) {
    free_value(x[i]);
  }
/*
  for (int i = 0; i < layer->nout; i++) {
    free(y_out[i]);
  }
  free(y_out);

  for (int i = 0; i < layer->nout; i++) {
    for (int j = 0; j < layer->neurons[i]->nin; j++) {
      free(layer->neurons[i]->w[j]);
    }
    free(layer->neurons[i]->w);
    free(layer->neurons[i]->b);
    free(layer->neurons[i]);
  }
*/
  free(y_out);
  for (int i = 0; i < layer->nout; i++) {
    free_neuron(layer->neurons[i]);
  }
  free(layer->neurons);
  free(layer);

  return 0;
}
