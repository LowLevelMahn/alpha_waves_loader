#include <cstdint>
#include <vector>
#include <stack>
#include <array>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <fstream>

#pragma pack( push, 1 )
struct pack_block_t
{
    uint8_t packed_size{};
    uint8_t flag{};      // 1 = more data to uncompress, 0 = end
    uint16_t data_len{}; // (un)compressed data
                         /*
	if packed_size != 0
		table0[ 1 + pack_block.packed_size ]
		table1[ 1 + pack_block.packed_size ]
		table2[ 1 + pack_block.packed_size ]
	data[ pack_block.data_len ]
	*/
};
static_assert( sizeof( pack_block_t ) == 4, "wrong size" );
#pragma pack( pop )

struct tables_t
{
    std::vector<uint8_t> table1;
    std::vector<uint8_t> table2;
    std::vector<uint8_t> data;

    //prepared
    std::array<uint8_t, 256> table3{}; // needs to be 0 filled
    std::vector<uint8_t> table4;
};

tables_t prepare_tables( const uint8_t*& compressed, const uint8_t packed_size, const uint16_t data_len_ )
{
    tables_t tables;
    // from uncompressed data right after packed_block
    std::vector<uint8_t> table0( 1 + packed_size ); // only for initialization
    tables.table1.resize( 1 + packed_size );
    tables.table2.resize( 1 + packed_size );
    tables.data.resize( data_len_ );

    ::memcpy( &table0[1], compressed, packed_size );
    compressed += packed_size;

    ::memcpy( &tables.table1[1], compressed, packed_size );
    compressed += packed_size;

    ::memcpy( &tables.table2[1], compressed, packed_size );
    compressed += packed_size;

    ::memcpy( tables.data.data(), compressed, data_len_ );
    compressed += data_len_;

    // runtime filled
    tables.table4.resize( 1 + packed_size );

    for( uint16_t i = 0; i < packed_size; ++i )
    {
        const uint8_t ofs_0 = i + 1;
        const uint8_t ofs_1 = table0[ofs_0];
        tables.table4[ofs_0] = tables.table3[ofs_1];
        tables.table3[ofs_1] = ofs_0;
    }

    return tables;
}

void val_3_non_0( uint8_t*& uncompressed_, const tables_t& tables_, const uint8_t val_3_ )
{
    struct stack_vals_t
    {
        uint8_t val_0{};
        uint8_t val_1{};
    };

    std::stack<stack_vals_t> stack;

    auto helper1 = [&stack, &tables_]( const uint8_t val_7_ ) {
        stack.push( { val_7_, tables_.table2[val_7_] } );
        return tables_.table1[val_7_];
    };

    auto helper2 = [&stack]( uint8_t* val_7_, uint8_t* val_4_ ) {
        if( stack.empty() )
        {
            return true;
        }

        const stack_vals_t stack_val = stack.top();
        stack.pop();
        *val_7_ = stack_val.val_0;
        *val_4_ = stack_val.val_1;

        return false;
    };

    uint8_t val_7 = val_3_;
    uint8_t val_4 = helper1( val_7 );

    while( true )
    {
        const uint8_t val_5 = val_4;
        const uint8_t val_6 = tables_.table3[val_5];

        if( val_6 == 0 )
        {
            *uncompressed_++ = val_4;
            if( helper2( &val_7, &val_4 ) )
            {
                return;
            }
        }
        else if( val_7 > val_6 )
        {
            val_7 = val_6;
            val_4 = helper1( val_7 );
        }
        else
        {
            val_4 = val_7;
            val_7 = val_6;

            assert( stack.size() >= 0 );
            while( true )
            {
                val_7 = tables_.table4[val_7];

                if( val_7 == 0 )
                {
                    *uncompressed_++ = val_5;
                    if( helper2( &val_7, &val_4 ) )
                    {
                        return;
                    }
                    break;
                }
                else if( val_7 < val_4 )
                {
                    val_4 = helper1( val_7 );
                    break;
                }

                // another run
            }
        }
    }
}

void emu_GAME_START_sub_3( std::vector<uint8_t>& uncompressed_buffer_, const std::vector<uint8_t>& compressed_buffer_ )
{
    uint8_t* uncompressed = uncompressed_buffer_.data();
    const uint8_t* compressed = compressed_buffer_.data();

    while( true )
    {
        const pack_block_t pack_block = *reinterpret_cast<const pack_block_t*>( compressed );
        compressed += sizeof( pack_block );
        assert( pack_block.flag == 0 || pack_block.flag == 1 );

        if( pack_block.packed_size == 0 )
        {
            // uncompressed (just append the data)
            ::memcpy( uncompressed, compressed, pack_block.data_len );
            uncompressed += pack_block.data_len;
            compressed += pack_block.data_len;
            continue;
        }

        // compressed - unpack

        // some sort of offset/value maps
        tables_t tables = prepare_tables( compressed, pack_block.packed_size, pack_block.data_len );

        for( const uint8_t val_2 : tables.data )
        {
            const uint8_t val_3 = tables.table3[val_2];

            if( val_3 == 0 )
            {
                *uncompressed++ = val_2;
            }
            else
            {
                val_3_non_0( uncompressed, tables, val_3 );
            }
        }

        if( pack_block.flag == 0 )
        {
            return;
        }
    }
}

bool read_binary_file( const std::string& filename_, std::vector<uint8_t>& content_ )
{
    std::ifstream file( filename_, std::ios::binary );
    if( !file.is_open() )
    {
        return false;
    }
    content_ = std::vector<uint8_t>( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>() );
    return true;
}

#pragma pack( push, 1 )

template <size_t Size>
struct exec_data_t
{
    uint32_t data_size;
    uint32_t unpacked_data_size; // maybe
    std::array<uint8_t, Size> data;
};

struct progs_cc1_t
{
    //0:
    uint16_t exec_table_count;
    //2:
    std::array<uint32_t, 6> execs_offset_table;
    //26
    //26 + execs_offset_table[n] offset
    struct sound_t
    {
        exec_data_t<2422> pc_buz;
        exec_data_t<2885> tandy;
        exec_data_t<3053> adlib;
    };

    sound_t sound;

    struct gfx_t
    {
        exec_data_t<51265> cga_hercules;
        exec_data_t<51720> ega_vga;
        exec_data_t<50404> tandy;
    };
    gfx_t gfx;
};

static_assert( sizeof( progs_cc1_t ) == 161823, "wrong size" );
static_assert( offsetof( progs_cc1_t, exec_table_count ) == 0, "wrong offset" );
static_assert( offsetof( progs_cc1_t, execs_offset_table ) == 2, "wrong offset" );
static_assert( offsetof( progs_cc1_t, sound.pc_buz ) == 26, "wrong offset" );
static_assert( offsetof( progs_cc1_t, sound.tandy ) == 2456, "wrong offset" );
static_assert( offsetof( progs_cc1_t, sound.adlib ) == 5349, "wrong offset" );
static_assert( offsetof( progs_cc1_t, gfx.cga_hercules ) == 8410, "wrong offset" );
static_assert( offsetof( progs_cc1_t, gfx.ega_vga ) == 59683, "wrong offset" );
static_assert( offsetof( progs_cc1_t, gfx.tandy ) == 111411, "wrong offset" );

#pragma pack( pop )

struct exec_data_slice_t
{
    uint32_t data_size{};
    uint32_t unpacked_data_size{};
    const uint8_t* data{};
};

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

const exec_data_slice_t get_exec( const progs_cc1_t* progs_cc1_, size_t exec_type_ )
{
    auto get_slice = []( const auto& exec_data_ ) {
        return exec_data_slice_t{ swap( exec_data_.data_size ), swap( exec_data_.unpacked_data_size ),
                                  exec_data_.data.data() };
    };

    switch( exec_type_ )
    {
        case 0:
            return get_slice( progs_cc1_->sound.pc_buz );
        case 1:
            return get_slice( progs_cc1_->sound.tandy );
        case 2:
            return get_slice( progs_cc1_->sound.adlib );
        case 3:
            return get_slice( progs_cc1_->gfx.cga_hercules );
        case 4:
            return get_slice( progs_cc1_->gfx.ega_vga );
        case 5:
            return get_slice( progs_cc1_->gfx.tandy );
        default:
            assert( false );
            break;
    }
    return exec_data_slice_t{};
};

void write_binary_file( const std::string& file_path_, const void* const data_, size_t size_ )
{
    FILE* fp = fopen( file_path_.c_str(), "wb+" );
    assert( fp );
    size_t written = fwrite( data_, 1, size_, fp );
    assert( written = size_ );
    fclose( fp );
}

void create_executable( const progs_cc1_t* progs_cc1_, size_t exec_type_, const std::string& file_path_ )
{
    printf( "extracting %s\n", file_path_.c_str() );

    const auto exec = get_exec( progs_cc1_, exec_type_ );

    std::vector<uint8_t> uncompressed( exec.unpacked_data_size );

    emu_GAME_START_sub_3( uncompressed, { exec.data, exec.data + exec.data_size } );

    write_binary_file( file_path_, uncompressed.data(), uncompressed.size() );
}

int main( int argc_, char* argv_[] )
{
    if( argc_ < 2 )
    {
        printf( "command line parameters invalid\n" );
        printf( "\n" );
        printf( "  extractor [path to progs.cc1]\n" );
        printf( "\n" );
        printf( "  example:\n" );
        printf( "  extractor d:\\games\\alpha_waves\n" );
        printf( "\n" );
        return 1;
    }

    const std::string game_dir = argv_[1];

    const std::string file_path = game_dir + "/progs.cc1";

    std::vector<uint8_t> prog_cc1_content;
    if( !read_binary_file( file_path, prog_cc1_content ) )
    {
        printf( "wrong path to progs.cc1\n" );
        return 1;
    }

    if( prog_cc1_content.size() != sizeof( progs_cc1_t ) )
    {
        printf( "wrong size of progs.cc1\n" );
        return 2;
    }

    const progs_cc1_t* progs_cc1 = reinterpret_cast<const progs_cc1_t*>( prog_cc1_content.data() );

    create_executable( progs_cc1, 0, game_dir + "/s_pc_buz.com" );
    create_executable( progs_cc1, 1, game_dir + "/s_tandy.com" );
    create_executable( progs_cc1, 2, game_dir + "/s_adlib.com" );
    create_executable( progs_cc1, 3, game_dir + "/cga_herc.exe" );
    create_executable( progs_cc1, 4, game_dir + "/ega_vga.exe" );
    create_executable( progs_cc1, 5, game_dir + "/tandy.exe" );

    return 0;
}
