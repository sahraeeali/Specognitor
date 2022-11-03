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
/////////////////// test 1 ///////////////////////////////

inline static void victim_fun0(uint32_t idx)  __attribute__ ((optnone)) 
{
    int y = 0;
    if (idx < array1_size) {    
        y = array1[idx];
        temp &= array2[y*64];
    }
}

/////////////////// test 2 ///////////////////////////////

inline static void victim_fun1(uint32_t idx)  __attribute__ ((optnone)) 
{
    int y = 0;
    int b = 6;
    int a = 12;
    if (idx >= array1_size) {
		//dummy computation.
		y += (a*b) + (a/b);
		y += a/b;
    } else {
		y = array1[idx];
		temp &= array2[y*64];
    }
}


/////////////////// test 3 ///////////////////////////////
inline static void leak_byte_local_function(uint8_t k) {
    temp &= array2[(k)* 512]; 
}

inline static void victim_fun2(uint32_t idx) {
    if (idx < array1_size) {                  
        leak_byte_local_function(array1[idx]);
    }
}

/////////////////// test 4 ///////////////////////////////
static __attribute__ ((noinline)) void leakByteNoinlineFunction(uint8_t k) { 
    temp &= array2[k* 512]; 
}

inline static void victim_fun3(uint32_t x) {
    if (x < array1_size)
        leakByteNoinlineFunction(array1[x]);
}
/////////////////// test 5 ///////////////////////////////
inline static void victim_fun4(uint32_t idx) {
    if (idx < array1_size) {                  
        temp &= array2[array1[idx << 1] * 512];
    }
}
/////////////////// test 6 ///////////////////////////////
inline static void victim_fun5(uint32_t idx) {
    size_t i;
    if (idx < array1_size) {                  
        for (i = idx; i >= 0; i--)
            temp &= array2[array1[i] * 512];
    }
}
/////////////////// test 7 ///////////////////////////////
inline static void victim_fun6(uint32_t idx) {

    if ((idx & array_size_mask) == idx) {                  
        temp &= array2[array1[idx]];
    }
}
/////////////////// test 8 ///////////////////////////////
inline static void victim_fun7(uint32_t idx) {
    static size_t last_x = 0;
    if (idx == last_x) {                  
        temp &= array2[array1[idx] * 512];
    }

    if (idx < array1_size)
        last_x = idx;
}
/////////////////// test 9 ///////////////////////////////
inline static void victim_fun8(uint32_t idx) {
    temp &= array2[array1[idx < (array1_size-1)? (idx + 1) : 0] * 512];
}
/////////////////// test 11 ///////////////////////////////
inline static void victim_fun9(uint32_t idx) {
    if (idx < array1_size) {                 
        if (array1[idx] == k)
            temp &= array2[0];
    }
}
/////////////////// test 12 ///////////////////////////////
inline static void victim_fun10(uint32_t idx) {
    if (idx < array1_size) {                  
        temp = memcmp(&temp, array2 + (array1[idx] * 512), 1);
    }
}
/////////////////// test 14 ///////////////////////////////
inline static int is_x_safe(uint32_t x) {
    if (x < array1_size) 
        return 1;
    return 0;
}

inline static void victim_fun11(uint32_t idx) {
    if (is_x_safe(idx)) {                  
        temp &= array2[array1[idx] * 512];
    }
}
/////////////////// test 15 ///////////////////////////////
inline static void victim_fun12(uint32_t idx) {
    if (idx < array1_size) {                  
        temp &= array2[array1[idx ^ 255] * 512];
    }
}

void (*p[13]) (uint32_t x);

void ind_call(uint8_t i, uint32_t val){
	(*p[i]) (val);
	return;
}

uint8_t victim_fun(uint32_t idx)  __attribute__ ((optnone)) 
{
	uint8_t i;
    p[0] = victim_fun0;
    p[1] = victim_fun1;
    p[2] = victim_fun2;
    p[3] = victim_fun3;
    p[4] = victim_fun4;
    p[5] = victim_fun5;
    p[6] = victim_fun6;
    p[7] = victim_fun7;
    p[8] = victim_fun8;
    p[9] = victim_fun9;
    p[10] = victim_fun10;
    p[11] = victim_fun11;
    p[12] = victim_fun12;
	
	for(i = 0; i < 10; i++){
		ind_call(i, i);
	}

	ind_call(5, idx);

    array2[0] = 2;  
    array1[0] = 2;
    return temp;
}

int main() {
    uint32_t source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}

