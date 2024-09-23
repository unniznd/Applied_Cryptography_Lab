#include <stdio.h>
#include <stdint.h>

unsigned char irreducible ;
unsigned char max;

unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    unsigned char hi_bit_set;

    while(b){
        if ((b & 1) == 1)
            p ^= a;
        hi_bit_set = (a & 0x20);  
        a <<= 1;                        
        if (hi_bit_set == 0x20)    
            a ^= irreducible;
        b >>= 1;                        
    }
    return p;
}

// Costly operation
unsigned char gdiv(unsigned char a, unsigned char b){
    unsigned char q = 0;
    unsigned char i = 0;
    unsigned char product = 0;
    unsigned char temp = 0;

    if (b > a){
        temp = a;
        a = b;
        b = temp;
    }

    while(i < 8){
        product = gmul(b, i+1);

        if(product == a){
            q ^= i+1;
            break;
        }
        if((product ^ a) < b){
            q ^= i+1;
            
            a ^= product;
            if(a == 0 || a < b){
                break;
            }

            i = 0;
        }
        i++;
    }

    return q;
}

void extended_euclidean(
    unsigned char num1, unsigned char num2, 
    unsigned char *x, unsigned char *y
){
    if(num2 == 0){
        *x = 1;
        *y = 0;
    }
    unsigned char x1 = 1, x2 = 0, y1 = 0, y2 = 1;
    unsigned char q, r;
    while(num2 != 0){
        q = gdiv(num1, num2);
        
        r = num1 ^ (unsigned char) (gmul(q, num2));
        if(q == 0) break;

        *x = x1 ^ gmul(q, x2);
        *y = y1 ^ gmul(q, y2);

        num1 = num2;
        num2 = r;

        x1 = x2;
        x2 = *x;

        y1 = y2;
        y2 = *y;
    }
    
    *x = x1;
    *y = y1;
}


int main() {
    printf("Enter irreducible polynomial: ");
    scanf("%x", &irreducible);
    unsigned char temp = irreducible;
    max = 1;
    while(temp > 0){
        temp >>= 1;
        max <<= 1;
    }
    unsigned char num;
    printf("Enter number: ");
    scanf("%x", &num);

    unsigned char x, y;
    extended_euclidean(irreducible, num, &x, &y);
    if(x<0) x = x ^ irreducible;
    if(y<0) y = y ^ irreducible;


    printf("Multiplicative inverse of %x is %x\n", num, y);
}
