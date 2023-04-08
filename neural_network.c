#include <time.h>
#include "value.h"
#include "neuron.h"

typedef struct Layer {
  Neuron **neurons;
  int nout;
  int nin;
} Layer;

typedef struct MLP {
  Layer **layers;
  int *sz;
  int nout;
} MLP;

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

MLP *create_mlp(int nin, int *nouts, int nout) {
  MLP *mlp = (MLP *)malloc(sizeof(MLP));
  mlp->nout = nout;
  mlp->sz = (int *)malloc((nout + 1) * sizeof(int));
  mlp->sz[0] = nin;
  for (int i = 0; i < nout; i++) {
    mlp->sz[i + 1] = nouts[i];
  }
  mlp->layers = (Layer **)malloc(nout * sizeof(Layer *));
  for (int i = 0; i < nout; i++) {
    mlp->layers[i] = create_layer(mlp->sz[i], mlp->sz[i + 1]);
  }
  return mlp;
}

void free_mlp(MLP *mlp) {
  for (int i = 0; i < mlp->nout; i++) 
    free_layer(mlp->layers[i]);
  free(mlp->layers);
  free(mlp->sz);
  free(mlp);
}

Value **layer_call(Layer *layer, Value **x) {
  Value **outs = (Value **)malloc(layer->nout * sizeof(Value *));
  for (int i = 0; i < layer->nout; i++) {
    outs[i] = neuron_call(layer->neurons[i], x);
  }
  return outs;
}

Value ***mlp_call(MLP *mlp, Value **x) {
  // 分配記憶體以保存每層的輸出
  Value ***layer_outputs = (Value ***)malloc(sizeof(Value **) * mlp->nout);

  Value **layer_input = x;
  for (int i = 0; i < mlp->nout; i++) {
    // 將當前層的輸出保存到陣列中
    layer_outputs[i] = layer_call(mlp->layers[i], layer_input);

    // 更新層輸入以供下一層使用
    layer_input = layer_outputs[i];
  }

  return layer_outputs;
}

void update_parameters(MLP *mlp, double learning_rate) {
  for (int i = 0; i < mlp->nout; i++) {
    Layer *layer = mlp->layers[i];
    for (int j = 0; j < layer->nout; j++) {
      Neuron *neuron = layer->neurons[j];
      for (int k = 0; k < neuron->nin; k++) {
        Value *p = neuron->w[k];
        p->grad = 0.0;
        backward_all(p);
        p->data -= learning_rate * p->grad;
      }
      neuron->b->grad = 0.0;
      backward_all(neuron->b);
      neuron->b->data -= learning_rate * neuron->b->grad;
    }
  }
}
/*
int main()
{
  srand(time(NULL));
  int nouts[] = {4, 4, 1};
  MLP *n = create_mlp(3, nouts, 3);
  // Define the input
  double xs[][3] = {{2.0, 3.0, -1.0}};

  // Prepare the input as Value objects
  Value *x[3];
  for (int i = 0; i < 3; i++) {
    x[i] = create_value(xs[0][i], NULL, 0, '\0', "");
  }

  // Calculate the layer output
  Value ***y_out = mlp_call(n, x);

  // Print the output
  printf("MLP output: %.8f\n", y_out[2][0]->data);
  printf("MLP output prev: %s\n", y_out[2][0]->prev[0]->label);

  // Free allocated memory
  for (int i = 0; i < 3; i++) {
    free(x[i]);
  }

  for (int i = 0; i < n->nout; i++) {
    for (int j = 0; j < n->layers[i]->nout; j++) 
       free(y_out[i][j]);
    free(y_out[i]);
  }
  free(y_out);

  free_mlp(n);
  return 0;
}
*/
/*
int main() {
  srand(time(NULL));
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
    free(x[i]);
  }

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
  free(layer->neurons);
  free(layer);

  return 0;
}
*/

int main() {
  srand(time(NULL));
  // Create a neuron with 2 inputs
  Neuron *neuron = create_neuron(2);

  // Define the input
  double xs[][2] = {{2.0, 3.0}};

  // Prepare the input as Value objects
  Value *x[2];
  for (int i = 0; i < 2; i++) {
    x[i] = create_value(xs[0][i], NULL, 0, '\0', "");
  }

  /*
  // Calculate the neuron output
  Value *y_out = neuron_call(neuron, x);
  */

  // Calculate the neuron output and keep track of allocated memory
  int allocated_count;
  Value **allocated_values = neuron_call(neuron, x, &allocated_count);
  Value *y_out = allocated_values[allocated_count - 1];

  // Print the output
  printf("Neuron output: %.8f\n", y_out->data);

  // Free allocated memory
  for (int i = 0; i < 2; i++) {
    free_value(x[i]);
  }
  //free_value(y_out);
  //free_neuron(neuron);
  free_allocated_values(allocated_values, allocated_count);
  free_neuron(neuron);
nt main() {
  srand(time(NULL));
  // Create a neuron with 2 inputs
  Neuron *neuron = create_neuron(2);

  // Define the input
  double xs[][2] = {{2.0, 3.0}};

  // Prepare the input as Value objects
  Value *x[2];
  for (int i = 0; i < 2; i++) {
    x[i] = create_value(xs[0][i], NULL, 0, '\0', "");
  }

  /*
  // Calculate the neuron output
  Value *y_out = neuron_call(neuron, x);
  */

  // Calculate the neuron output and keep track of allocated memory
  int allocated_count;
  Value **allocated_values = neuron_call(neuron, x, &allocated_count);
  Value *y_out = allocated_values[allocated_count - 1];

  // Print the output
  printf("Neuron output: %.8f\n", y_out->data);

  // Free allocated memory
  for (int i = 0; i < 2; i++) {
    free_value(x[i]);
  }
  //free_value(y_out);
  //free_neuron(neuron);
  free_allocated_values(allocated_values, allocated_count);
  free_neuron(neuron);


  return 0;
}

/*
int main() {
  srand(time(NULL));

  int nouts[] = {4, 4, 1};
  MLP *n = create_mlp(3, nouts, 3);

  double xs[][3] = {
      {2.0, 3.0, -1.0}, {3.0, -1.0, 0.5}, {0.5, 1.0, 1.0}, {1.0, 1.0, -1.0}};
  double ys[] = {1.0, -1.0, -1.0, 1.0};

  for (int k = 0; k < 20; k++) {
    double loss = 0.0;
    for (int i = 0; i < 4; i++) {
      Value **x = (Value **)malloc(3 * sizeof(Value *));
      for (int j = 0; j < 3; j++) {
        x[j] = create_value(xs[i][j], NULL, 0, '\0', "");
      }
      Value **ypred = mlp_call(n, x);
      Value *loss_i = pow_v(add(ypred[i], create_value(-ys[i], NULL, 0, '\0', "")), 2.0);
      backward_all(loss_i);
      update_parameters(n, 0.1);
      
      for (int j = 0; j < 3; j++) {
        free(x[j]);
      }
      free(ypred);
      free(loss_i);
      loss += loss_i->data;
    }
    printf("%d %.8f\n", k, loss);
  }

  // Free allocated memory
  for (int i = 0; i < n->nout; i++) {
    Layer *layer = n->layers[i];
    for (int j = 0; j < layer->nout; j++) {
      Neuron *neuron = layer->neurons[j];
      for (int k = 0; k < neuron->nin; k++) {
        free(neuron->w[k]);
      }
      free(neuron->w);
      free(neuron->b);
      free(neuron);
    }
    free(layer->neurons);
    free(layer);
  }
  free(n->layers);
  free(n);

  return 0;
}
*/
