#include "../value.h"
#include "../neuron.h"

int main() {
  // srand(time(NULL));
  // Create a neuron with 2 inputs
  Neuron *neuron = create_neuron(2);

  // Define the input
  double xs[][2] = {{2.0, 0.0}};

  // Prepare the input as Value objects
  Value *x[2];
  for (int i = 0; i < 2; i++) {
    x[i] = create_value(xs[0][i], NULL, 0, '\0', "");
  }

  // Calculate the neuron output and keep track of allocated memory
  Value *o = neuron_call(neuron, x);
 
  o->grad = 1;
  // Print the output
  printf("o.data: %.8f\n", o->data);

  backward_all(o);
  //backward_tanh(o);

  Value *n = o->prev[0]; 
  printf("n.grad: %.8f\n", n->grad);
  printf("n.data: %.8f\n", n->data);

  //backward_add(n);

  Value *x1w1x2w2 = n->prev[0];
  Value *b = n->prev[1];

  printf("x1w1x2w2.grad: %.8f\n", x1w1x2w2->grad);
  printf("x1w1x2w2.data: %.8f\n", x1w1x2w2->data);
  printf("b.grad: %.8f\n", b->grad);
  printf("b.data: %.8f\n", b->data);

  //backward_add(x1w1x2w2);

  Value *x1w1 = x1w1x2w2->prev[0];
  Value *x2w2 = x1w1x2w2->prev[1];
  
  printf("x1w1.grad: %.8f\n", x1w1->grad);
  printf("x1w1.data: %.8f\n", x1w1->data);
  printf("x2w2.grad: %.8f\n", x2w2->grad);
  printf("x2w2.data: %.8f\n", x2w2->data);

  //backward_mul(x2w2);

  Value *w2 = x2w2->prev[0];
  Value *x2 = x2w2->prev[1];

  //backward_mul(x1w1);

  Value *w1 = x1w1->prev[0];
  Value *x1 = x1w1->prev[1];

  printf("x1.grad: %.8f\n", x1->grad);
  printf("x1.data: %.8f\n", x1->data);
  printf("w1.grad: %.8f\n", w1->grad);
  printf("w1.data: %.8f\n", w1->data);
  
  printf("x2.grad: %.8f\n", x2->grad);
  printf("x2.data: %.8f\n", x2->data);
  printf("w2.grad: %.8f\n", w2->grad);
  printf("w2.data: %.8f\n", w2->data);
  
  // Free allocated memory
  free_value(o);
  free_neuron(neuron);
  return 0;
}

