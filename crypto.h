#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>

#define BUFSIZE 256
#define MAX_INPUT_LEN 65536
#define HMAC_LEN 64
#define IV_LEN 16

int encrypt(const unsigned char *plain, int l_p, unsigned char *cipher, unsigned char *key, unsigned char *IV);
int decrypt(const unsigned char *cipher, int l_c, unsigned char *plain, unsigned char *key, unsigned char *IV);

#endif