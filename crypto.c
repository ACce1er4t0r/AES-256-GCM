#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "crypto.h"

int encrypt(const unsigned char *plain, int l_p, unsigned char *cipher, unsigned char *key, unsigned char *IV) {
    unsigned char iv[AES_BLOCK_SIZE];

    int l_c, l;

    memset(cipher, '\0', sizeof(cipher));
    RAND_bytes(IV, AES_BLOCK_SIZE);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, IV);
    EVP_EncryptUpdate(ctx, cipher, &l, plain, l_p);
    l_c = l;
    EVP_EncryptFinal_ex(ctx, cipher + l, &l);
    l_c += l;
    EVP_CIPHER_CTX_free(ctx);
    return l_c;
}

int decrypt(const unsigned char *cipher, int l_c, unsigned char *plain, unsigned char *key, unsigned char *IV) {
    memset(plain, '\0', sizeof(plain));
    int l_p, l;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, IV);
    EVP_DecryptUpdate(ctx, plain, &l, cipher, l_c);
    l_p = l;
    EVP_DecryptFinal_ex(ctx, plain + l, &l);
    l_p += l;
    EVP_CIPHER_CTX_free(ctx);
    return l_p;
}