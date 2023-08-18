#include "generate_combinations.h"



void generate_next_combination(bool &carry, unsigned char *current, const unsigned char *charset);
bool is_md5_match(const unsigned char *in_1, unsigned char *in_2);
void copy_pad(char *padded, unsigned char *c, size_t length, unsigned int &padded_size);
void copy_uchar(unsigned char *to, unsigned char *from, size_t length);


void generate_combinations(const unsigned char md5bf[MD5_CHAR_LENGTH], const unsigned char charset[CHARSET_LENGTH], unsigned char out[LENGTH])
{

#pragma HLS INTERFACE s_axilite port=md5bf
#pragma HLS INTERFACE s_axilite port=charset
#pragma HLS INTERFACE s_axilite port=out
#pragma HLS INTERFACE s_axilite port=return


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
    long long max_iterations = compute_permutations_with_repetition(n, k);

    for (long long i = 0; i < max_iterations; i++)
    {
        // pad data
        char padded[BUFFER_SIZE];
        unsigned int padded_size;
        copy_pad(padded, current, LENGTH, padded_size);
        unsigned int md5out[16];
        md5((unsigned int*)padded, md5out, padded_size);

        unsigned char md5out_hex[MD5_CHAR_LENGTH];
        byte_to_hex_string((unsigned char*)md5out, MD5_CHAR_LENGTH, md5out_hex);
        bool match = is_md5_match(md5bf, md5out_hex);
        if (match)
        {
        	copy_uchar(out, current, LENGTH);
        	break;
        }
        // Generate next combination on current
        bool carry;
        generate_next_combination(carry, current, charset);
        if (carry) {
            break;
        }

    }
}

void generate_next_combination(bool &carry, unsigned char *current, const unsigned char *charset) {
#pragma HLS INLINE
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

bool is_md5_match(const unsigned char *in_1, unsigned char *in_2) {
	bool match = true;
	for (int i=0; i<MD5_CHAR_LENGTH; i++)
	{
		if (in_1[i] != in_2[i])
		{
			match = false;
			break;
		}
	}
	return match;
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

void byte_to_hex_string(unsigned char* bytes, size_t length, unsigned char* hexString) {
    // Characters for hexadecimal representation
    const char hex_chars[] = "0123456789abcdef";

    for (size_t i = 0; i < length; ++i) {
        // Convert the high nibble (first 4 bits) to hex
        hexString[i * 2] = hex_chars[(bytes[i] >> 4) & 0x0F];

        // Convert the low nibble (last 4 bits) to hex
        hexString[i * 2 + 1] = hex_chars[bytes[i] & 0x0F];
    }

    // Null-terminate the string
    hexString[length * 2] = '\0';
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

