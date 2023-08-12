#include <cstddef>
#include <stdio.h>
#include <ap_int.h>


#define LENGTH 3
#define CHARSET_LENGTH 4
#define BUFFER_SIZE 64

void md5_round1(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5_round2(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5_round3(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5_round4(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5(const unsigned int *in, unsigned int out[4], const size_t size);
void generate_combinations(const unsigned char md5bf[32], const unsigned char charset[CHARSET_LENGTH], unsigned char out[4]);
ap_uint<32> factorial(ap_uint<8> n);
// Function to compute permutations of n items taken r at a time.
ap_uint<32> permutation(ap_uint<8> n, ap_uint<8> r);
// Function to compute sum of permutations of n items taken up to k at a time.
ap_uint<32> sum_permutations(ap_uint<8> n, ap_uint<8> k);
