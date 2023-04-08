#ifndef _LAYER_H
#define _LAYER_H
#include "neuron.h"

typedef struct Layer {
  Neuron **neurons;
  int nout;
  int nin;
} Layer;

Layer *create_layer(int nin, int nout);
void free_layer(Layer *layer);
void layer_call(Layer *layer, Value **x);
#endif
