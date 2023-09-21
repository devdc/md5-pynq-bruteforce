#include "generate_combinations.h"

//void print_md5(unsigned char* md5);
//
//void print_md5(unsigned char* md5) {
//    for(unsigned char* p=md5; p<md5+16; ++p) printf("%02x",*p);
//    printf("\n");
//}

void print_arr(unsigned char* arr, int length)
{
    for(unsigned char* p=arr; p<arr+length; ++p) printf("%02x",*p);
    printf("\n");
}

int main()
{
    int found = 0;
    volatile ap_uint<32> progress = 0;
//	const unsigned char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@ ";
//	const unsigned char charset[] = "abcd";
	const unsigned char charset[] = "AabCcdefghijklmnopqrsTuvwxyz0123456 ";

//	const unsigned char charset[] = "abcdef";

    unsigned char out[LENGTH];
    const unsigned char md5bf[] = "47bce5c74f589f4867dbd57e9ca9f808"; // hashed "aaa"
    const unsigned char md5bf_2[] = "5a89924ad353f0f0d5b7bf6ee8e0909b"; // "cca"
    const unsigned int md5bf_2_int[] = {0x5a89924a, 0xd353f0f0, 0xd5b7bf6e, 0xe8e0909b}; // "cca"

    const unsigned char md5bf_3[] = "43b5688a505003ebae686e46c866eca7"; // "AbCdE5!"
    const unsigned char md5bf_4[] = "4506ca152a692ca44a41998450f583f4"; // "AbCd"
    const unsigned int md5bf_4_int[] = {0x4506ca15, 0x2a692ca4, 0x4a419984, 0x50f583f4};
    const unsigned int md5bf_5_int[] = {0xc073d0a5, 0x8f4d14d1, 0x3e400d3a, 0x504339c3}; // "This is me"
    const unsigned int md5bf_6_int[] = {0x3dc158c6, 0xd918aa48, 0x1af72ef8, 0x34c93ea8}; // "AbCd2"
    printf("calling generate_combinations!\n");
    generate_combinations(md5bf_6_int, charset, out, found, progress);
    printf("result: \n");
    for (int i = 0; i < LENGTH; i++) {
		printf("%c", out[i]);
	}
    printf("\n");
    return 0;
}
