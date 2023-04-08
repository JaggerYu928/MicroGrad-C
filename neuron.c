#include "neuron.h"

Neuron *create_neuron(int nin) {
  Neuron *neuron = (Neuron *)malloc(sizeof(Neuron));
  neuron->nin = nin;
  neuron->w = (Value **)malloc(nin * sizeof(Value *));
  for (int i = 0; i < nin; i++) {
    //neuron->w[i] = create_value(rand_uniform(-1, 1), NULL, 0, '\0', "");
    neuron->w[i] = create_value(0.12, NULL, 0, '\0', "");
  }
  //neuron->b = create_value(rand_uniform(-1, 1), NULL, 0, '\0', "");
  neuron->b = create_value(0.3, NULL, 0, '\0', "");
  return neuron;
}

void free_neuron(Neuron *neuron) {
  //free(neuron->b);
  for (int i = 0; i < neuron->nin; i++)
    free(neuron->w[i]);
  free(neuron->w);
  free(neuron);
}

Value **neuron_call(Neuron *neuron, Value **x, int *allocated_count) {
  *allocated_count = neuron->nin * 2 + 1;
  Value **allocated_values = (Value **)malloc(*allocated_count * sizeof(Value *));
  int idx = 0;

  Value *act = neuron->b;
  for (int i = 0; i < neuron->nin; i++) {
    Value *wi_xi = mul(neuron->w[i], x[i]);
    allocated_values[idx++] = wi_xi;
    Value *temp = act;
    act = add(act, wi_xi);
    allocated_values[idx++] = act;
  }
  allocated_values[idx++] = tanh_v(act);
  return allocated_values;
}

void free_allocated_values(Value **allocated_values, int allocated_count) {
  for (int i = 0; i < allocated_count; i++) {
    free_value(allocated_values[i]);
  }
  free(allocated_values);
}






