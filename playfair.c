#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


int is_valid_key(char key[26]){
    int char_count[128] = {0}; 
    for(int i = 0; i < strlen(key); i++){
        if(char_count[(int)key[i]] != 0 ){ 
            return 0;
        }
        char_count[(int)key[i]] = 1;  
    }
    return 1;
}

void init_key_matrx(char matrix[6][6], char key[26]){
    int char_count[128] = {0}; 
    int row_index = 0, col_index = 0;
    for(int i = 0; i<strlen(key); i++){
        if(65 <= (int) key [i] && (int) key[i]<= 90){
            key[i] = (char)((int) key[i] + 32);
        }
        matrix[row_index][col_index] = key[i];
        char_count[(int)key[i]] = 1;

        col_index++;
        if(col_index == 6){
            col_index = 0;
            row_index++;  
        }
    }

    
    for(int chr = 97; chr<123; chr++){
        if(!char_count[chr]){
            matrix[row_index][col_index] = (char) chr;
            char_count[chr] = 1;

            col_index++;
            if(col_index == 6){
                col_index = 0;
                row_index++;
                
            }
            
        }
    }

    for(int chr = 48; chr<58; chr++){
        if(!char_count[chr]){
            matrix[row_index][col_index] = (char) chr;
            char_count[chr] = 1;

            col_index++;
            if(col_index == 6){
                col_index = 0;
                row_index++;
                
            }
        }
        
    }
}


int find_repeat_char_count(char *temp_plaintext, int temp_len){
    int rep_count = 0;
    for(int i = 0; i<temp_len-1; i++){
        if(temp_plaintext[i] == temp_plaintext[i+1]){
            rep_count += 1;
        }
    }

    return rep_count;
}

void preprocess_plaintext(char *temp_plaintext, char *plaintext, int temp_len){
    int idx = 0;
    for(int i = 0; i < temp_len; i++){
        if(65 <= (int) temp_plaintext[i] && (int) temp_plaintext[i] <= 'Z'){
            temp_plaintext[i] = (char)((int) temp_plaintext[i] + 32);
        }
        plaintext[idx++] = temp_plaintext[i];
        if(i < temp_len - 1 && temp_plaintext[i] == temp_plaintext[i + 1]){
            plaintext[idx++] = (temp_plaintext[i] == 'x') ? 'y' : 'x';
        }
    }
    if(idx % 2 != 0){
        plaintext[idx] = (plaintext[idx - 1] == 'x') ? 'y' : 'x';
    }
}



void get_position(char matrix[6][6], char c, int pos[2]){
    for(int i = 0; i<6; i++){
        for(int j = 0; j<6; j++){
            if(c == matrix[i][j]){
                pos[0] = i;
                pos[1] = j;
                break;
            }
        }
    }
}

void encrypt(char key_matrix[6][6], char *plaintext, char *ciphertext, int len){
    int pos0[2], pos1[2];
    for(int i = 0; i<len; i+=2){
        get_position(key_matrix, plaintext[i], pos0);
        get_position(key_matrix, plaintext[i+1], pos1);
        if(pos0[0] == pos1[0]){
            ciphertext[i] = key_matrix[pos1[0]][(pos0[1] + 1) % 6];
            ciphertext[i+1] = key_matrix[pos1[0]][(pos1[1] + 1) % 6];
        }
        else if(pos0[1] == pos1[1]){
            ciphertext[i] = key_matrix[(pos0[0] + 1) % 6][pos1[1]];
            ciphertext[i+1] = key_matrix[(pos1[0] + 1) % 6][pos1[1]];
        }
        else{
            ciphertext[i] = key_matrix[pos0[0]][pos1[1]];
            ciphertext[i+1] = key_matrix[pos1[0]][pos0[1]];
        }
    }
}

void decrypt(char key_matrix[6][6], char *plaintext, char *ciphertext, int len){
    int pos0[2], pos1[2];
    for(int i = 0; i<len; i+=2){
        get_position(key_matrix, plaintext[i], pos0);
        get_position(key_matrix, plaintext[i+1], pos1);

        if(pos0[0] == pos1[0]){
            if(pos0[1] == 0)
                pos0[1] = 6;
            if(pos1[1] == 0)
                pos1[1] = 6;
            
                
            ciphertext[i] = key_matrix[pos1[0]][((pos0[1] - 1) % 6)];
            ciphertext[i+1] = key_matrix[pos1[0]][((pos1[1] - 1) % 6)];
        }
        else if(pos0[1] == pos1[1]){
            if(pos0[0] == 0)
                pos0[0] = 6;
            if(pos1[0] == 0)
                pos1[0] = 6;
            ciphertext[i] = key_matrix[((pos0[0] - 1) % 6)][pos1[1]];
            ciphertext[i+1] = key_matrix[((pos1[0] - 1) % 6)][pos1[1]];
        }
        else{
            ciphertext[i] = key_matrix[pos0[0]][pos1[1]];
            ciphertext[i+1] = key_matrix[pos1[0]][pos0[1]];
        }
    }
}

void display_key_matrix(char matrix[6][6]){
    for(int i = 0; i<6; i++){
        for(int j = 0; j<6; j++){
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}


int main(){

    char key[26];

    printf("Playfair Cipher\n");

    printf("Enter key: ");
    scanf("%s", key);

    if(!is_valid_key(key)){
        printf("Error: Repeating character found in key\n");
        exit(1);
    }

    char key_matrix[6][6];
    init_key_matrx(key_matrix, key);
    display_key_matrix(key_matrix);
    
    int temp_len;
    printf("Enter length of plaintext: ");
    scanf("%d", &temp_len);

    char temp_plaintext[temp_len];
    printf("Enter plaintext: ");
    scanf("%s", temp_plaintext);

    int rep_count = find_repeat_char_count(temp_plaintext, temp_len);

    int len = temp_len + rep_count;
    if(len % 2 != 0){
        len += 1;
    }

    char plaintext[len];
    preprocess_plaintext(temp_plaintext, plaintext, temp_len);

    printf("Preprocessed plaintext: ");
    for(int i = 0; i<len; i++){
        printf("%c", plaintext[i]);
    }
    printf("\n");

    char ciphertext[len];
    encrypt(key_matrix, plaintext, ciphertext, len);


    printf("Ciphertext: ");
    for(int i = 0; i<len; i++){
        printf("%c", ciphertext[i]);
    }
    printf("\n");

    char decrypted_text[len];
    decrypt(key_matrix, ciphertext, decrypted_text, len);

    printf("Decrypted text: ");
    for(int i = 0; i<len; i++){
        printf("%c", decrypted_text[i]);
    }

    printf("\n");

    return 0;
}