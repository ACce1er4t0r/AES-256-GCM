# AES-256-GCM

File encryption/decryption/transmission suite akin to scp using *OpenSSL* libraries.

+ C
+ PBKDF2
+ use SHA3-256 with 4096 iterations
+ AES-256-GCM

## Usage

``` bash
ufsend <input file> [-d <IP-addr:port>][-l]
ufrec <filename> [-d <port>][-l]
```
