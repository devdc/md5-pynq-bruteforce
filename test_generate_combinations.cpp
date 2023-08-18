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
    const int LEN = LENGTH;
//	const unsigned char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@";
	const unsigned char charset[] = "AabCcdefghijklmnopqrstuvwxyz01234567";

//	const unsigned char charset[] = "abcdef";

    unsigned char out[LEN];
    const unsigned char md5bf[] = "47bce5c74f589f4867dbd57e9ca9f808"; // hashed "aaa"
    const unsigned char md5bf_2[] = "5a89924ad353f0f0d5b7bf6ee8e0909b"; // "cca"
    const unsigned char md5bf_3[] = "43b5688a505003ebae686e46c866eca7"; // "AbCdE5!"
    const unsigned char md5bf_4[] = "4506ca152a692ca44a41998450f583f4"; // AbCd
    printf("calling generate_combinations!\n");
    generate_combinations(md5bf_4, charset, out);
    printf("result: \n");
    for (int i = 0; i < LEN; i++) {
		printf("%c", out[i]);
	}
    printf("\n");
    return 0;
}
