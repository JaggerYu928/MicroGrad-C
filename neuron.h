#ifndef _NEURON_H
#define _NEURON_H
#include "value.h"

typedef struct Neuron {
  Value **w;
  Value *b;
  int nin;
} Neuron;

Neuron *create_neuron(int nin);
void free_neuron(Neuron *neuron);
Value **neuron_call(Neuron *neuron, Value **x, int *allocated_count);
void free_allocated_values(Value **allocated_values, int allocated_count);

#endif
