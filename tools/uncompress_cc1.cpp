#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include <fstream>
#include <string.h>
#include <stack>

inline uint8_t lo(uint16_t value_)
{
	return value_ & 0xFF;
}

inline uint8_t hi(uint16_t value_)
{
	return value_ >> 8;
}

inline uint16_t lo(uint32_t value_)
{
	return value_ & 0xFFFF;
}

inline uint16_t hi(uint32_t value_)
{
	return value_ >> 16;
}

inline uint16_t swap16(const uint16_t value_)
{
	return (value_ << 8) + (value_ >> 8);
}

inline uint32_t swap32(const uint32_t value_)
{
	const uint16_t lv = lo(value_);
	const uint16_t hv = hi(value_);
	return (swap16(lv) << 16) + swap16(hv);
}

std::vector<uint8_t> read_binary_file(const std::string &filename_)
{
	std::ifstream file(filename_, std::ios::binary);
	assert(file);
	return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
	                            std::istreambuf_iterator<char>());
}

struct data_block_t {
	uint32_t unpacked_size{};
	std::vector<uint8_t> packed_data;
};

void read(const uint8_t*& current, void* into, size_t size)
{
	::memcpy(into, current, size);
	current += size;
}

uint16_t read16(const uint8_t*& current)
{
	uint16_t tmp = 0;
	read(current, &tmp, sizeof(tmp));
	return swap16(tmp);
}

uint32_t read32(const uint8_t*& current)
{
	uint32_t tmp = 0;
	read(current, &tmp, sizeof(tmp));
	return swap32(tmp);
}

std::vector<data_block_t> read_cc1_file(const std::string &filepath_)
{
	const std::vector<uint8_t> content = read_binary_file(filepath_);

	const uint8_t *current = content.data();

	const uint16_t offset_count = read16(current);

	std::vector<uint32_t> offset_table(offset_count);
	for (size_t i = 0; i < offset_count; ++i) {
		offset_table[i] = read32(current);
	}

	std::vector<data_block_t> data_blocks(offset_count);

	for (size_t i = 0; i < offset_count; ++i) {
		const uint32_t packed_size = read32(current);
		
		const uint32_t unpacked_size = read32(current);
        
		const std::vector<uint8_t> packed_data(current, current+packed_size);
		current += packed_size;

		data_blocks[i] = {unpacked_size, packed_data};
	}

	// size of the parts + offsets fits exact the file size? no gaps?
	std::size_t result_size = sizeof(uint16_t) + offset_count * sizeof(uint32_t);
	for (size_t i = 0; i < offset_count; ++i) {
		result_size += sizeof(uint32_t) + sizeof(uint32_t) +
		               data_blocks[i].packed_data.size();
	}
	assert(result_size == content.size());

	return data_blocks;
}

constexpr uint8_t LAST_BLOCK     = 0;
constexpr uint8_t NOT_LAST_BLOCK = 1;

#pragma pack(push, 1)
struct block_t {
	uint8_t packed_size{}; // 0xBA9A
	uint8_t flag{}; // 0 = last block, 1 = more blocks comming
	uint16_t data_len{}; // (un)compressed data
};
static_assert(sizeof(block_t) == 4, "invalid size");
#pragma pack(pop)

struct tables_t {
	std::vector<uint8_t> table0;
	std::vector<uint8_t> table1;
	std::vector<uint8_t> table3;
	std::vector<uint8_t> table4;
};

constexpr uint8_t UNPACKED_VAL = 0;

struct stack_item_t {
	uint8_t a{};
	uint8_t b{};
};

void uncompress_part(const uint8_t start_val_, const uint8_t *&input_ptr, uint8_t *&output_ptr, const tables_t& tables)
{
	std::stack<stack_item_t> stack;

	uint8_t var2 = start_val_;

outer_loop:
	assert(var2 > 0);
	stack.push({tables.table1[var2], var2});

	uint8_t var1 = tables.table0[var2];
	assert(var1 >= 0);

	while (true) {
		const uint8_t table3_val = tables.table3[var1];
		assert(table3_val >= 0);

		if (table3_val == UNPACKED_VAL) {
			*output_ptr++ = var1;
			goto end_or_loop;
		} else if (var2 > table3_val) {
			var2 = table3_val;
			goto outer_loop;
		} else {
		        const uint8_t old_var1 = var1;
			const uint8_t old_var2 = var2;
			var2 = table3_val;
			while (true) {
				assert(var2 > 0);
				var2 = tables.table4[var2];
				if (var2 == 0) { // also UNPACKED_VAL?
					*output_ptr++ = old_var1;
					goto end_or_loop;
				} else if (var2 < old_var2) {
					goto outer_loop;
				}
				else
				{
					// get next value from table4
				}
			}
		}

	assert(false); // only gotos reach the end_or_loop label

	end_or_loop:
		if (stack.size() == 0) {
			return;
		}

		const stack_item_t item = stack.top();
		stack.pop();

		// from stack
		var1 = item.a;
		var2 = item.b;
	}
}

tables_t prepare_tables(const block_t& block, const uint8_t *&input_ptr)
{
	assert(block.packed_size != 0);

	// read & prepare uncompress-helper tables
	std::vector<uint8_t> table0(1 + block.packed_size);
	std::vector<uint8_t> table1(1 + block.packed_size);
	std::vector<uint8_t> table2(block.packed_size); // only needed for initialization, not for uncompression
	std::vector<uint8_t> table3(256);
	std::vector<uint8_t> table4(1 + block.packed_size);

	read(input_ptr, table2.data(), block.packed_size);

	table0[0] = 0xFF; // unused, never read
	read(input_ptr, &table0[1], block.packed_size);
	
	table1[0] = 0xFF; // unused, never read
	read(input_ptr, &table1[1], block.packed_size);

	// its currently, unclear what the max-packed_size could be
	// packed_size is uint8_t so max would be 255

	for (int i = 0; i < block.packed_size; ++i) {
		const uint8_t ofs   = table2[i];
		assert(ofs >= 0);
		uint8_t *value      = &table3[ofs]; // [0] is used
		const uint8_t index = i + 1; // (0..255)+1
		table4[index]     = *value; //1+256  [0] ignored, [1-256]
		*value       = index;
	}
	table4[0] = 0xFF; // unused, never read

	return {table0, table1, table3, table4};
}

void uncompress_block(const block_t &block, const uint8_t *&input_ptr, uint8_t *&output_ptr)
{
	const tables_t tables = prepare_tables(block, input_ptr);

	for (int i = 0; i < block.data_len; ++i) 
	{
		const uint8_t var1 = *input_ptr++;
		const uint8_t var2 = tables.table3[var1]; // var1 0..n

		if (var2 == UNPACKED_VAL) {    // uncompressed part
			*output_ptr++ = var1; // just store value
		} else {                      // compressed part
			uncompress_part(var2, input_ptr, output_ptr, tables);
		}
	}
}

std::vector<uint8_t> uncompress(const data_block_t& data_block)
{
	const uint8_t *input_ptr = data_block.packed_data.data();

	std::vector<uint8_t> output(data_block.unpacked_size);
	uint8_t *output_ptr = output.data();

	block_t block{};
	do
	{
		read(input_ptr, &block, sizeof(block));
		assert(block.flag == LAST_BLOCK || block.flag == NOT_LAST_BLOCK);

		if (block.packed_size == 0) { // is not packed?
			read(input_ptr, output_ptr, block.data_len);
			output_ptr += block.data_len;
		} else {
			// biggest block.packed_size so far: 223
			uncompress_block(block, input_ptr, output_ptr);
		}
	}
	while(block.flag != LAST_BLOCK);

	assert(input_ptr == data_block.packed_data.data()+data_block.packed_data.size());
	assert(output_ptr == output.data()+output.size());
	return output; // the-end
}

void write_binary_file(const std::string &file_path_, const void *const data_, size_t size_)
{
	FILE *fp = fopen(file_path_.c_str(), "wb+");
	assert(fp);
	size_t written = fwrite(data_, 1, size_, fp);
	assert(written = size_);
	fclose(fp);
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
	  printf("uncompress_cc1 CC1-FILE\n");
	  return 1;
	}
	
	const std::string cc1_filepath = argv[1];
	
	printf("%s\n", cc1_filepath.c_str());
	std::vector<data_block_t> data_blocks = read_cc1_file(cc1_filepath);

	for (size_t i = 0; i < data_blocks.size(); ++i) {
		const auto &db = data_blocks[i];
		printf("  [%lu] packed_size: %lu, unpacked_size: %u\n", i, db.packed_data.size(), db.unpacked_size);
		
		std::vector<uint8_t> uncompressed = uncompress(db);
		char cc1_block_filepath[1024]{};
		sprintf(cc1_block_filepath, "%s_block%05zu.bin", cc1_filepath.c_str(), i);
		printf("  write: %s\n", cc1_block_filepath);
		write_binary_file(cc1_block_filepath, uncompressed.data(), uncompressed.size());
	}
	
	return 0;
}
