#pragma once

#include <cstdint>
#include <string>
#include <vector>

std::string hexdump( const void* const ptr, int buflen, int width );
std::vector<uint8_t> read_binary_file( const std::string& filename_ );
void write_binary_file( const std::string& file_path_, const void* const data_, size_t size_ );

std::string hex_string( const void* const buffer_, const size_t& size_, bool as_stream_ = false );

inline uint8_t lo( uint16_t value_ )
{
    return value_ & 0xFF;
}

inline uint8_t hi( uint16_t value_ )
{
    return value_ >> 8;
}

inline uint16_t lo( uint32_t value_ )
{
    return value_ & 0xFFFF;
}

inline uint16_t hi( uint32_t value_ )
{
    return value_ >> 16;
}

inline uint16_t swap( const uint16_t value_ )
{
    return ( value_ << 8 ) + ( value_ >> 8 );
}

inline uint32_t swap( const uint32_t value_ )
{
    const uint16_t lv = lo( value_ );
    const uint16_t hv = hi( value_ );
    return ( swap( lv ) << 16 ) + swap( hv );
}
