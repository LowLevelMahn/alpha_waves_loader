#include "cc_file.hpp"

#include "helper.hpp"
#include "cc_algorithm.hpp"

#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm> // std::sort,...

struct packed_block_t {
    uint32_t unpacked_size{};
    std::vector<uint8_t> packed_data;
};

static packed_block_t read_packed_block(const uint8_t*& current_, const uint8_t* const end_)
{
    if (left(current_, end_) < 8)
    {
        throw 14;
    }

    const uint32_t packed_size = read32(current_);
    const uint32_t unpacked_size = read32(current_);

    if (left(current_, end_) < packed_size)
    {
        throw 15;
    }

    std::vector<uint8_t> packed_data(packed_size);
    read(current_, packed_data.data(), packed_size);

    return { unpacked_size , std::move(packed_data) };
}

#if 0

// port to stream based - future use on file reading cleanup

static packed_block_t read_packed_block2(stream_reader_t& input_reader_)
{
    if (input_reader_.left() < 8)
    {
        throw 14;
    }

    const uint32_t packed_size = input_reader_.read_uint32_be();
    const uint32_t unpacked_size = input_reader_.read_uint32_be();

    if (input_reader_.left() < packed_size)
    {
        throw 15;
    }

    const std::vector<uint8_t> packed_data = input_reader_.read_vector(packed_size);

    return { unpacked_size , std::move(packed_data) };
}

#endif

int cc0_uncompress(const std::string& cc_filepath, const std::vector<uint8_t>& content, const bool extract)
{
    try
    {
#if 0
        {
            stream_reader_t input_reader(content.data(), content.data() + content.size());
            const packed_block_t packed_block = read_packed_block2(input_reader);
        }
#endif

        const uint8_t* current = content.data();
        const uint8_t* end = current + content.size();

        const packed_block_t packed_block = read_packed_block(current, end);
        if (left(current, end) != 0) // no left bytes?
        {
            return 3;
        }

        const std::vector<uint8_t> uncompressed = uncompress(packed_block.packed_data, packed_block.unpacked_size);

        if (extract)
        {
            char cc0_filepath[1024]{};
            sprintf(cc0_filepath, "%s.uncompressed.bin", cc_filepath.c_str());
            printf("  write: %s\n", cc0_filepath);
            write_binary_file(cc0_filepath, uncompressed.data(), uncompressed.size());
        }
    }
    catch (...)
    {
        return 30;
    }

    return 0;
};

std::vector<std::string> filenames_after;

int cc1_uncompress(const std::string& cc_filepath, const std::vector<uint8_t>& content, const bool extract)
{
    try
    {
        /*
        ---- header
        uint16 offset_count
        uint32 offset_table[offset_count]
        ---- data
        ...
        data + offset_table[x]
           packed_block
        ...
        data + offset_table[y]
           packed_block
        ...
        data + offset_table[z]
           packed_block
        ...
        */

#if 0
        // unsing stream reader
        {
            stream_reader_t input_reader(content.data(), content.data() + content.size());

            if (!input_reader.fits(2))
            {
                return 2;
            }

            const uint16_t offset_count = input_reader.read_uint16_be();

            if (offset_count == 0)
            {
                printf("no offset count!!!\n");
                return 27;
            }

            printf("offset_count: %u\n", offset_count);
        }
#endif

        const uint8_t* current = content.data();
        const uint8_t* end = current + content.size();

        if (left(current, end) < 2)
        {
            return 2;
        }

        const uint16_t offset_count = read16(current);

        if (offset_count == 0)
        {
            printf("no offset count!!!\n");
            return 27;
        }

        printf("offset_count: %u\n", offset_count);

        std::vector<packed_block_t> data_blocks(offset_count);
        {
            std::vector<uint32_t> offset_table(offset_count);
            for (size_t i = 0; i < offset_count; ++i) {

                if (left(current, end) < 4)
                {
                    return 3;
                }

                const auto offset = read32(current);
                offset_table[i] = offset;

                printf("offset[%zu]: %u\n", i, offset);
            }

            if (offset_table.empty())
            {
                printf("no offsets!!!\n");
                return 22;
            }

            auto sorted_table = offset_table;
            std::sort(sorted_table.begin(), sorted_table.end());

            if (sorted_table != offset_table)
            {
                // never seen with my test data
                printf("not ascending offsets\n");
            }

            // header-size + block.offset = block.begin
            const uint8_t* const data_start = current;
            for (size_t i = 0; i < offset_count; ++i)
            {
                if ((data_start + offset_table[i]) >= end)
                {
                    throw 44;
                }
                current = data_start + offset_table[i];
                data_blocks[i] = read_packed_block(current, end);

                printf("data_blocks: packed_size: %zu, unpacked_size: %u\n", data_blocks[i].packed_data.size(), data_blocks[i].unpacked_size);
            }

            {
                int last = sorted_table[0];
                for (size_t i = 1; i < sorted_table.size(); ++i)
                {
                    const int distance = sorted_table[i] - last;

                    const int expected_distance = sizeof(uint32_t) + sizeof(uint32_t) +
                        data_blocks[i - 1].packed_data.size();

                    printf("distance: %i, expected-distance: %i\n", distance, expected_distance);
                    if (distance != expected_distance)
                    {
                        printf("gap between packed blocks\n");
                    }

                    last = sorted_table[i];
                }
            }

            {
                // size of the parts + offsets fits exact the file size? no gaps?
                std::size_t result_size = sizeof(uint16_t) + offset_count * sizeof(uint32_t);
                for (size_t i = 0; i < offset_count; ++i) {
                    result_size += sizeof(uint32_t) + sizeof(uint32_t) +
                        data_blocks[i].packed_data.size();
                }
                if (result_size != content.size())
                {
                    size_t last_end = 0;
                    for (size_t i = 0; i < offset_count; ++i)
                    {
                        int offset = offset_table[i];
                        int gap = offset - last_end;
                        assert(gap == 0);
                        int block_size = 2 * sizeof(uint32_t) + data_blocks[i].packed_data.size();
                        int block_end = offset + block_size;
                        printf("gap: %i, offset: %u, block_size: %u, block_end: %i\n", gap, offset_table[i], block_size, block_end);
                        last_end = block_end;
                    }

                    if (last_end < content.size() - sizeof(uint16_t) + offset_count * sizeof(uint32_t))
                    {
                        // only Mystical: MYSJEUCG.1 and MYSJEUEG.2

                        printf("%zu bytes remaining after last block\n", left(content.data() + last_end, end));

                        const uint8_t* const rest_begin = content.data() + last_end;
                        size_t rest_size = content.data() + content.size() - rest_begin;
                        printf("%s\n", hexdump(rest_begin, rest_size, 16).c_str());

                        if (extract)
                        {
                            char cc1_rest_filepath[1024]{};
                            sprintf(cc1_rest_filepath, "%s_rest.bin", cc_filepath.c_str());
                            printf("  write: %s\n", cc1_rest_filepath);
                            write_binary_file(cc1_rest_filepath, rest_begin, rest_size);
                        }

                        {
                            // from back to front
                            size_t fileinfo_table_offset = rest_size - offset_count * 84;

                            {
                                // no idea what the 4 or 6 bytes are, or in what order, or splitted
                                const size_t calculated_table_offset = (offset_count * 4) + 6;
                                assert(fileinfo_table_offset == calculated_table_offset);
                            }

#if 0
                            const uint8_t* const fileinfo_table_begin = rest_begin + fileinfo_table_offset;
                            current = fileinfo_table_begin;
#else
                            current = rest_begin;
                            for (size_t i = 0; i < offset_count; ++i)
                            {
                                uint32_t unknown0_0 = read16(current);
                                uint32_t unknown0_1 = read16(current);
                                printf("unknown0_0: %u, unknown0_1: %u\n", unknown0_0, unknown0_1);
                            }
                            std::vector<uint8_t> unknown1(6);
                            read(current, unknown1.data(), unknown1.size());
                            printf("unknown1:\n%s\n", hexdump(unknown1.data(), unknown1.size(), unknown1.size()).c_str());
#endif

                            for (size_t i = 0; i < offset_count; ++i)
                            {
                                std::vector<uint8_t> fileinfo(84);
                                read(current, fileinfo.data(), fileinfo.size());
                                printf("%s\n", hexdump(fileinfo.data(), fileinfo.size(), fileinfo.size()).c_str());
                            }

                            filenames_after.push_back(cc_filepath);
                        }
                    }

                }
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
                return 6;
            }

            printf("  [%zu] packed_size: %zu, unpacked_size: %u\n", i, db.packed_data.size(), db.unpacked_size);

            const std::vector<uint8_t> uncompressed = uncompress(db.packed_data, db.unpacked_size);

            if (extract)
            {
                char cc1_block_filepath[1024]{};
                sprintf(cc1_block_filepath, "%s_uncompressed_block%05zu.bin", cc_filepath.c_str(), i);
                printf("  write: %s\n", cc1_block_filepath);
                write_binary_file(cc1_block_filepath, uncompressed.data(), uncompressed.size());
            }
        }

        return 0;
    }
    catch (...)
    {
        return 20;
    }

    return 0;
}
