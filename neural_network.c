#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Value {
  double data;
  double grad;
  void (*backward)(struct Value *);
  struct Value **prev;
  int prev_size;
  char op;
  char *label;
} Value;

void backward_add(struct Value *v);
void backward_mul(struct Value *v);
void backward_pow(struct Value *v);
void backward_tanh(struct Value *v);
void backward_exp(struct Value *v);

Value *create_value(double data, Value **prev, int prev_size, char op,
                    char *label) {
  Value *v = (Value *)malloc(sizeof(Value));
  v->data = data;
  v->grad = 0.0;
  v->prev = prev;
  v->prev_size = prev_size;
  v->op = op;
  v->label = label;
  return v;
}

Value *add(Value *a, Value *b) {
  Value *out = (Value *)malloc(sizeof(Value));
  out->data = a->data + b->data;
  out->grad = 0.0;
  out->backward = backward_add;

  out->prev_size = 2;
  out->prev = (Value **)malloc(out->prev_size * sizeof(Value *));
  out->prev[0] = a;
  out->prev[1] = b;

  out->op = '+';
  out->label = "";

  return out;
}

Value *mul(Value *a, Value *b) {
  Value *out = (Value *)malloc(sizeof(Value));
  out->data = a->data * b->data;
  out->grad = 0.0;
  out->backward = backward_mul;

  out->prev_size = 2;
  out->prev = (Value **)malloc(out->prev_size * sizeof(Value *));
  out->prev[0] = a;
  out->prev[1] = b;

  out->op = '*';
  out->label = "";

  return out;
}

Value *tanh_v(Value *v) {
  double x = v->data;
  double t = (exp(2 * x) - 1) / (exp(2 * x) + 1);

  Value *out = (Value *)malloc(sizeof(Value));
  out->data = t;
  out->grad = 0.0;
  out->backward = backward_tanh;

  out->prev_size = 1;
  out->prev = (Value **)malloc(out->prev_size * sizeof(Value *));
  out->prev[0] = v;

  out->op = 't';
  out->label = "";

  return out;
}

Value *pow_v(Value *v, double p) {
  Value *out = (Value *)malloc(sizeof(Value));
  out->data = pow(v->data, p);
  out->grad = 0.0;
  out->backward = backward_pow;

  out->prev_size = 1;
  out->prev = (Value **)malloc(out->prev_size * sizeof(Value *));
  out->prev[0] = v;

  out->op = 'p';
  out->label = "";

  return out;
}

Value *exp_v(Value *v) {
  double x = v->data;
  double e = exp(x);

  Value *out = (Value *)malloc(sizeof(Value));
  out->data = e;
  out->grad = 0.0;
  out->backward = backward_exp;

  out->prev_size = 1;
  out->prev = (Value **)malloc(out->prev_size * sizeof(Value *));
  out->prev[0] = v;

  out->op = 'e';
  out->label = "";

  return out;
}

void backward_add(struct Value *v) {
  v->prev[0]->grad += 1.0 * v->grad;
  v->prev[1]->grad += 1.0 * v->grad;
}

void backward_mul(struct Value *v) {
  v->prev[0]->grad += v->prev[1]->data * v->grad;
  v->prev[1]->grad += v->prev[0]->data * v->grad;
}

void backward_pow(struct Value *v) {
  double n = v->data / v->prev[0]->data;
  v->prev[0]->grad += n * pow(v->prev[0]->data, n - 1) * v->grad;
}

void backward_tanh(struct Value *v) {
  double t = v->data;
  v->prev[0]->grad += (1 - t * t) * v->grad;
}

void backward_exp(struct Value *v) {
  double e = v->data;
  v->prev[0]->grad += e * v->grad;
}

void backward_all(Value *v) {
  if (v->prev_size == 0) {
    return;
  }

  v->backward(v);

  for (int i = 0; i < v->prev_size; i++) {
    backward_all(v->prev[i]);
  }
}

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
