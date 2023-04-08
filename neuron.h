#ifndef _NEURON_H
#define _NEURON_H
#include "value.h"

typedef struct Neuron {
  Value **w;
  Value *b;
  int nin;
  Value **allocated_values;
  int allocated_count;
} Neuron;

Neuron *create_neuron(int nin);
void free_neuron(Neuron *neuron);
//Value **neuron_call(Neuron *neuron, Value **x, int *neuron_allocated_count);
void neuron_call(Neuron *neuron, Value **x);
//void free_neuron_allocated_values(Value **neuron_allocated_values, int neuron_allocated_count);

#endif
