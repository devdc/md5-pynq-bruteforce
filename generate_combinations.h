#include <cstddef>
#include <stdio.h>
#include <ap_int.h>

typedef ap_uint<8> char_t;

#define LENGTH 4
//#define CHARSET_LENGTH 94 // ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
#define CHARSET_LENGTH 32 //A-Z a-z 0-9 !@
#define BUFFER_SIZE 64
#define MD5_CHAR_LENGTH 32

void md5_round1(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5_round2(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5_round3(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5_round4(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5(const unsigned int *in, unsigned int out[4], const size_t size);
void generate_combinations(const unsigned char md5bf[MD5_CHAR_LENGTH], const unsigned char charset[CHARSET_LENGTH], unsigned char out[LENGTH]);
long long compute_permutations_with_repetition(int n, int r);
void byte_to_hex_string(unsigned char* bytes, size_t length, unsigned char* hexString);
