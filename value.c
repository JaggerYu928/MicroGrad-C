#include "value.h"

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

void free_value(Value *v) {
  if (v->prev != NULL) {
    for (int i = 0; i < v->prev_size; i++)
	   free_value(v->prev[i]);
    free(v->prev);
  }
  free(v);
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
  printf("label: %s\n", v->label);
  v->backward(v);

  for (int i = 0; i < v->prev_size; i++) {
    backward_all(v->prev[i]);
  }
}
