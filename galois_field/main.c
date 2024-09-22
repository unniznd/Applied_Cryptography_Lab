#include <stdio.h>



unsigned char anti_log[256];
unsigned char glog[256];


unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    unsigned char counter;
    unsigned char hi_bit_set;

    for (counter = 0;counter < 8;counter++) {
        if ((b & 1) == 1)
                p ^= a;
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set == 0x80)
                a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

int init() {
    int i;
    unsigned char p;
    anti_log[0]=0;
    p=1;

    printf("\n");
    for (i = 1;i < 256; i++) {
        p = gmul(p, 0xe5);
        anti_log[i] = p;
    }
   
   
    unsigned char temp;
    glog[0] = 0;
    glog[1] = 1;

    for (i = 1;i < 256;i++) {
        temp = anti_log[i];
        glog[temp]=i;
    }
    glog[1]=0x00;
}


int main() {
    init();

    /* try and multiply with log tables */
    int a;
    unsigned char result;
    printf("Enter number to calculate multiplicative inverse <0x00 - 0xff>? ");
    if (scanf("%x", &a) < 0) {
        printf("\nincorrect usage\n");
        return 1;
    }
    if (a > 255 || a < 0) {
        printf("number between 0-ff please!\n");
        return 1;
    }
    if (a == 0) {
        result = 0;
    } else {
        result = anti_log[(255 - glog[a])];
    }
    printf("result = %x\n", result);
}