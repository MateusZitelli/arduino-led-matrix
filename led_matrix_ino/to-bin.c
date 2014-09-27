#include <stdio.h>
#define ROW_SIZE 10

/**
 * Converts an integer value to a string given a
 * number of bits to count. Giving bitsCount is
 * important as we are able to always have a
 * string representation with the expected size.
 * @param value
 * @param bitsCount
 * @param str
 */
char * toBin(int value, int bitsCount, char * str)
{
    int i;

    str[bitsCount] = '\0';
    for (i = bitsCount - 1; i >= 0; --i, value >>= 1)
    {
        str[i] = (value & 1) + '0';
    }

    return str;
}

int main(int argc, char const *argv[])
{
	int value = 34;
	int size = ROW_SIZE;
	char repr[ROW_SIZE];

	printf("%s\n", toBin(value, size, repr));

	return 0;
}
