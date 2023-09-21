#include <cstddef>
#include <stdio.h>
//#include <hls_stream.h>
//#include <ap_axi_sdata.h>
#include <ap_int.h>

//typedef ap_axis<32,0,0,0> progress_t;


#define LENGTH 5
//#define CHARSET_LENGTH 94 // ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
#define CHARSET_LENGTH 36 //A-Z a-z 0-9 !@
#define BUFFER_SIZE 64
//#define MD5_CHAR_LENGTH 32

void md5_round1(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5_round2(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5_round3(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5_round4(const unsigned int *M, const unsigned int *in, unsigned int *out);
void md5(const unsigned int *in, unsigned int out[4], const size_t size);
void generate_combinations(const unsigned int md5bf[4], const unsigned char charset[CHARSET_LENGTH], unsigned char out[LENGTH], int &found, volatile ap_uint<32> &progress);
long long compute_permutations_with_repetition(int n, int r);
