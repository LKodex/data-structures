#include <cstdio>
#include <cstdint>
#include <iostream>
#include "BitBuffer.hpp"

int main()
{
	FILE *original, *copia;
	original = fopen("original.txt", "rb");
	copia = fopen("copia.txt", "wb");

	BitBuffer in(original), out(copia);
	uint8_t bit;
	
	while ((bit = in.get_bit()) != 2) {
		putchar('0' + bit);
		out.put_bit(bit);
	}
	putchar('\n');
	out.write_byte();

	fclose(original);
	fclose(copia);
	
	return 0;
}
  