#include "helper.hpp"

#include <cassert>
#include <fstream>

std::string hexdump(const void* const buffer, int buffer_size, int width)
{
	assert(width > 0);

	/*
AAAAAAAA 00 11 22 33 44 55 66 77 abcdefgh\n
	8+1        width*(2+1)       width   + 1
	*/

	constexpr int address_size = 8 + 1;
	const int hex_size = width * (2 + 1);
	const int ascii_size = width + 1;

	const int full_lines = buffer_size / width;
	const int rest = buffer_size % width;
	const int all_lines = (rest == 0) ? full_lines : full_lines + 1;
	const int result_size = all_lines * (address_size + hex_size + ascii_size);
	std::string result(result_size, ' ');
	char* current_out = &result[0];
	const uint8_t* begin_in = static_cast<const uint8_t*>(buffer);
	const uint8_t* current_in = begin_in;

	auto line = [&result,
		address_size,
		hex_size,
		ascii_size](char*& current_out_,
			const uint8_t* const begin_in_,
			const uint8_t*& current_in_,
			int count_)
	{
		auto hex2 = [](char* const current_out_, uint8_t value_) {
			constexpr char hex[] = "0123456789ABCDEF";
			current_out_[0] = hex[value_ >> 4];
			current_out_[1] = hex[value_ & 0xF];
		};

		auto hex8 = [&](char* const current_out_, int value_) {
			hex2(&current_out_[0], static_cast<uint8_t>(value_ >> 24));
			hex2(&current_out_[2], static_cast<uint8_t>(value_ >> 16));
			hex2(&current_out_[4], static_cast<uint8_t>(value_ >> 8));
			hex2(&current_out_[6], static_cast<uint8_t>(value_ >> 0));
		};

#if 0
		{
			{
				std::string a(8, ' ');
				hex8(a.data(), 0xAABBCCDD);
				assert(a == "AABBCCDD");
			}
			{
				std::string a(8, ' ');
				hex8(a.data(), 0xDDCCBBAA);
				assert(a == "DDCCBBAA");
			}
			{
				std::string a(8, ' ');
				hex8(a.data(), 0x01234567);
				assert(a == "01234567");
			}
		}
#endif

		int address = current_in_ - begin_in_;
		hex8(current_out_, address);
		current_out_ += address_size;

		for (int b = 0; b < count_; ++b) {
			const uint8_t value = current_in_[b];
			hex2(current_out_ + b * (2 + 1), value);
			const char c = (value >= 32 && value <= 126) ? static_cast<char>(value) : '.';
			current_out_[hex_size + b] = c;
		}

		current_out_ += hex_size + count_ + 1;
		current_in_ += count_;
	};

	// full lines
	for (int f = 0; f < full_lines; ++f) {
		line(current_out, begin_in, current_in, width);
		current_out[-1] = '\n';
	}

	// rest
	if (rest) {
		line(current_out, begin_in, current_in, rest);
		current_out[-1] = '\n';
	}

	return result;
}

struct dump_test {
	static void ReplaceStringInPlace(std::string& subject, const std::string& search,
		const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos) {
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}

	static std::string show_ghosts(const std::string& str_)
	{
		std::string result = str_;
		ReplaceStringInPlace(result, "\n", "\\n");
		ReplaceStringInPlace(result, "\t", "\\t");
		ReplaceStringInPlace(result, "\f", "\\f");
		return result;
	}

	dump_test()
	{

		std::vector<uint8_t> blub{ 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
		std::string x = hexdump(blub.data(), blub.size(), 2);
		printf("x:\n<%s>\n<%s>\n", x.c_str(), show_ghosts(x).c_str());
		int brk = 1;
	}
};
#if 0
static dump_test t;
#endif

std::string hex_string(const void* const buffer_, const size_t& size_, bool as_stream_)
{
	if (size_ == 0) {
		return {};
	}

	std::string tmp;
	const size_t byte_width = as_stream_ ? 2 : 3;

	const size_t string_size = (size_ * byte_width) - (as_stream_ ? 0 : 1);
	tmp.resize(string_size, ' ');

	const uint8_t* const in_buffer = reinterpret_cast<const uint8_t*>(buffer_);
	char* const out_buffer = &tmp[0];

	for (size_t i = 0; i < size_; ++i) {
		const char hex_digits[] = "0123456789ABCDEF";
		const uint8_t value = in_buffer[i];
		char* chr = &out_buffer[i * byte_width];
		chr[0] = hex_digits[value >> 4];
		chr[1] = hex_digits[value & 0xF];
	}

	return tmp;
}

std::vector<uint8_t> read_binary_file( const std::string& filename_ )
{
    std::ifstream file( filename_, std::ios::binary );

    return std::vector<uint8_t>( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>() );
}

void write_binary_file( const std::string& file_path_, const void* const data_, size_t size_ )
{
    FILE* fp = fopen( file_path_.c_str(), "wb+" );
    assert( fp );
    size_t written = fwrite( data_, 1, size_, fp );
    assert( written = size_ );
    fclose( fp );
}


