#include <stdint.h>
#include <stdio.h>

void lfsr_fib(void){
    uint16_t start_state = 0xACE1u;  
    uint16_t lfsr = start_state;
    uint16_t bit;                    
    unsigned period = 0;

    /* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
    do {  
        bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
        lfsr = (lfsr >> 1) | (bit << 15);
        printf("Random number: %u\n", lfsr);  // Print the random number
        ++period;
    }
    while (lfsr != start_state);

    printf("Period of LFSR: %u\n", period);
}

int main(void){
    lfsr_fib();
    return 0;
}
