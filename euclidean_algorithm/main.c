#include<stdio.h>


int gcd(int num1, int num2){
    while(num2 != 0){
        int temp = num2;
        num2 = num1 % num2;
        num1 = temp;
    }
    return num1;
}

void extended_euclidean(int num1, int num2, int *x, int *y){
    if(num2 == 0){
        *x = 1;
        *y = 0;
    }
    int x1 = 1, x2 = 0, y1 = 0, y2 = 1;
    int q, r;
    while(num2 != 0){
        q = num1 / num2;
        r = num1 % num2;

        *x = x1 - q * x2;
        *y = y1 - q * y2;

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

int main(){
    int num1, num2;
    printf("Enter two numbers: ");
    scanf("%d %d", &num1, &num2);
    int gcd_result = gcd(num1, num2);
    printf("GCD of %d and %d is %d\n", num1, num2, gcd_result);
    int x, y;
    extended_euclidean(num1, num2, &x, &y);
    printf("x = %d, y = %d\n", x, y);
    if(gcd_result == 1){
        if(x<0){
            x += num2;
        }
        if(y<0){
            y += num1;
        }
        printf("Multiplicative inverse of %d mod %d is %d\n", num1, num2, x);
        printf("Multiplicative inverse of %d mod %d is %d\n", num2, num1, y);
    }
    return 0;
}