#ifndef _VALUE_H
#define _VALUE_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

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
void backward_all(Value *v);

Value *create_value(double data, Value **prev, int prev_size, char op,
                    char *label);
void free_value(Value *v);
Value *add(Value *a, Value *b);
Value *mul(Value *a, Value *b);
Value *tanh_v(Value *v);
Value *pow_v(Value *v, double p);
Value *exp_v(Value *v);

#endif
