#include <time.h>
#include "layer.h"

typedef struct MLP {
  Layer **layers;
  int *sz;
  int nout;
} MLP;

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

Value ***mlp_call(MLP *mlp, Value **x) {
  // 分配記憶體以保存每層的輸出
  Value ***layer_outputs = (Value ***)malloc(sizeof(Value **) * mlp->nout);

  Value **layer_input = x;
  for (int i = 0; i < mlp->nout; i++) {
    // 將當前層的輸出保存到陣列中
    layer_call(mlp->layers[i], layer_input);
    layer_outputs[i] = (Value **)malloc(sizeof(Value *) * mlp->layers[i]->nout);

    for (int j = 0; j < mlp->layers[i]->nout; j++) {
      layer_outputs[i][j] =  mlp->layers[i]->neurons[j]->allocated_values[mlp->layers[i]->neurons[j]->allocated_count - 1];
    }

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

  // Free allocated memory
  for (int i = 0; i < 3; i++) {
    free(x[i]);
  }

  for (int i = 0; i < n->nout; i++) {
    free(y_out[i]);
  }
  free(y_out);

  free_mlp(n);
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
