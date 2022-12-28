#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "utils.h"
#include "crypto.h"

int main(int argc, char** argv) {
    if(argc != 3 && argc != 4) {
        printf("Usage:\n\t-l local\n\t-d network daemon\n\nExample:\n\t./ufsend [filename] -l\n\t./ufsend [filename] -d [ip]:[port]\n");
        exit(1);
    }
    if(argc == 3 && strcmp(argv[2], "-l") != 0) {
        printf("Usage:\n\t-l local\n\t-d network daemon\n\nExample:\n\t./ufsend [filename] -l\n\t./ufsend [filename] -d [ip]:[port]\n");
        exit(1);
    }
    if(argc == 4 && strcmp(argv[2], "-d") != 0) {
        printf("Usage:\n\t-l local\n\t-d network daemon\n\nExample:\n\t./ufsend [filename] -l\n\t./ufsend [filename] -d [ip]:[port]\n");
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

    memset(plaintext, '\0', 65536);
    memset(ciphertext, '\0', 65536);
    memset(IV, '\0', sizeof(IV));

    FILE *fplain = fopen(argv[1], "r");
    if(!fplain) {
        return -1;
    }
    fseek(fplain, 0, SEEK_END);
    int size = ftell(fplain);
    rewind(fplain);
    fread(plaintext, sizeof(char), size, fplain);
    
    int size_c = encrypt(plaintext, size, ciphertext, key, IV);

    for(int i=0; i<16; i++) {
        ciphertext[i+size_c] = IV[i]; 
    }

    char filename[256];
    memset(filename, '\0', 256);
    strcpy(filename, argv[1]);
    strcat(filename, ".ufsec");

    FILE *fcipher = fopen(filename, "r");
    if(fcipher == NULL) {
        fcipher = fopen(filename, "w+");
        if(!fcipher) {
            printf("error");
        }
        fwrite(ciphertext, size_c+16, sizeof(char), fcipher);
    } else {
        printf("File Exist\n");
        return 33;
    }
    // if(argc == 3) {
        
    // }
    if(argc == 4) {
        char ip[16];
        char port_s[6];
        for(int i=0; i<strlen(argv[3]); i++) {
            if(argv[3][i] == ':') {
                strncpy(ip, argv[3], i);
                for(int j=0; j<strlen(argv[3])-i+1; j++) {
                    port_s[j] = argv[3][i+j+1];
                }
                break;
            }
        }
        int port = atoi(port_s);

        printf("Successfully encrypted testfile to %s (%d bytes written)\n", filename, size_c + 16);

        for(int i=0; i < size_c+16; i++) {
            printf("%x ",ciphertext[i]);
        }
        printf("\n");

        Send_message(ciphertext, size_c+16, ip, port);
    }

    return 0;
}