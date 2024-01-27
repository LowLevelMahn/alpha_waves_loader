#include "helper.hpp"

#include <fstream>
#include <cassert>

uint8_t lo(const uint16_t value_)
{
    return value_ & 0xFF;
}

uint8_t hi(const uint16_t value_)
{
    return value_ >> 8;
}

uint16_t lo(const uint32_t value_)
{
    return value_ & 0xFFFF;
}

uint16_t hi(const uint32_t value_)
{
    return value_ >> 16;
}

uint16_t swap16(const uint16_t value_)
{
    return (value_ << 8) + (value_ >> 8);
}

uint32_t swap32(const uint32_t value_)
{
    const uint16_t lv = lo(value_);
    const uint16_t hv = hi(value_);
    return (swap16(lv) << 16) + swap16(hv);
}

uint16_t swap(const uint16_t value_)
{
    return swap16(value_);
}

uint32_t swap(const uint32_t value_)
{
    return swap32(value_);
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

void write_binary_file(const std::string& file_path_, const void* const data_, size_t size_)
{
    FILE* fp = fopen(file_path_.c_str(), "wb+");
    if (!fp)
    {
        throw 12;
    }
    size_t written = fwrite(data_, 1, size_, fp);
    if (written != size_)
    {
        throw 13;
    }
    fclose(fp);
}

void read(const uint8_t*& current, void* into, const size_t size)
{
    ::memcpy(into, current, size);
    current += size;
}

size_t left(const uint8_t* const current_, const uint8_t* const end_)
{
    return end_ - current_;
}

std::vector<uint8_t> read_bytes(const uint8_t*& current, const size_t size)
{
    std::vector<uint8_t> result(size);
    ::memcpy(result.data(), current, result.size());
    current += size;
    return result;
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
    char* current_out = result.data();
    const uint8_t* begin_in = static_cast<const uint8_t*>(buffer);
    const uint8_t* current_in = begin_in;

    auto line = [/*&result,
        address_size,*/
        hex_size/*,
            ascii_size*/](char*& current_out_,
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

stream_writer_t::stream_writer_t(uint8_t* begin_, uint8_t* end_) :m_begin(begin_), m_end(end_), m_current(begin_)
{
}

size_t stream_writer_t::left() const
{
    return m_end - m_current;
}

void stream_writer_t::advanced(size_t distance_)
{
    if (left() < distance_)
    {
        printf("stream_writer::advance over buffer end\n");
        throw 2;
    }
    m_current += distance_;
}

void stream_writer_t::write(const uint8_t* begin_, const uint8_t* end_)
{
    const size_t size = end_ - begin_;
    if (left() < size)
    {
        printf("stream_writer::write over buffer end\n");
        throw 1;
    }
    ::memcpy(m_current, begin_, size);
    advanced(size);
}

void stream_writer_t::write_uint8(uint8_t byte_)
{
    write(&byte_, &byte_ + sizeof(byte_));
}

void stream_writer_t::write_vector(const std::vector<uint8_t>& bytes_)
{
    write(bytes_.data(), bytes_.data() + bytes_.size());
}

stream_reader_t::stream_reader_t(const uint8_t* begin_, const uint8_t* end_) :m_begin(begin_), m_end(end_), m_current(begin_)
{
}

size_t stream_reader_t::left() const
{
    return m_end - m_current;
}

bool stream_reader_t::fits(size_t size_) const
{
    return left() >= size_;
}

void stream_reader_t::advanced(size_t distance_)
{
    if (!fits(distance_))
    {
        printf("stream_reader_t::advance over buffer end\n");
        throw 2;
    }
    m_current += distance_;
}

void stream_reader_t::read(void* ptr_, size_t size_)
{
    if (!fits(size_))
    {
        printf("stream_reader_t::read over buffer end\n");
        throw 1;
    }
    ::memcpy(ptr_, m_current, size_);
    advanced(size_);
}

uint8_t stream_reader_t::read_uint8()
{
    return read<uint8_t>();
}

uint16_t stream_reader_t::read_uint16()
{
    return read<uint16_t>();
}

uint16_t stream_reader_t::read_uint16_be()
{
    return read_be<uint16_t>();
}

uint32_t stream_reader_t::read_uint32()
{
    return read<uint32_t>();
}

uint32_t stream_reader_t::read_uint32_be()
{
    return read_be<uint32_t>();
}

std::vector<uint8_t> stream_reader_t::read_vector(size_t size_)
{
    if (!fits(size_))
    {
        printf("stream_reader_t::read over buffer end\n");
    }
    std::vector<uint8_t> tmp(size_);
    read(tmp.data(), tmp.size());
    return tmp;
}
