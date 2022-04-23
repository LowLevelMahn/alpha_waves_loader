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

    void emu_GAME_START_sub_3( uint8_t* src_buffer_, uint8_t* dest_buffer_, uint8_t* another_pointer2_ )
    {
        // tests
        uint8_t* org_src_buffer = src_buffer_;
        uint8_t* org_dest_buffer = dest_buffer_;
        uint8_t* org_another_pointer2 = another_pointer2_;

        while( true )
        {
            {
                // src_buffer ptr does not change
                assert( src_buffer_ == org_src_buffer );
                // dest_buffer_ and another_pointer2_ do only grow
                assert( dest_buffer_ >= org_dest_buffer );
                assert( another_pointer2_ >= org_another_pointer2 );
            }

            const pack_block_t pack_block = *reinterpret_cast<pack_block_t*>( another_pointer2_ );
            another_pointer2_ += sizeof( pack_block );

            printf( "pack_block: packed_size: 0x%02X, flag: 0x%02X, data_len: 0x%04X\n", pack_block.packed_size,
                    pack_block.flag, pack_block.data_len );

            assert( pack_block.flag == 0 || pack_block.flag == 1 );

            if( pack_block.packed_size == 0 )
            {
                // uncompressed (just append the data)

                printf( " uncompressed data\n" );

                ::memcpy( dest_buffer_, another_pointer2_, pack_block.data_len );
                dest_buffer_ += pack_block.data_len;
                another_pointer2_ += pack_block.data_len;
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

                std::vector<std::vector<uint8_t>> data_block( 4 );
                for( size_t d = 0; d < 4; ++d )
                {
                    auto& data = data_block[d];
                    data.resize( pack_block.packed_size );
                    ::memcpy( data.data(), another_pointer2_ + d * pack_block.packed_size, pack_block.packed_size );
                }

                std::array<uint8_t, 256> data_301{};
                std::array<uint8_t, 256> data_402{};

                for( uint16_t i = 0; i < pack_block.packed_size; ++i )
                {
                    const uint8_t ofs2 = data_block[0][i];

                    // offset could be only 0-255 - written as uint8_t in data_301
                    assert( i < 255 );
                    const uint8_t offset = i + 1;

                    data_402[offset] = data_301[ofs2];
                    data_301[ofs2] = offset;
                }

                for( const auto& val : data_block[3] )
                {
                    uint8_t v = data_301[val];
                    if( v == 0 )
                    {
                        // just store value
                    }
                }
                //}

                //src-offsets that are multiple used
                //0 ?
                uint8_t* src_0x001 = &src_buffer_[0x001]; //[0x001-[0x100 255 bytes (0-254)
                uint8_t* src_0x100 = &src_buffer_[0x100]; //[0x100-[0x101 1 byte
                uint8_t* src_0x101 = &src_buffer_[0x101]; //[0x101-[0x200 255 bytes (0-254)
                uint8_t* src_0x200 = &src_buffer_[0x200]; //[0x200-[0x201 1 byte
                uint8_t* src_0x201 = &src_buffer_[0x201]; //[0x201-[0x301 256 bytes (0-255)
                uint8_t* src_0x301 = &src_buffer_[0x301]; //[0x301-[0x402 257 bytes (0-256)
                uint8_t* src_0x402 = &src_buffer_[0x402]; //[0x402-...

                // overwrite with 0xDD "dirty" value - start-value seems only relevant for src_0x301 block
                ::memset( src_0x001, 0xDD, 255 + 1 + 255 + 1 + 256 + 257 ); // = 1025

                // only used for compressed blocks
                ::memset( src_0x301, 0, 256 ); // clean 256 bytes, [0x301-[0x401
                                               // 0 means unused or something - can't be any value

                printf( "init #1\n" );

                printf( " compressed data\n" );
                printf( "   src_0x201: %s\n",
                        hex_string( another_pointer2_ + 0 * pack_block.packed_size, pack_block.packed_size ).c_str() );
                printf( "   src_0x001: %s\n",
                        hex_string( another_pointer2_ + 1 * pack_block.packed_size, pack_block.packed_size ).c_str() );
                printf( "   src_0x101: %s\n",
                        hex_string( another_pointer2_ + 2 * pack_block.packed_size, pack_block.packed_size ).c_str() );

                assert( pack_block.packed_size <= 255 );
                ::memcpy( src_0x201, another_pointer2_, pack_block.packed_size );
                another_pointer2_ += pack_block.packed_size;

                assert( pack_block.packed_size <= 254 );
                ::memcpy( src_0x001, another_pointer2_, pack_block.packed_size );
                another_pointer2_ += pack_block.packed_size;

                assert( pack_block.packed_size <= 254 );
                ::memcpy( src_0x101, another_pointer2_, pack_block.packed_size );
                another_pointer2_ += pack_block.packed_size;

                //src_0x301 complete filled with 0

                for( uint16_t i = 0; i < pack_block.packed_size; ++i )
                {
                    // packed_size 0-255 => i+1 => 1-256
                    const uint8_t ofs2 = src_0x201[i];

                    src_0x402[i + 1] = src_0x301[ofs2];
                    src_0x301[ofs2] = i + 1;
                }

                printf( "init #2 after: src_0x301:\n%s\n", hexdump( src_0x301, 256, 64 ).c_str() );
                printf( "init #2 after: src_0x402:\n%s\n", hexdump( src_0x402, 256, 64 ).c_str() );

                struct stack_vals_t
                {
                    uint8_t val0{};
                    uint8_t val1{};
                };

                std::stack<stack_vals_t> stack;
                uint8_t val_3 = 0;
                uint8_t val_4 = 0;

                auto loc_128_block = [src_buffer_, &stack, &val_3, &val_4, &src_0x100]() {
                    stack.push( { val_3, src_0x100[val_3] } );
                    val_4 = src_buffer_[val_3];
                };

                auto loc_572_block = [&dest_buffer_, &stack, &val_3, &val_4]() {
                    *dest_buffer_++ = val_4;

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
                    val_3 = *another_pointer2_++;

                    const uint8_t val301_0 = src_0x301[val_3];

                    if( val301_0 == 0 )
                    {
                        *dest_buffer_++ = val_3;
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

                            const uint8_t val301_1 = src_0x301[ofs1];

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
                                    val_3 = src_0x402[val_3];

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

#if 1
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
        uint8_t* another_pointer2 = executable_buffer + another_pointer2_ofs;

        ::memcpy( another_pointer2, slice.data, slice.data_size );

        const size_t result_executable_buffer_ofs = slice.unpacked_data_size + 16;
        executable_buffer_ = e.ptr_to_ptr16( executable_buffer + result_executable_buffer_ofs );

        const size_t src_buffer_ofs = ( ( result_executable_buffer_ofs / 16 ) + 1 ) *
                                      16; // align to segment adress (keep it 100% exact to original code)
        uint8_t* src_buffer = executable_buffer + src_buffer_ofs;
        uint8_t* dest_buffer = executable_buffer;

    #if 0
        printf( "another_pointer2_ofs: %u, result_executable_buffer_ofs: %u, src_buffer_ofs: %u, dest_buffer_ofs: %u\n",
                another_pointer2_ofs, result_executable_buffer_ofs, src_buffer_ofs, 0 );
    #endif
#else
        FILE* fp = fopen( file_path.c_str(), "rb" );
        assert( fp );

        assert( ( exec_info_->byte_13h & 0x18 ) != 0 );
        assert( ( exec_info_->byte_13h & 0x10 ) != 0 );

        //---

        uint16_t file_offset1{ 0 };
        size_t read_bytes = fread( &file_offset1, 1, sizeof( file_offset1 ), fp );
        assert( read_bytes == sizeof( file_offset1 ) );

        const uint32_t pos2 = exec_info_->byte_12h * 4;

        int res = fseek( fp, pos2, SEEK_CUR );
        assert( res == 0 );

        uint32_t offset2{};
        read_bytes = fread( &offset2, 1, sizeof( offset2 ), fp );
        assert( read_bytes == sizeof( offset2 ) );

        const uint32_t pos = swap( offset2 ) + ( swap( file_offset1 ) * 4 ) + 2;

        res = fseek( fp, pos, SEEK_SET );
        assert( res == 0 );

        //---

        uint32_t file_offsets2[2];
        read_bytes = fread( file_offsets2, 1, sizeof( file_offsets2 ), fp );
        assert( read_bytes == sizeof( file_offsets2 ) );

        // (un)packed sizes?
        const uint32_t ofs1 = swap( file_offsets2[0] );
        const uint32_t ofs2 = swap( file_offsets2[1] );

        uint8_t* another_pointer2 = executable_buffer + ( ofs2 - ofs1 ) + 16;

        read_bytes = fread( another_pointer2, 1, ofs1, fp );
        assert( read_bytes == ofs1 );

        res = fclose( fp );
        assert( res == 0 );

        const size_t ofs3 = ofs2 + 16;
        executable_buffer_ = e.ptr_to_ptr16( executable_buffer + ofs3 );

        const size_t ofs4 = ( ( ofs3 / 16 ) + 1 ) * 16; // align to segment adress
        uint8_t* src_buffer = executable_buffer + ofs4;
        uint8_t* dest_buffer = executable_buffer;

        // ofs4, ofs3 or ofs2 = encoded size?
        // ofs2 = exact ae_vga.exe size or the possible size of adlib.com
#endif

        // some sort of uncompression, after that the executable is at executable_buffer[0] with size = slice.unpacked_data_size
        emu_GAME_START_sub_3( src_buffer, dest_buffer, another_pointer2 );
        printf( "---------\n" );

        return slice.unpacked_data_size;
    }

} // namespace cleanup
