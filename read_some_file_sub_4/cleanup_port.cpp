#include "emu.hpp"
#include "types.hpp"
#include "helper.hpp"

namespace cleanup
{
#pragma pack( push, 1 )
    struct something_t
    {
        uint8_t len1{};
        uint8_t unknown1{};
        uint16_t len2{};
    };
#pragma pack( pop )
    static_assert( sizeof( something_t ) == 4, "wrong size" );

    void emu_GAME_START_sub_3( uint8_t* src_buffer_, uint8_t* dest_buffer_, uint8_t* another_pointer2_ )
    {
#if 1
        size_t distance = src_buffer_ - dest_buffer_; // encoded size?
        int brk = 1;
#endif

        while( true )
        {
            ::memset( &src_buffer_[0x301], 0, 128 * sizeof( uint16_t ) ); // clean 256 bytes

            const something_t something = *reinterpret_cast<something_t*>( another_pointer2_ );
            another_pointer2_ += sizeof( something );

            if( something.len1 == 0 )
            {
                ::memcpy( dest_buffer_, another_pointer2_, something.len2 );
                dest_buffer_ += something.len2;
                another_pointer2_ += something.len2;
            }
            else
            {
                ::memcpy( &src_buffer_[0x201], another_pointer2_, something.len1 );
                another_pointer2_ += something.len1;

                ::memcpy( &src_buffer_[0x001], another_pointer2_, something.len1 );
                another_pointer2_ += something.len1;

                ::memcpy( &src_buffer_[0x101], another_pointer2_, something.len1 );
                another_pointer2_ += something.len1;

                for( uint16_t i = 0; i < something.len1; ++i )
                {
                    const uint8_t ofs = i + 1;
                    uint8_t* at0x301 = &src_buffer_[src_buffer_[ofs + 0x200] + 0x301];
                    src_buffer_[ofs + 0x402] = *at0x301;
                    *at0x301 = ofs;
                }

                struct stack_vals_t
                {
                    uint8_t val0{};
                    uint8_t val1{};
                };

                std::stack<stack_vals_t> stack;
                uint8_t val_3 = 0;
                uint8_t val_4 = 0;

                auto loc_128_block = [src_buffer_, &stack, &val_3, &val_4]() {
                    stack.push( { val_3, src_buffer_[val_3 + 0x100] } );
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

                for( uint16_t i = something.len2; i > 0; --i ) // just loop n times
                {
                    val_3 = *another_pointer2_++;

                    const uint8_t val301_0 = src_buffer_[val_3 + 0x301];
                    if( val301_0 == 0 )
                    {
                        *dest_buffer_++ = val_3;
                    }
                    else
                    {
                        val_3 = val301_0;

                        stack.push( { 0, 0 } ); // end mark

                        loc_128_block(); // also stack push

                        bool end_inner_loop = false;
                        while( true )
                        {
                            const uint8_t ofs1 = val_4;

                            const uint8_t val301_1 = src_buffer_[ofs1 + 0x301];

                            if( val301_1 == 0 )
                            {
                                if( loc_572_block() )
                                {
                                    end_inner_loop = true;
                                    break;
                                }
                            }
                            else if( val_3 > val301_1 )
                            {
                                val_3 = val301_1;

                                loc_128_block();
                            }
                            else
                            {
                                val_4 = val_3;
                                val_3 = val301_1;

                                while( true )
                                {
                                    val_3 = src_buffer_[val_3 + 0x402];

                                    if( val_3 == 0 )
                                    {
                                        val_4 = ofs1;
                                        if( loc_572_block() )
                                        {
                                            end_inner_loop = true;
                                        }
                                        break;
                                    }
                                    else if( val_3 < val_4 )
                                    {
                                        loc_128_block();
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

                if( something.unknown1 == 0 )
                {
                    return;
                }
            }
        }
    }

    void emu_read_some_file_sub_4( emu_t& e,
                                   const uint8_t /*byte_55_*/,
                                   emu_t::ptr16_t& executable_buffer_,
                                   const slice_t& executable_buffer_slice_ )
    {
        const config_tat_t::executable_info_t* exec_info = e.memory<config_tat_t::executable_info_t>( e.cs, e.bx );
        uint8_t* executable_buffer = e.byte_ptr( executable_buffer_ );

        const std::string filename( exec_info->filename.data() );
        const std::string game_dir = R"(F:\projects\fun\dos_games_rev\alpha_waves_dev\tests\alpha)";
        const std::string file_path = game_dir + "\\" + filename;
        FILE* fp = fopen( file_path.c_str(), "rb" );
        assert( fp );

        assert( ( exec_info->byte_13h & 0x18 ) != 0 );

        if( ( exec_info->byte_13h & 0x10 ) != 0 )
        {
            assert( fread( executable_buffer, 1, 2, fp ) == 2 );
            uint16_t some_offset = swap( *reinterpret_cast<uint16_t*>( executable_buffer ) );
            const uint32_t pos2 = exec_info->byte_12h * 4;

            assert( fseek( fp, pos2, SEEK_CUR ) == 0 );

            assert( fread( executable_buffer, 1, 4, fp ) == 4 );
            const uint32_t pos = swap( *reinterpret_cast<uint32_t*>( executable_buffer ) ) + ( some_offset * 4 ) + 2;

            assert( fseek( fp, pos, SEEK_SET ) == 0 );
        }

        assert( fread( executable_buffer, 1, 8, fp ) == 8 );

        const uint32_t* val32 = reinterpret_cast<uint32_t*>( executable_buffer );
        const uint32_t ofs1 = swap( val32[0] );
        const uint32_t ofs2 = swap( val32[1] );

        const uint32_t distance = ( ofs2 - ofs1 ) + 16;

        uint8_t* another_pointer2 = executable_buffer + distance;

        size_t read_bytes = fread( another_pointer2, 1, ofs1, fp );
        assert( read_bytes == ofs1 );

        assert( fclose( fp ) == 0 );

        assert( e.byte_ptr( executable_buffer_ ) == executable_buffer );

        const emu_t::ptr16_t exec_buff = emu_t::ptr16( emu_t::offset32( executable_buffer_ ) + ofs2 + 16 );
        if( exec_buff.offset != 0 )
        {
            //sometimes
            //assert( false );
        }
        uint8_t* src_buffer = e.byte_ptr( exec_buff.segment + 1, 0 ); // some sort of align

        //printf( "distance: %u\n", src_buffer - e.byte_ptr( exec_buff ) );

        uint8_t* dest_buffer = e.byte_ptr( executable_buffer_ );
        executable_buffer_ = exec_buff;

        // some sort of uncompression, after that the executable is +sizeof(PSP) behind executable_buffer_begin
        emu_GAME_START_sub_3( src_buffer, dest_buffer, another_pointer2 );
    }

} // namespace cleanup
