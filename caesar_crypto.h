#ifndef CAESAR_CRYPTO_H
#define CAESAR_CRYPTO_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char caesar_char(char c, int shift);
int caesar_encrypt_file(const char *input, const char *output, int shift);
int caesar_decrypt_file(const char *input, const char *output, int shift);

#endif
