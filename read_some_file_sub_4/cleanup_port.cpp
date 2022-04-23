#include "emu.hpp"
#include "types.hpp"
#include "helper.hpp"

namespace cleanup
{
#pragma pack( push, 1 )
    struct pack_block_t
    {
        uint8_t packed_size{};
        uint8_t flag{};      // 1 = more data to uncompress, 0 = end
        uint16_t data_len{}; // (un)compressed data
    };
#pragma pack( pop )
    static_assert( sizeof( pack_block_t ) == 4, "wrong size" );

    void emu_GAME_START_sub_3( uint8_t* uncompressed_, uint8_t* compressed_ )
    {
        // tests
        uint8_t* org_uncompressed = uncompressed_;
        uint8_t* org_compressed = compressed_;

        while( true )
        {
            {
                // dest_buffer_ and another_pointer2_ do only grow
                assert( org_uncompressed >= org_uncompressed );
                assert( compressed_ >= org_compressed );
            }

            const pack_block_t pack_block = *reinterpret_cast<pack_block_t*>( compressed_ );
            compressed_ += sizeof( pack_block );

            printf( "pack_block: packed_size: 0x%02X, flag: 0x%02X, data_len: 0x%04X\n", pack_block.packed_size,
                    pack_block.flag, pack_block.data_len );

            assert( pack_block.flag == 0 || pack_block.flag == 1 );

            if( pack_block.packed_size == 0 )
            {
                // uncompressed (just append the data)

                printf( " uncompressed data\n" );

                ::memcpy( uncompressed_, compressed_, pack_block.data_len );
                uncompressed_ += pack_block.data_len;
                compressed_ += pack_block.data_len;
            }
            else
            {
                // compressed - unpack

                /*
				pack_block 0
				    uint8_t packed_size
					uint8_t flag
					uint16_t data_len
				
				uint8_t data1[pack_block.packed_size] -> src[0x201]
				uint8_t data2[pack_block.packed_size] -> src[0x001]
				uint8_t data3[pack_block.packed_size] -> src[0x101]
				uint8_t data4[pack_block.packed_size] -> im loop "just loop n times" verarbeitet

				pack_block 1
					uint8_t packed_size
					uint8_t flag
					uint16_t data_len

				uint8_t data1[pack_block.packed_size] -> src[0x201]
				uint8_t data2[pack_block.packed_size] -> src[0x001]
				uint8_t data3[pack_block.packed_size] -> src[0x101]
				uint8_t data4[pack_block.packed_size] -> im loop "just loop n times" verarbeitet

				pack_block 2
				...
				*/

                //{

                // some sort of offset/value maps

                std::vector<uint8_t> table0( 1 + pack_block.packed_size );
                std::vector<uint8_t> table1( 1 + pack_block.packed_size );
                std::vector<uint8_t> table2( 1 + pack_block.packed_size );
                std::array<uint8_t, 256> table3{}; // needs to be 0 filled
                std::vector<uint8_t> table4( 1 + pack_block.packed_size );

                ::memcpy( &table2[1], compressed_, pack_block.packed_size );
                compressed_ += pack_block.packed_size;

                ::memcpy( &table0[1], compressed_, pack_block.packed_size );
                compressed_ += pack_block.packed_size;

                ::memcpy( &table1[1], compressed_, pack_block.packed_size );
                compressed_ += pack_block.packed_size;

                for( uint16_t i = 0; i < pack_block.packed_size; ++i )
                {
                    const uint8_t ofs2 = i + 1;

                    // packed_size 0-255 => ofs2 => 1-256
                    const uint8_t ofs = table2[ofs2];
                    assert( ( ofs > 0 ) && ( ofs <= 255 ) );

                    assert( ofs2 <= pack_block.packed_size ); // packed_size+1
                    table4[ofs2] = table3[ofs];
                    assert( ( table4[ofs2] >= 0 ) && ( table4[ofs2] < pack_block.packed_size ) );
                    table3[ofs] = ofs2;
                }

                struct stack_vals_t
                {
                    uint8_t val0{};
                    uint8_t val1{};
                };

                std::stack<stack_vals_t> stack;
                uint8_t val_3 = 0;
                uint8_t val_4 = 0;

                auto loc_128_block = [&stack, &val_3, &val_4, &table1, &table0, &pack_block]() {
                    assert( ( val_3 > 0 ) && ( val_3 <= pack_block.packed_size ) ); // packed_size+1
                    const uint8_t val_5 = table1[val_3];
                    assert( ( val_5 >= 0 ) && ( val_5 <= 255 ) );
                    stack.push( { val_3, val_5 } );
                    val_4 = table0[val_3];
                    assert( ( val_4 >= 0 ) && ( val_4 <= 255 ) );
                };

                auto loc_572_block = [&uncompressed_, &stack, &val_3, &val_4]() {
                    *uncompressed_++ = val_4;

                    const stack_vals_t stack_val = stack.top();
                    stack.pop();

                    if( ( stack_val.val0 == 0 ) && ( stack_val.val1 == 0 ) )
                    {
                        return true; // goto loc_124;
                    }

                    val_3 = stack_val.val0;
                    val_4 = stack_val.val1;
                    return false;
                };

                for( uint16_t i = 0; i < pack_block.data_len; ++i ) // just loop n times
                {
                    val_3 = *compressed_++;
                    assert( ( val_3 >= 0 ) && ( val_3 <= 255 ) );

                    const uint8_t val301_0 = table3[val_3];
                    assert( ( val301_0 >= 0 ) && ( val301_0 <= pack_block.packed_size ) );

                    if( val301_0 == 0 )
                    {
                        *uncompressed_++ = val_3;
                    }
                    else
                    {
                        val_3 = val301_0;

                        stack.push( { 0, 0 } ); // end mark

                        loc_128_block(); // stack push

                        bool end_inner_loop = false;
                        while( true )
                        {
                            const uint8_t ofs1 = val_4;

                            const uint8_t val301_1 = table3[ofs1];

                            if( val301_1 == 0 )
                            {
                                if( loc_572_block() ) // stack pop
                                {
                                    end_inner_loop = true;
                                    break;
                                }
                            }
                            else if( val_3 > val301_1 )
                            {
                                val_3 = val301_1;

                                loc_128_block(); // stack push
                            }
                            else
                            {
                                val_4 = val_3;
                                val_3 = val301_1;

                                while( true )
                                {
                                    assert( val_3 <= pack_block.packed_size ); // packed_size+1
                                    val_3 = table4[val_3];
                                    assert( ( val_3 >= 0 ) && ( val_3 < pack_block.packed_size ) );

                                    if( val_3 == 0 )
                                    {
                                        val_4 = ofs1;
                                        if( loc_572_block() ) // stack pop
                                        {
                                            end_inner_loop = true;
                                        }
                                        break;
                                    }
                                    else if( val_3 < val_4 )
                                    {
                                        loc_128_block(); // stack push
                                        break;
                                    }

                                    // another run
                                }
                            }
                            if( end_inner_loop )
                            {
                                break;
                            }
                        }
                    }
                }

                if( pack_block.flag == 0 )
                {
                    return;
                }
            }
        }
    }

    size_t emu_read_some_file_sub_4( emu_t& e,
                                     config_tat_t::executable_info_t* exec_info_,
                                     emu_t::ptr16_t& executable_buffer_ )
    {
        uint8_t* executable_buffer = e.byte_ptr( executable_buffer_ );

        const std::string filename( exec_info_->filename.data() );
        const std::string game_dir = R"(F:\projects\fun\dos_games_rev\alpha_waves_dev\tests\alpha)";
        const std::string file_path = game_dir + "\\" + filename;

        const std::vector<uint8_t> prog_cc1_content = read_binary_file( file_path );
        assert( prog_cc1_content.size() == sizeof( progs_cc1_t ) );
        const progs_cc1_t* progs_cc1 = reinterpret_cast<const progs_cc1_t*>( prog_cc1_content.data() );

        struct exec_data_slice_t
        {
            uint32_t data_size{};
            uint32_t unpacked_data_size{};
            const uint8_t* data{};
        };

        auto get_slice = []( const auto& exec_data_ ) {
            return exec_data_slice_t{ swap( exec_data_.data_size ), swap( exec_data_.unpacked_data_size ),
                                      exec_data_.data.data() };
        };

        const exec_data_slice_t slice = [&]( size_t exec_type_ ) {
            switch( exec_info_->byte_12h )
            {
                case 0:
                    return get_slice( progs_cc1->sound.pc_buz );
                case 1:
                    return get_slice( progs_cc1->sound.tandy );
                case 2:
                    return get_slice( progs_cc1->sound.adlib );
                case 3:
                    return get_slice( progs_cc1->gfx.cga_hercules );
                case 4:
                    return get_slice( progs_cc1->gfx.tandy );
                case 5:
                    return get_slice( progs_cc1->gfx.ega_vga );
                default:
                    assert( false );
                    break;
            }
            return exec_data_slice_t{};
        }( exec_info_->byte_12h );

        const size_t another_pointer2_ofs = ( slice.unpacked_data_size - slice.data_size ) + 16;
        uint8_t* compress_data = executable_buffer + another_pointer2_ofs;

        ::memcpy( compress_data, slice.data, slice.data_size );

        const size_t result_executable_buffer_ofs = slice.unpacked_data_size + 16;
        executable_buffer_ = e.ptr_to_ptr16( executable_buffer + result_executable_buffer_ofs );

        uint8_t* uncompressed_data = executable_buffer;

        // some sort of uncompression, after that the executable is at executable_buffer[0] with size = slice.unpacked_data_size
        emu_GAME_START_sub_3( uncompressed_data, compress_data );
        printf( "---------\n" );

        return slice.unpacked_data_size;
    }

} // namespace cleanup
