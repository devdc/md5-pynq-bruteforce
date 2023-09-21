#include "generate_combinations.h"



void generate_next_combination(bool &carry, unsigned char *current, const unsigned char *charset);
void copy_pad(char *padded, unsigned char *c, size_t length, unsigned int &padded_size);
void copy_uchar(unsigned char *to, unsigned char *from, size_t length);
unsigned int reorderBytes(unsigned int value);


void generate_combinations(const unsigned int md5bf[4], const unsigned char charset[CHARSET_LENGTH], unsigned char out[LENGTH], int &found, volatile ap_uint<32> &progress)
{

#pragma HLS INTERFACE s_axilite port=progress
#pragma HLS INTERFACE s_axilite port=md5bf
#pragma HLS INTERFACE s_axilite port=charset
#pragma HLS INTERFACE s_axilite port=out
#pragma HLS INTERFACE s_axilite port=found
#pragma HLS INTERFACE s_axilite port=return
//#pragma HLS pipeline II=1 enable_flush


	unsigned int md5bf_reversed[4];
	for (int i=0; i<4; i++) {
		md5bf_reversed[i] = reorderBytes(md5bf[i]);
	}

	found = 0;
//	progress_t progress_data;
    unsigned char current[LENGTH];
    // initialize the first sequence of characters
    for (int i = 0; i < LENGTH; i++)
    {
        current[i] = charset[0];
    }

    // from all the items
    int n = CHARSET_LENGTH;
    // choose k items
	int k = LENGTH;
    ap_uint<32> max_iterations = compute_permutations_with_repetition(n, k);
//    ap_uint<32> cycle = max_iterations / 5;
    ap_uint<32> int_progress = 0;
    for (ap_uint<32> i = 0; i < max_iterations; i++)
    {

        // pad data
        char padded[BUFFER_SIZE];
        unsigned int padded_size;
        copy_pad(padded, current, LENGTH, padded_size);

        unsigned int md5out[4];
		md5((unsigned int*)padded, md5out, padded_size);

		bool match = true;
		for (int i=0; i < 4; i++) {
			if (md5bf_reversed[i] != md5out[i]) {
				match = false;
				break;
			}
		}


        if (match)
        {
        	copy_uchar(out, current, LENGTH);
        	found = 1;
        	// if found send last signal
//        	progress = i;
//        	progress_data.last = true;
//        	progress << progress_data;
        	break;
        }
        // Generate next combination on current
        bool carry;
        generate_next_combination(carry, current, charset);
        if (carry) {
            break;
        }


//        if (i == 10 || i % cycle == 0) {
			// Send progress data
//			progress = i;
//			progress_data.last = 0;
//			progress << progress_data;
//		}
//        if (i == max_iterations-1) {
//        	progress = i;
        	// Signal the end of the stream
//        	progress_data.last = true;
//        	progress << progress_data;
//        }
        // Send progress data
		int_progress++;
		progress = int_progress;
    }
}

unsigned int reorderBytes(unsigned int value) {
    return ((value & 0x000000FF) << 24) |  // Move the last byte to the first
           ((value & 0x0000FF00) << 8)  |  // Move the third byte to the second
           ((value & 0x00FF0000) >> 8)  |  // Move the second byte to the third
           ((value & 0xFF000000) >> 24);   // Move the first byte to the last
}

void generate_next_combination(bool &carry, unsigned char *current, const unsigned char *charset) {
	carry = true;
	for (int i = LENGTH - 1; i >= 0; --i) {
		if (carry) {
			int idx = 0;
			for (int j = 0; j < CHARSET_LENGTH; j++) {
				if (charset[j] == current[i]) {
					idx = j;
					break;
				}
			}
			idx = (idx + 1) % CHARSET_LENGTH;
			if (idx == 0) {
				carry = true;
			}
			else {
				carry = false;
			}
			current[i] = charset[idx];
		}
	}
}


void copy_pad(char *padded, unsigned char *c, size_t length, unsigned int &padded_size) {
	padded_size = 64;
	unsigned int len = length;
	unsigned int mod = (len + 1)%64;
	unsigned int pad_size = (64+56-mod)%64;
	padded_size = len+1+pad_size+8;

	for (unsigned int i = 0; i < len; i++)
	{
		padded[i] = c[i];
	}
	padded[len] = (char)0x80;
	for (unsigned int i = len + 1; i < len+1+pad_size; i++)
	{
		padded[i] = 0x0;
	}

	// Convert bytes to bits and append length
	len <<= 3;
	for(char* p = padded+(padded_size-8); p < padded+padded_size; ++p){
		*p = len & 0xff;
		len >>= 8;
	}
}

void copy_uchar(unsigned char *to, unsigned char *from, size_t length) {
	for (size_t i = 0; i < length; i++) {
		to[i] = from[i];
	}
}


// HLS function to compute the number of permutations with repetition
long long compute_permutations_with_repetition(int n, int r) {
    long long total_permutations = 1;
    for(int i = 0; i < r; ++i) {
        total_permutations *= n;
    }
    return total_permutations;
}

const unsigned int K[] = {
    0XD76AA478,
    0XE8C7B756,
    0X242070DB,
    0XC1BDCEEE,
    0XF57C0FAF,
    0X4787C62A,
    0XA8304613,
    0XFD469501,
    0X698098D8,
    0X8B44F7AF,
    0XFFFF5BB1,
    0X895CD7BE,
    0X6B901122,
    0XFD987193,
    0XA679438E,
    0X49B40821,
    0XF61E2562,
    0XC040B340,
    0X265E5A51,
    0XE9B6C7AA,
    0XD62F105D,
    0X02441453,
    0XD8A1E681,
    0XE7D3FBC8,
    0X21E1CDE6,
    0XC33707D6,
    0XF4D50D87,
    0X455A14ED,
    0XA9E3E905,
    0XFCEFA3F8,
    0X676F02D9,
    0X8D2A4C8A,
    0XFFFA3942,
    0X8771F681,
    0X6D9D6122,
    0XFDE5380C,
    0XA4BEEA44,
    0X4BDECFA9,
    0XF6BB4B60,
    0XBEBFBC70,
    0X289B7EC6,
    0XEAA127FA,
    0XD4EF3085,
    0X04881D05,
    0XD9D4D039,
    0XE6DB99E5,
    0X1FA27CF8,
    0XC4AC5665,
    0XF4292244,
    0X432AFF97,
    0XAB9423A7,
    0XFC93A039,
    0X655B59C3,
    0X8F0CCC92,
    0XFFEFF47D,
    0X85845DD1,
    0X6FA87E4F,
    0XFE2CE6E0,
    0XA3014314,
    0X4E0811A1,
    0XF7537E82,
    0XBD3AF235,
    0X2AD7D2BB,
    0XEB86D391
};

const unsigned char S[][16] = {
    {7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22},
    {5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20},
    {4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23},
    {6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21}
};


void md5_round1(const unsigned int *M, const unsigned int *in, unsigned int *out)
{
    unsigned int A = in[0];
    unsigned int B = in[1];
    unsigned int C = in[2];
    unsigned int D = in[3];


    for(int i=0; i<16; ++i){
        unsigned int tmp0 = (B&C)|(~B&D);
        unsigned int tmp1 = A + tmp0;
        unsigned int tmp2 = M[i] + K[i] + tmp1;
        unsigned int tmp3 = (tmp2 << S[0][i]) | (tmp2 >> (32-S[0][i]));
        A = D;
        D = C;
        C = B;
        B = tmp3 + B;

    }
    out[0] = A;
    out[1] = B;
    out[2] = C;
    out[3] = D;
}

void md5_round2(const unsigned int *M, const unsigned int *in, unsigned int *out)
{
    unsigned int A = in[0];
    unsigned int B = in[1];
    unsigned int C = in[2];
    unsigned int D = in[3];
    for(int i=0; i<16; ++i){
        unsigned int tmp0 = (B&D)|(C&~D);
        unsigned int tmp1 = A + tmp0;
        // unsigned int tmp2 = M[16+i] + K[16+i] + tmp1;
        unsigned int tmp2 = M[(((i+16) * 5) + 1) % 16] + K[16+i] + A + tmp0;
        unsigned int tmp3 = (tmp2 << S[1][i]) | (tmp2 >> (32-S[1][i]));
        A = D;
        D = C;
        C = B;
        B = tmp3 + B;

    }
    out[0] = A;
    out[1] = B;
    out[2] = C;
    out[3] = D;
}

void md5_round3(const unsigned int *M, const unsigned int *in, unsigned int *out)
{
    unsigned int A = in[0];
    unsigned int B = in[1];
    unsigned int C = in[2];
    unsigned int D = in[3];
    for(int i=0; i<16; ++i){
        unsigned int tmp0 = B^C^D;
        unsigned int tmp1 = A + tmp0;
        unsigned int tmp2 = M[(((i+32) * 3) + 5) % 16] + K[32+i] + tmp1;
        unsigned int tmp3 = (tmp2 << S[2][i]) | (tmp2 >> (32-S[2][i]));
        A = D;
        D = C;
        C = B;
        B = tmp3 + B;

    }
    out[0] = A;
    out[1] = B;
    out[2] = C;
    out[3] = D;
}

void md5_round4(const unsigned int *M, const unsigned int *in, unsigned int *out)
{
    unsigned int A = in[0];
    unsigned int B = in[1];
    unsigned int C = in[2];
    unsigned int D = in[3];
    for(int i=0; i<16; ++i){
        unsigned int tmp0 = C^(B|~D);
        unsigned int tmp1 = A + tmp0;
        unsigned int tmp2 = M[((i+48) * 7) % 16] + K[48+i] + tmp1;
        unsigned int tmp3 = (tmp2 << S[3][i]) | (tmp2 >> (32-S[3][i]));
        A = D;
        D = C;
        C = B;
        B = tmp3 + B;

    }
    out[0] = A;
    out[1] = B;
    out[2] = C;
    out[3] = D;
}

void md5(const unsigned int *in, unsigned int *out, const size_t size)
{

    const unsigned int *M = in;
    unsigned int v0[] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476};
    unsigned int v1[4];
    unsigned int v2[4];
    unsigned int v3[4];
    unsigned int v4[4];

    for(size_t i=0; i<size; i+=64)
    {
        md5_round1(M, v0, v1);
        md5_round2(M, v1, v2);
        md5_round3(M, v2, v3);
        md5_round4(M, v3, v4);

        M += 16;
        v0[0] += v4[0];
        v0[1] += v4[1];
        v0[2] += v4[2];
        v0[3] += v4[3];
    }
    out[0] = v0[0];
    out[1] = v0[1];
    out[2] = v0[2];
    out[3] = v0[3];
}

