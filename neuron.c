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
  neuron->allocated_count = neuron->nin * 2 + 1;
  neuron->allocated_values = (Value **)malloc(neuron->allocated_count * sizeof(Value *));
  return neuron;
}

void free_neuron(Neuron *neuron) {
  free(neuron->b);
  for (int i = 0; i < neuron->nin; i++)
    free(neuron->w[i]);
  free(neuron->w);
  for (int i = 0; i < neuron->allocated_count; i++)
    free_value(neuron->allocated_values[i]);
  free(neuron->allocated_values);
  free(neuron);
}

void neuron_call(Neuron *neuron, Value **x) {
  int idx = 0;
  Value *act = neuron->b;
  for (int i = 0; i < neuron->nin; i++) {
    Value *wi_xi = mul(neuron->w[i], x[i]);
    neuron->allocated_values[idx++] = wi_xi;
    act = add(act, wi_xi);
    neuron->allocated_values[idx++] = act;
  }
  neuron->allocated_values[idx++] = tanh_v(act);
}

/*
void free_neuron_allocated_values(Value **neuron_allocated_values, int neuron_allocated_count) {
  for (int i = 0; i < neuron_allocated_count; i++) {
    free_value(neuron_allocated_values[i]);
  }
  free(neuron_allocated_values);
}
*/
