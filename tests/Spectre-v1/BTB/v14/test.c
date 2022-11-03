/*
test14 (Kocher13)
Do the safety check into an inline function. control dependency
No BTB restrictions.
*/
#include <stdio.h>
#include "../../../../klee/include/klee/klee.h"
#include <stdlib.h>
#include <stdint.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;

inline static int is_x_safe(size_t x) {
    if (x < array1_size) 
        return 1;
    return 0;
}

void victim_fun(int idx) {
    int x = 0;
    uint8_t y;
	if (is_x_safe(x)) {                  
        temp &= array2[array1[x] * 512];
    }

	x = idx;
	
	if( x < array1_size){
		y = array1[x];
	}
}

int main(int argn, char* args[]) {
    int source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}

