#include "../value.h"

int main() {
  // inputs x1, x2
  Value *x1 = create_value(2.0, NULL, 0, '\0', "x1");
  Value *x2 = create_value(0.0, NULL, 0, '\0', "x2");
  // weights w1, w2
  Value *w1 = create_value(-3.0, NULL, 0, '\0', "w1");
  Value *w2 = create_value(1.0, NULL, 0, '\0', "w2");
  // bias b
  Value *b = create_value(6.8813735870195432, NULL, 0, '\0', "b");
  // x1w1 + x2w2 + b
  Value *x1w1 = mul(x1, w1);
  x1w1->label = "x1w1";
  Value *x2w2 = mul(x2, w2);
  x2w2->label = "x2w2";
  Value *x1w1x2w2 = add(x1w1, x2w2);
  x1w1x2w2->label = "x1w1x2w2";
  Value *n = add(x1w1x2w2, b);
  n->label = "n";
  Value *o = tanh_v(n);
  o->label = "o";
  printf("o output: %f\n", o->data);

  o->grad = 1.0;
/*  o->backward(o);
  printf("o grad: %f\n", o->grad);
  printf("n grad: %f\n", n->grad);
  n->backward(n);
  printf("b grad: %f\n", b->grad);
  printf("x1w1x2w2 grad: %f\n", x1w1x2w2->grad);
  x1w1x2w2->backward(x1w1x2w2);
  printf("x1w1 grad: %f\n", x1w1->grad);
  printf("x2w2 grad: %f\n", x2w2->grad);
  x1w1->backward(x1w1);
  printf("x1 grad: %f\n", x1->grad);
  printf("w1 grad: %f\n", w1->grad);
  x2w2->backward(x2w2);
  printf("x2 grad: %f\n", x2->grad);
  printf("w2 grad: %f\n", w2->grad);
*/

  backward_all(o);

  printf("Gradients:\n");
  printf("x1: %f\n", x1->grad);
  printf("x2: %f\n", x2->grad);
  printf("w1: %f\n", w1->grad);
  printf("w2: %f\n", w2->grad);
  printf("x1w1: %f\n", x1w1->grad);
  printf("x2w2: %f\n", x2w2->grad);
  printf("x1w1x2w2: %f\n", x1w1x2w2->grad);
  printf("b: %f\n", b->grad);

  // Free memory
  free(x1);
  free(x2);
  free(w1);
  free(w2);
  free(b);
  free(x1w1->prev);
  free(x1w1);
  free(x2w2->prev);
  free(x2w2);
  free(x1w1x2w2->prev);
  free(x1w1x2w2);
  free(n->prev);
  free(n);
  free(o->prev);
  free(o);

  return 0;
}
