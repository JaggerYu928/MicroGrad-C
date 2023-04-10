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
  free(neuron->w);
  free(neuron);
}

Value *neuron_call(Neuron *neuron, Value **x) {
  Value *act = neuron->b;
  for (int i = 0; i < neuron->nin; i++) {
    Value *wi_xi = mul(neuron->w[i], x[i]);
    act = add(act, wi_xi);
  }
  return tanh_v(act);
}

