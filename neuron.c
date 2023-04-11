#include "neuron.h"

Neuron *create_neuron(int nin) {
  Neuron *neuron = (Neuron *)malloc(sizeof(Neuron));
  neuron->nin = nin;
  neuron->w = (Value **)malloc(nin * sizeof(Value *));
  for (int i = 0; i < nin; i++) {
    //neuron->w[i] = create_value(rand_uniform(-1, 1), NULL, 0, '\0', "");
    neuron->w[i] = create_value(0.12, NULL, 0, '\0', "");
  }
  //neuron->w[0] = create_value(-3.0, NULL, 0, '\0', "");
  //neuron->w[1] = create_value(1.0, NULL, 0, '\0', "");
  //neuron->b = create_value(rand_uniform(-1, 1), NULL, 0, '\0', "");
  //neuron->b = create_value(6.8813735870195432, NULL, 0, '\0', "");
  neuron->b = create_value(0.3, NULL, 0, '\0', "");
  return neuron;
}

void free_neuron(Neuron *neuron) {
  for(int i = 0; i < neuron->nin; i++)
    free(neuron->w[i]);
  free(neuron->w);
  free(neuron->b);
  free(neuron);
}

Value *neuron_call(Neuron *neuron, Value **x) {
  Value *act = mul(neuron->w[0], x[0]);
  for (int i = 1; i < neuron->nin; i++) {
    Value *wi_xi = mul(neuron->w[i], x[i]);
    act = add(act, wi_xi);
  }
  act = add(act, neuron->b);
  return tanh_v(act);
}

