#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "crypto.h"

int main(int argc, char** argv) {
    if(argc != 3 && argc != 4) {
        printf("Usage:\n\t-l local\n\t-d network daemon\n\nExample:\n\t./ufrec [filename].ufsec -l\n\t./ufrec [filename] -d [port]\n");
        exit(1);
    }
    if(argc == 3 && strcmp(argv[2], "-l") != 0) {
        printf("Usage:\n\t-l local\n\t-d network daemon\n\nExample:\n\t./ufrec [filename].ufsec -l\n\t./ufrec [filename] -d [port]\n");
        exit(1);
    }
    if(argc == 4 && strcmp(argv[2], "-d") != 0) {
        printf("Usage:\n\t-l local\n\t-d network daemon\n\nExample:\n\t./ufrec [filename].ufsec -l\n\t./ufrec [filename] -d [port]\n");
        exit(1);
    }

    char password[256];
    char raw_password[256];
    char salt[16];
    unsigned char key[16];
    unsigned char IV[16];

    printf("Password: ");
    scanf("%s", password);
    
    strcpy(salt, "CalciumChloride");
    memset(key, 0, sizeof(key));
    strcpy(raw_password, password);
    PKCS5_PBKDF2_HMAC(raw_password, strlen((const char *)raw_password), salt, strlen((const char*)salt), 4096, EVP_sha512(), 16, key);

    printf("Key: ");
    for(int i=0; i < 16; i++) {
        printf("%X ",key[i]);
    }
    printf("\n");

    unsigned char plaintext[65536];
    unsigned char ciphertext[65536];
    unsigned char tmp[65536];
    memset(plaintext, '\0', 65536);
    memset(ciphertext, '\0', 65536);
    memset(tmp, '\0', 65536);

    memset(IV, '\0', sizeof(IV));

    if(argc == 3) {
        FILE *fcipher = fopen(argv[1], "r");
        if(!fcipher) {
            return -1;
        }
        fseek(fcipher, 0, SEEK_END);
        int size = ftell(fcipher);
        rewind(fcipher);

        for(int i=0; i<size; i++) {
            tmp[i] = fgetc(fcipher);
        }

        for(int i=0; i<size-16; i++) {
            ciphertext[i] = tmp[i];
        }
        for(int i=0; i<16; i++) {
            IV[i] = tmp[size-16+i];
        }

        fflush(stdin);

        int size_p = decrypt(ciphertext, size-16, plaintext, key, IV);

        char filename[256];
        strncpy(filename, argv[1], strlen(argv[1])-6);

        FILE *fplain = fopen(filename, "r");
        if(fplain == NULL) {
            fplain = fopen(filename, "w+");
            if(!fplain) {
                printf("error");
            }
            fwrite(plaintext, size_p, sizeof(char), fplain);
        } else {
            printf("File Exist\n");
            return 33;
        }

    }
    
    
    if(argc == 4) {
        int port = atoi(argv[3]);

        int size = Rec_message(tmp, port);

        for(int i=0; i<size-16; i++) {
            ciphertext[i] = tmp[i];
        }
        for(int i=0; i<16; i++) {
            IV[i] = tmp[size-16+i];
        }

        for(int i=0; i < size; i++) {
            printf("%x ", ciphertext[i]);
        }
        printf("\n");

        decrypt(ciphertext, size-16, plaintext, key, IV);

        char filename[256];
        strcpy(filename, argv[1]);

        FILE *fplain = fopen(filename, "r");
        if(fplain == NULL) {
            fplain = fopen(filename, "w+");
            if(!fplain) {
                printf("error");
            }
            fwrite(plaintext, size-16, sizeof(char), fplain);
            printf("Successfully received and decrypted %s (%d bytes written).\n", filename, size-16);
        } else {
            printf("File Exist\n");
            return 33;
        }
    }
    
    return 0;
}