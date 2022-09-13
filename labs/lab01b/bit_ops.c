#include <stdint.h>
#include <stdio.h>
#include "bit_ops.h"

/*
  Returns the n-th bit of x.
  Assumes 0 <= n <= 31.
*/
uint32_t get_bit(uint32_t x, uint32_t n) {
  return (x >> n) & 1;
}

/*
  Set the n-th bit of *x to v.
  Assumes 0 <= n <= 31, and v is 0 or 1.
*/
void set_bit(uint32_t *x, uint32_t n, uint32_t v) {
  uint32_t setter = ~(1 << n);
  v = v << n;
  *x = (*x & setter) | v;
}

/*
  Flips the n-th bit of *x.
  Assumes 0 <= n <= 31.
*/
void flip_bit(uint32_t *x, uint32_t n) {
    *x = (*x ^ 1 << n);
}

