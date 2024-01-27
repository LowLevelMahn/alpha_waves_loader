#pragma once

// swapping, binary file handling, hexdump, ... 

#include <string>
#include <vector>
#include <cstdint>

uint8_t lo(const uint16_t value_);
uint8_t hi(const uint16_t value_);

uint16_t lo(const uint32_t value_);
uint16_t hi(const uint32_t value_);

uint16_t swap16(const uint16_t value_);
uint32_t swap32(const uint32_t value_);
uint16_t swap(const uint16_t value_);
uint32_t swap(const uint32_t value_);

std::vector<uint8_t> read_binary_file(const std::string& filename_);

void write_binary_file(const std::string& file_path_, const void* const data_, size_t size_);

void read(const uint8_t*& current, void* into, const size_t size);

size_t left(const uint8_t* const current_, const uint8_t* const end_);

std::vector<uint8_t> read_bytes(const uint8_t*& current, const size_t size);

uint16_t read16(const uint8_t*& current);

uint32_t read32(const uint8_t*& current);

// fixed output buffer
class stream_writer_t
{
private:
    uint8_t* m_begin{};
    uint8_t* m_end{};
    uint8_t* m_current{};

public:
    stream_writer_t(uint8_t* begin_, uint8_t* end_);
    size_t left() const;
    void advanced(size_t distance_);
    void write(const uint8_t* begin_, const uint8_t* end_);
    void write_uint8(uint8_t byte_);
    void write_vector(const std::vector<uint8_t>& bytes_);
};

class stream_reader_t
{
private:
    const uint8_t* m_begin{};
    const uint8_t* m_end{};
    const uint8_t* m_current{};

public:
    stream_reader_t(const uint8_t* begin_, const uint8_t* end_);
    size_t left() const;
    bool fits(size_t size_) const;
    void advanced(size_t distance_);
    void read(void* ptr_, size_t size_);

    template<typename ValueType>
    ValueType read();

    template<typename ValueType>
    ValueType read_be();

    uint8_t read_uint8();
    uint16_t read_uint16();
    uint16_t read_uint16_be();
    uint32_t read_uint32();
    uint32_t read_uint32_be();

    std::vector<uint8_t> read_vector(size_t size_);
};

template<typename ValueType>
ValueType stream_reader_t::read()
{
    ValueType tmp{};
    read(&tmp, sizeof(tmp));
    return tmp;
}

template<typename ValueType>
ValueType stream_reader_t::read_be()
{
    return swap(read<ValueType>());
}

std::string hexdump(const void* const buffer, int buffer_size, int width);
