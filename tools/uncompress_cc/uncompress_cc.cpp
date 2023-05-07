#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include <fstream>
#include <string.h>
#include <stack>
#include <algorithm>

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

std::vector<uint8_t> read_binary_file(const std::string& filename_)
{
    std::ifstream file(filename_, std::ios::binary);
    if (!file) {
        throw 1;
    }
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

constexpr uint8_t LAST_BLOCK = 0;
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

static void func1(
    uint8_t*& output_ptr,
    const tables_t& tables,
    const uint8_t index1_,
    const uint8_t val1_,
    const uint8_t index2_);

static void func0(uint8_t*& output_ptr, const tables_t& tables, uint8_t table1_index_)
{
    const uint8_t table0_val = tables.table0[table1_index_];
    const uint8_t table1_val = tables.table1[table1_index_];
    func1(output_ptr, tables, table1_index_, table0_val, table0_val);
    func1(output_ptr, tables, table1_index_, table0_val, table1_val);
}

static void func1(
    uint8_t*& output_ptr,
    const tables_t& tables,
    const uint8_t index1_,
    const uint8_t val1_,
    const uint8_t table3_index_
)
{
    //assert(val1_ == table3_index_); // not always

    const uint8_t val2 = tables.table3[table3_index_];

    if (val2 == UNPACKED_VAL)
    {
        *output_ptr++ = table3_index_;
        return;
    }

    if (index1_ > val2)
    {
        func0(output_ptr, tables, val2);
        return;
    }

    //else

    {
        uint8_t table4_index = val2;
        while (true)
        {
            const uint8_t table4_val = tables.table4[table4_index];

            if (table4_val == UNPACKED_VAL)
            {
                *output_ptr++ = table3_index_;
                return;
            }

            if (table4_val < index1_)
            {
                func0(output_ptr, tables, table4_val);
                return;
            }

            table4_index = table4_val;
        }
    }
}

void uncompress_part(const uint8_t start_val_, uint8_t*& output_ptr, const tables_t& tables)
{
    func0(output_ptr, tables, start_val_);
}

tables_t prepare_tables(const block_t& block, const uint8_t*& input_ptr)
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
        const uint8_t ofs = table2[i];
        assert(ofs >= 0);
        uint8_t* value = &table3[ofs]; // [0] is used
        const uint8_t index = i + 1; // (0..255)+1
        table4[index] = *value; //1+256  [0] ignored, [1-256]
        *value = index;
    }
    table4[0] = 0xFF; // unused, never read

    return { table0, table1, table3, table4 };
}

void uncompress_block(const block_t& block, const uint8_t*& input_ptr, uint8_t*& output_ptr)
{
    const tables_t tables = prepare_tables(block, input_ptr);

    for (int i = 0; i < block.data_len; ++i)
    {
        const uint8_t var1 = *input_ptr++;
        const uint8_t var2 = tables.table3[var1]; // var1 0..n

        if (var2 == UNPACKED_VAL) {    // uncompressed part
            //printf("    if1 - just store: %02X\n", var1);
            *output_ptr++ = var1; // just store value
        }
        else {                      // compressed part
            //printf("    if2 - uncompress_part: start_value: %02X\n", var2);
            uncompress_part(var2, output_ptr, tables);
        }
    }
}

std::vector<uint8_t> uncompress(const data_block_t& data_block)
{
    const uint8_t* input_ptr = data_block.packed_data.data();

    const uint8_t* input_end_ptr = input_ptr + data_block.packed_data.size();

    std::vector<uint8_t> output(data_block.unpacked_size);
    uint8_t* output_ptr = output.data();

    block_t block{};
    do
    {
        if ((input_end_ptr - input_ptr) < sizeof(block))
        {
            throw 4;
        }
        read(input_ptr, &block, sizeof(block));
        assert(block.flag == LAST_BLOCK || block.flag == NOT_LAST_BLOCK);

        //printf("block: packed_size: %u, flag: %u, data_len: %u\n", block.packed_size, block.flag, block.data_len);

        if (block.packed_size == 0) { // is not packed?
            //printf("  not packed: data_len: %u\n", block.data_len);
            // assert(block.data_len == 0); always ok in GRAPHS.CC1, not for PROGS.CC1
            read(input_ptr, output_ptr, block.data_len);
            output_ptr += block.data_len;
        }
        else {
            //printf("  packed\n");
            // biggest block.packed_size so far: 223
            uncompress_block(block, input_ptr, output_ptr);
        }
    } while (block.flag != LAST_BLOCK);

    assert(input_ptr == data_block.packed_data.data() + data_block.packed_data.size());
    assert(output_ptr == output.data() + output.size());
    return output; // the-end
}

void write_binary_file(const std::string& file_path_, const void* const data_, size_t size_)
{
    FILE* fp = fopen(file_path_.c_str(), "wb+");
    assert(fp);
    size_t written = fwrite(data_, 1, size_, fp);
    assert(written = size_);
    fclose(fp);
}

int main(int argc, char* argv[])
{
    if ((argc < 2) || (argc > 3))
    {
        printf("uncompress_cc1 CC-FILE [e]\n");
        return -1;
    }

    const std::string cc_filepath = argv[1];
    printf("%s\n", cc_filepath.c_str());

    bool extract = false;
    if (argc == 3)
    {
        const std::string arg2 = argv[2];
        if (arg2 != "e")
        {
            printf("optional second argument needs to be 'e'\n");
            return -1;
        }
        extract = true;
    }

    try
    {
        const std::vector<uint8_t> content = read_binary_file(cc_filepath);
        const uint8_t* current = content.data();
        const uint8_t* end = current + content.size();

        if (content.size() < 2)
        {
            return 2;
        }

        const uint16_t offset_count = read16(current);

        if (offset_count == 0)
        {
            printf("info: likely CC0 format\n");
            // compressed size?
            const uint16_t packed_size = read16(current);
            if (packed_size >= content.size())
            {
                printf("error: is not a CC0: packed size >= file size\n");
                return -1;
            }
            assert(packed_size < content.size());

            // uncompressed?
            const uint32_t unpacked_size = read32(current);

            const size_t data_size = content.size() - (current - content.data());
            if (data_size != packed_size)
            {
                printf("error: data_size != packed_size\n");
                return -2;
            }
            assert(data_size == packed_size);
            std::vector<uint8_t> data(data_size);
            read(current, data.data(), data_size);

            data_block_t db;
            db.unpacked_size = unpacked_size;
            db.packed_data = data;

            const std::vector<uint8_t> uncompressed = uncompress(db);

            if (extract)
            {
                char cc0_filepath[1024]{};
                sprintf(cc0_filepath, "%s.uncompressed.bin", cc_filepath.c_str());
                printf("  write: %s\n", cc0_filepath);
                write_binary_file(cc0_filepath, uncompressed.data(), uncompressed.size());
            }
            return 0;
        }

        printf("info: likely CC1 format\n");
        std::vector<data_block_t> data_blocks(offset_count);
        {
            std::vector<uint32_t> offset_table(offset_count);
            for (size_t i = 0; i < offset_count; ++i) {
                if (current + 4 > end)
                {
                    throw 2;
                }
                offset_table[i] = read32(current);
            }

            auto sorted_table = offset_table;
            std::sort(sorted_table.begin(), sorted_table.end());

            if (sorted_table != offset_table)
            {
                printf("info: not ascending offsets\n");
            }

            for (size_t i = 0; i < offset_count; ++i) {
                const uint32_t packed_size = read32(current);
                if (packed_size >= content.size())
                {
                    printf("error: packed size >= file size\n");
                    return -1;
                }
                assert(packed_size < content.size());

                const uint32_t unpacked_size = read32(current);

                const std::vector<uint8_t> packed_data(current, current + packed_size);
                current += packed_size;

                data_blocks[i] = { unpacked_size, packed_data };
            }

            // size of the parts + offsets fits exact the file size? no gaps?
            std::size_t result_size = sizeof(uint16_t) + offset_count * sizeof(uint32_t);
            for (size_t i = 0; i < offset_count; ++i) {
                result_size += sizeof(uint32_t) + sizeof(uint32_t) +
                    data_blocks[i].packed_data.size();
            }
            if (result_size != content.size())
            {
                printf("info: gaps between blocks\n");
            }
        }

        for (size_t i = 0; i < data_blocks.size(); ++i) {
            const auto& db = data_blocks[i];

            if (db.packed_data.size() == 0)
            {
                //assert(false);
                //throw 5; // is that ok?
                // not packed block?
            }
            if (db.unpacked_size == 0)
            {
                throw 6;
            }
            printf("  [%zu] packed_size: %zu, unpacked_size: %u\n", i, db.packed_data.size(), db.unpacked_size);

            const std::vector<uint8_t> uncompressed = uncompress(db);

            if (extract)
            {
                char cc1_block_filepath[1024]{};
                sprintf(cc1_block_filepath, "%s_block%05zu.bin", cc_filepath.c_str(), i);
                printf("  write: %s\n", cc1_block_filepath);
                write_binary_file(cc1_block_filepath, uncompressed.data(), uncompressed.size());
            }
        }

        return 0;
    }
    catch (int e)
    {
        return e;
    }
    return 0;
}
