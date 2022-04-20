#include "emu.hpp"
#include "types.hpp"
#include "helper.hpp"

namespace
{
    void normalize_ptr( uint16_t& segment_, uint16_t& offset_ )
    {
        const auto normalized_ptr = emu_t::ptr16( emu_t::offset32( segment_, offset_ ) );
        segment_ = normalized_ptr.segment;
        offset_ = normalized_ptr.offset;
    }

    void normalize_ptr( emu_t::ptr16_t& ptr_ )
    {
        const auto normalized_ptr = emu_t::ptr16( emu_t::offset32( ptr_.segment, ptr_.offset ) );
        ptr_.segment = normalized_ptr.segment;
        ptr_.offset = normalized_ptr.offset;
    }

} // namespace

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

    namespace
    {
        void long_div( uint16_t& hi_, uint16_t& lo_, const uint16_t divider_ )
        {
            uint32_t val = ( hi_ << 16 ) + lo_;
            val /= divider_;
            hi_ = val >> 16;
            lo_ = val & 0xFFFF;
        }

    } // namespace

    void emu_read_some_file_sub_4( emu_t& e,
                                   const uint8_t byte_55_,
                                   emu_t::ptr16_t& executable_buffer_,
                                   const slice_t& executable_buffer_slice_ )
    {
        // these are not all ptr16 but more lo/hi int32 offsets (the pair looked liked pointers)
        emu_t::ptr16_t another_far_ptr;
        emu_t::ptr16_t another_pointer2;
        emu_t::ptr16_t also_a_pointer;

        e.ax = e.cs;
        e.ds = e.ax;
        e.dx = e.bx; // bx = gfx-block start = offset filename
        e.ah = 0x3D;
        e.al = 0;
        e.intr_0x21();
        // DOS - 2 + -OPEN DISK FILE WITH HANDLE
        // DS:DX->ASCIZ filename
        // AL = access mode
        //  0 - read
        assert( !e.flags.carry );

        e.si = e.bx;
        e.bx = e.ax;
        e.ah = 0x3F;
        e.cl = byte_55_;
        assert( ( e.cl & 0x18 ) != 0 );

        if( ( e.cl & 0x10 ) != 0 )
        {
            e.cx = 2;
            e.lds( e.dx, executable_buffer_ );
            e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
                           // BX = file handle, CX = number of bytes to read
                           // DS:DX->buffer
            assert( !e.flags.carry || ( e.ax == 2 ) );

            e.di = swap( *e.word_ptr( e.ds, e.dx ) );

            const uint32_t pos2 = e.memory<config_tat_t::executable_info_t>( e.cs, e.si )->byte_12h * 4;
            e.dx = lo( pos2 );
            e.cx = hi( pos2 );

            e.al = 1;
            e.ah = 0x42;
            e.intr_0x21(); // DOS - 2 + -MOVE FILE READ / WRITE POINTER(LSEEK)
                           // AL = method: offset from present location
            assert( !e.flags.carry );

            e.lds( e.dx, executable_buffer_ );
            e.ah = 0x3F;
            e.cx = 4;
            e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
                           // BX = file handle, CX = number of bytes to read
                           // DS:DX->buffer
            assert( !e.flags.carry && ( e.ax == 4 ) );

            const uint32_t pos = swap( *e.dword_ptr( e.ds, e.dx ) ) + ( e.di * 4 ) + 2;
            e.dx = lo( pos );
            e.cx = hi( pos );

            e.al = 0;
            e.ah = 0x42;
            e.intr_0x21(); // DOS - 2 + -MOVE FILE READ / WRITE POINTER(LSEEK)
                           // AL = method: offset from beginning of file
            assert( !e.flags.carry );
        }

        e.lds( e.dx, executable_buffer_ );
        e.ah = 0x3F;
        e.cx = 8;
        e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
                       // BX = file handle, CX = number of bytes to read
                       // DS:DX->buffer
        assert( !e.flags.carry && ( e.ax == 8 ) );

        const uint32_t* val32 = e.dword_ptr( executable_buffer_ );
        const uint32_t ofs1 = swap( val32[0] );
        const uint32_t ofs2 = swap( val32[1] );

        const uint32_t distance = ( ofs2 - ofs1 ) + 16;
        another_pointer2 = another_far_ptr = emu_t::ptr16( emu_t::offset32( executable_buffer_ ) + distance );

        e.si = lo( ofs1 );
        e.di = hi( ofs1 );
        also_a_pointer = 0;

        e.lds( e.dx, another_far_ptr );
        do
        {
            normalize_ptr( e.ds, e.dx );
            e.ax = 48000;
            e.sub( e.si, e.ax );
            e.sbb( e.di, 0 );
            if( e.flags.carry )
            {
                e.add( e.si, e.ax );
                e.ax = e.si;
                e.si = 0;
                e.di = 0;
            }

            e.cx = e.ax;
            e.ah = 0x3F;
            e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
                           // BX = file handle, CX = number of bytes to read
                           // DS:DX->buffer
            assert( !e.flags.carry );

            e.add( also_a_pointer.offset, e.ax );
            e.adc( also_a_pointer.segment, 0 );
            e.add( e.dx, e.ax );
            assert( e.ax == e.cx );
            e.ax = e.si;
        } while( ( e.ax | e.di ) != 0 );

        e.ah = 0x3E;
        e.intr_0x21(); // DOS - 2 + -CLOSE A FILE WITH HANDLE
                       // BX = file handle
        assert( ( byte_55_ & 0x18 ) != 0 );

        e.les( e.di, executable_buffer_ );

        another_far_ptr.offset = e.di;
        another_far_ptr.segment = e.es;
        e.ds = hi( ofs2 );
        e.si = lo( ofs2 );

        e.cx = e.ds;
        e.si += 16;
        e.add( e.cx, 0 );
        e.add( e.si, e.di );
        e.adc( e.cx, 0 );
        e.bx = e.si;

        long_div( e.cx, e.si, 16 );
        assert( e.cx == 0 );

        e.ax = e.es;
        e.add( e.ax, e.si );
        executable_buffer_.segment = e.ax;
        ++e.ax;
        e.ds = e.ax;
        e.bx &= 0x0F;
        executable_buffer_.offset = e.bx;
        e.cld();

        // some sort of uncompression, after that the executable is +sizeof(PSP) behind executable_buffer_begin
        uint8_t* src_buffer = e.byte_ptr( e.ds, 0 );
        uint8_t* dest_buffer = e.byte_ptr( e.es, e.di );

        emu_GAME_START_sub_3( src_buffer, dest_buffer, e.byte_ptr( another_pointer2 ) );

        //write_binary_file( "d:/temp/out.after_game_sub_3_call.BIN", executable_buffer_slice_.data,
        //                   executable_buffer_slice_.size );

        e.clc();
        return;
    }

} // namespace cleanup
