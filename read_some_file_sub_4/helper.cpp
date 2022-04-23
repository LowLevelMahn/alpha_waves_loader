#include "helper.hpp"

#include <cassert>
#include <fstream>

std::string hexdump( const void* const ptr, int buflen, int width )
{
    std::string dump;
    unsigned char* buf = (unsigned char*)ptr;
    for( int i = 0; i < buflen; i += width )
    {
        char buffer[100]{};
        sprintf_s( buffer, "%06X: ", i );
        dump += buffer;
        for( int j = 0; j < 16; j++ )
        {
            if( i + j < buflen )
            {
                sprintf_s( buffer, "%02X ", buf[i + j] );
                dump += buffer;
            }
            else
            {
                dump += "   ";
            }
        }
        dump += " ";
        for( int j = 0; j < width; j++ )
        {
            if( i + j < buflen )
            {
                sprintf_s( buffer, "%c", isprint( buf[i + j] ) ? buf[i + j] : '.' );
                dump += buffer;
            }
        }
        dump += "\n";
    }
    return dump;
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

std::string hex_string( const void* const buffer_, const size_t& size_, bool as_stream_ )
{
    if( size_ == 0 )
    {
        return {};
    }

    std::string tmp;
    const size_t byte_width = as_stream_ ? 2 : 3;

    const size_t string_size = ( size_ * byte_width ) - ( as_stream_ ? 0 : 1 );
    tmp.resize( string_size, ' ' );

    const uint8_t* const in_buffer = reinterpret_cast<const uint8_t*>( buffer_ );
    char* const out_buffer = &tmp[0];

    for( size_t i = 0; i < size_; ++i )
    {
        const char hex_digits[] = "0123456789ABCDEF";
        const uint8_t value = in_buffer[i];
        char* chr = &out_buffer[i * byte_width];
        chr[0] = hex_digits[value >> 4];
        chr[1] = hex_digits[value & 0xF];
    }

    return tmp;
}
