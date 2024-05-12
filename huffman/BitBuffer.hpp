#include <cstdint>

class BitBuffer {
	public:
		BitBuffer(FILE *file);
		uint8_t free_bits();

		// Write mode
		void put_bit(uint8_t bit);
		void write_byte();

		// Read mode
		uint8_t get_bit();

	private:
		FILE *file;
		uint8_t buffer;
		uint8_t size;
};
  
BitBuffer::BitBuffer(FILE *file) :
	file(file),
	size(0),
	buffer(0)
{ }

void BitBuffer::put_bit(uint8_t bit)
{
	uint8_t new_bit = bit & 0b1;
	buffer <<= 1;
	buffer |= new_bit;
	size++;
	if (size == 8) write_byte();
}

void BitBuffer::write_byte()
{
	const uint8_t SIZE = 1;	// byte size in bytes
	const uint8_t QUANTITY = 1; // quantity of bytes to be read
	
	bool isBufferEmpty = size == 0;
	if (isBufferEmpty)
	{
		return;
	}

	buffer <<= free_bits();
	fwrite(&buffer, SIZE, QUANTITY, file);
	size = 0;
	buffer = 0b0;
}

uint8_t BitBuffer::get_bit()
{
	bool isBufferEmpty = size == 0;
	if (isBufferEmpty)
	{
		fread(&buffer, 1, 1, file);
		size = 8;
	}

	bool isEOF = feof(file) != 0;
	if (isEOF)
	{
		const uint8_t _EOF = 0b10; // end of file return value
		return _EOF;
	}

	bool isReadError = ferror(file) != 0;
	if (isReadError)
	{
		const uint8_t _ERROR = 0b11; // error return value
		return _ERROR;
	}

	uint8_t read_bit = buffer & 0b1;
	buffer >>= 1;
	size--;
	return read_bit;
}

uint8_t BitBuffer::free_bits()
{
	return 8 - size;
}
