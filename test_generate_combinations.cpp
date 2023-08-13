#include "generate_combinations.h"


void print_arr(unsigned char* arr, int length)
{
    for(unsigned char* p=arr; p<arr+length; ++p) printf("%02x",*p);
    printf("\n");
}

int main()
{
    const unsigned char charset[] = "abc2";
    int length = 4;
    unsigned char out[4];
    const unsigned char md5bf[] = "47bce5c74f589f4867dbd57e9ca9f808"; // hashed "aaa"
    const unsigned char md5bf_2[] = "bcbe3365e6ac95ea2c0343a2395834dd"; // "cca"
    generate_combinations(md5bf_2, charset, out);
    for (int i = 0; i < 4; i++) {
		printf("%c", out[i]);
	}
    return 0;
}
