#include <stdio.h>
#include <stdint.h>

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

void initialize_aes_sbox(uint8_t sbox[256]) {
    uint8_t p = 1, q = 1;
    
    do {
        p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

        q ^= q << 1;
        q ^= q << 2;
        q ^= q << 4;
        q ^= q & 0x80 ? 0x09 : 0;

        uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

        sbox[p] = xformed ^ 0x63;
    } while (p != 1);

    sbox[0] = 0x63;
}

uint8_t galois_multiply(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    uint8_t hi_bit_set;
    
    for (int counter = 0; counter < 8; counter++) {
        if (b & 1) {
            p ^= a;
        }

        hi_bit_set = a & 0x80;
        a <<= 1; 
        
        if (hi_bit_set) {
            a ^= 0x1b;
        }
        
        b >>= 1; 
    }
    
    return p;
}

uint8_t calculate_rcon(uint8_t round) {
    uint8_t rcon = 0x01; 
    
    for (uint8_t i = 1; i < round; i++) {
        rcon = galois_multiply(rcon, 0x02);
    }
    
    return rcon;
}

int main() {
    char input[33]; 
    uint32_t word[44]; 
    
    printf("Enter a 32-character hex string: ");
    scanf("%32s", input);

    sscanf(input, "%8x%8x%8x%8x", &word[0], &word[1], &word[2], &word[3]);

    uint32_t temp;
    uint8_t temp0, temp1, temp2, temp3;
    uint8_t rcon;
    uint8_t sbox[256];
    initialize_aes_sbox(sbox);
    
    for(int i = 4; i < 44; i++) {
        temp = word[i - 1];
        if(i % 4 == 0) {
            temp = (temp << 8) | (temp >> 24); 

            temp0 = sbox[temp >> 24];
            temp1 = sbox[(temp >> 16) & 0xFF];
            temp2 = sbox[(temp >> 8) & 0xFF];
            temp3 = sbox[temp & 0xFF];
            temp = (temp0 << 24) | (temp1 << 16) | (temp2 << 8) | temp3;

            rcon = calculate_rcon(i / 4);
            temp = temp ^ (rcon<<24);
        }
        word[i] = word[i - 4] ^ temp;
    }

    printf("Key schedule:\n");
    for(int i = 0; i < 44; i = i + 4) {
        printf("%08x %08x %08x %08x\n", word[i], word[i + 1], word[i + 2], word[i + 3]);
    }

    return 0;
}
