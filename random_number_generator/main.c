#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_PERIOD 65536  // Maximum possible period for a 16-bit LFSR

// LFSR-1: taps: 16 12 3 1 0 (feedback polynomial: x^16 + x^12 + x^3 + x^1 + 1)
uint16_t lfsr1_fib(void){
    static uint16_t start_state1 = 0xACd1u;
    static uint16_t lfsr1 = 0xACE1u;
    uint16_t bit;
    bit = ((lfsr1 >> 15) ^ (lfsr1 >> 11) ^ (lfsr1 >> 2) ^ (lfsr1 >> 0)) & 1u;
    lfsr1 = (lfsr1 >> 1) | (bit << 15);
    return lfsr1>>15;
}

// LFSR-2: taps: 16 5 3 1 0 (feedback polynomial: x^16 + x^5 + x^3 + x^1 + 1)
uint16_t lfsr2_fib(void){
    static uint16_t start_state2 = 0xcEEF;
    static uint16_t lfsr2 = 0xBEEF;
    uint16_t bit;
    bit = ((lfsr2 >> 15) ^ (lfsr2 >> 4) ^ (lfsr2 >> 2) ^ (lfsr2 >> 0)) & 1u;
    lfsr2 = (lfsr2 >> 1) | (bit << 15);
    return lfsr2>>15;
}

// LFSR-3: taps: 16 11 9 8 1 (feedback polynomial: x^16 + x^11 + x^9 + x^8 + 1)
uint16_t lfsr3_fib(void){
    static uint16_t start_state3 = 0x1534;
    static uint16_t lfsr3 = 0x1234;
    uint16_t bit;
    bit = ((lfsr3 >> 15) ^ (lfsr3 >> 10) ^ (lfsr3 >> 8) ^ (lfsr3 >> 7)) & 1u;
    lfsr3 = (lfsr3 >> 1) | (bit << 15);
    return lfsr3>>15;
}

// Generate random number stream by XORing the outputs of three LFSRs and check for repeats
void calculate_combined_lfsr_period(void) {
    uint32_t generated[MAX_PERIOD];

    uint16_t previous_state = 0;
    uint16_t lfsr1, lfsr2, lfsr3;
    uint16_t period = 0;

    for(int i = 0; i<16; i++){
        lfsr1 = lfsr1_fib();
        lfsr2 = lfsr2_fib();
        lfsr3 = lfsr3_fib();
        uint16_t start_state_combined = lfsr1 ^ lfsr2 ^ lfsr3;
        previous_state = previous_state * 10 + start_state_combined;
    }
    printf("Number: %d\n", previous_state);
    uint16_t current_state = 0;

    while(current_state != previous_state){
        previous_state = current_state;
        lfsr1 = lfsr1_fib();
        lfsr2 = lfsr2_fib();
        lfsr3 = lfsr3_fib();
        uint16_t start_state_combined = lfsr1 ^ lfsr2 ^ lfsr3;

        current_state = current_state*10 + start_state_combined;
        printf("Number: %d\n", current_state);
        period++;
    }

    printf("Period: %d\n", period);
}

int main(void){
    calculate_combined_lfsr_period();
    return 0;
}
