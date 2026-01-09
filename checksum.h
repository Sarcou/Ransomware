#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t crc32(const unsigned char *data, size_t length);
uint32_t calculate_crc32(const char *filepath);
int verify_integrity(const char *filepath, uint32_t expected_crc);

#endif
