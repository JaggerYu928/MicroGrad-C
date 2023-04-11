#include "layer.h"

Layer *create_layer(int nin, int nout) {
  Layer *layer = (Layer *)malloc(sizeof(Layer));
  layer->nin = nin;
  layer->nout = nout;
  layer->neurons = (Neuron **)malloc(nout * sizeof(Neuron *));
  for (int i = 0; i < nout; i++) {
    layer->neurons[i] = create_neuron(nin);
  }
  return layer;
}

void free_layer(Layer *layer) {
  for (int i = 0; i < layer->nout; i++)
    free_neuron(layer->neurons[i]);
  free(layer->neurons);
  free(layer);
}

Value **layer_call(Layer *layer, Value **x) {
  Value **out = (Value **)malloc(layer->nout * sizeof(Value *));
  for (int i = 0; i < layer->nout; i++) {
    out[i] =  neuron_call(layer->neurons[i], x);
  }
  return out;
}

