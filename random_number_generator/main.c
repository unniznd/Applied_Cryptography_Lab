#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_PERIOD 65536  // Maximum possible period for a 16-bit LFSR

// LFSR-1: taps: 16 12 3 1 0 (feedback polynomial: x^16 + x^12 + x^3 + x^1 + 1)
uint16_t lfsr1_fib(void){
    static uint16_t start_state1 = 0xACE1u;
    static uint16_t lfsr1 = 0xACE1u;
    uint16_t bit;
    bit = ((lfsr1 >> 15) ^ (lfsr1 >> 11) ^ (lfsr1 >> 2) ^ (lfsr1 >> 0)) & 1u;
    lfsr1 = (lfsr1 >> 1) | (bit << 15);
    return lfsr1;
}

// LFSR-2: taps: 16 5 3 1 0 (feedback polynomial: x^16 + x^5 + x^3 + x^1 + 1)
uint16_t lfsr2_fib(void){
    static uint16_t start_state2 = 0xBEEF;
    static uint16_t lfsr2 = 0xBEEF;
    uint16_t bit;
    bit = ((lfsr2 >> 15) ^ (lfsr2 >> 4) ^ (lfsr2 >> 2) ^ (lfsr2 >> 0)) & 1u;
    lfsr2 = (lfsr2 >> 1) | (bit << 15);
    return lfsr2;
}

// LFSR-3: taps: 16 11 9 8 1 (feedback polynomial: x^16 + x^11 + x^9 + x^8 + 1)
uint16_t lfsr3_fib(void){
    static uint16_t start_state3 = 0x1234;
    static uint16_t lfsr3 = 0x1234;
    uint16_t bit;
    bit = ((lfsr3 >> 15) ^ (lfsr3 >> 10) ^ (lfsr3 >> 8) ^ (lfsr3 >> 7)) & 1u;
    lfsr3 = (lfsr3 >> 1) | (bit << 15);
    return lfsr3;
}

// Check if a number has already been generated
bool check_repeat(uint16_t *generated, uint16_t number, unsigned period) {
    for (unsigned i = 0; i < period; ++i) {
        if (generated[i] == number) {
            return true;  // Number has already been generated
        }
    }
    return false;
}

// Generate random number stream by XORing the outputs of three LFSRs and check for repeats
void calculate_combined_lfsr_period(void) {
    uint16_t start_state_combined;
    uint16_t lfsr1, lfsr2, lfsr3, combined_lfsr;
    uint16_t generated[MAX_PERIOD];  // To store generated numbers
    unsigned period = 0;

    // Initialize start state for the combined LFSR
    lfsr1 = lfsr1_fib();
    lfsr2 = lfsr2_fib();
    lfsr3 = lfsr3_fib();
    start_state_combined = lfsr1 ^ lfsr2 ^ lfsr3;

    combined_lfsr = start_state_combined;
    generated[period++] = combined_lfsr;  // Store the initial state
    
    printf("Random number 1: %u\n", combined_lfsr);

    do {
        // Generate the next random number by XORing the three LFSRs
        lfsr1 = lfsr1_fib();
        lfsr2 = lfsr2_fib();
        lfsr3 = lfsr3_fib();
        combined_lfsr = lfsr1 ^ lfsr2 ^ lfsr3;

        // Check if this number has already been generated
        if (check_repeat(generated, combined_lfsr, period)) {
            continue;
        }

        generated[period++] = combined_lfsr;  // Store the new number

        printf("Random number %u: %u\n", period + 1, combined_lfsr);

    } while (period < MAX_PERIOD );

    printf("Period of combined LFSR: %u\n", period);
}

int main(void){
    calculate_combined_lfsr_period();
    return 0;
}
