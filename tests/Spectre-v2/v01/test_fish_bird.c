/*
The mixed example.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../../../klee/include/klee/klee.h"

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 64];

uint8_t marked_secret_array1[16];

unsigned int input_bound = 13;

// variables used
uint8_t k = 10;
uint8_t temp = 0;
unsigned int array_size_mask = 15;

inline static void safe_func(uint32_t idx)  __attribute__ ((optnone))
{
    int a = 2*3;
}

inline static void unsafe_func(uint32_t idx)  __attribute__ ((optnone))
{
  temp = array1[idx];
  temp &= array2[temp*64];
}

void (*p[2]) (uint32_t x);

void ind_call(uint8_t i, uint32_t val){
	(*p[i]) (val);
	return;
}

uint8_t victim_fun(uint32_t idx)  __attribute__ ((optnone))
{
	uint8_t i;
  p[0] = safe_func;
  p[1] = unsafe_func;

	for(i = 0; i < 10; i++){
		ind_call(1, i);
	}

  array2[0] = 2;
  array1[0] = 2;

	ind_call(0, idx);

  return temp;
}

int main() {
    uint32_t source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}
