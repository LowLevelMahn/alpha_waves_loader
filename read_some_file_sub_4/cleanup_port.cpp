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
    void emu_GAME_START_sub_3( emu_t& e,
                               emu_t::ptr16_t src_buffer,
                               emu_t::ptr16_t dest_buffer,
                               emu_t::ptr16_t another_pointer2,
                               const slice_t& executable_buffer_slice_ )
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

        while( true )
        {
            assert( !e.flags.dir );
            ::memset( e.memory( e.ds, 0x301 ), 0, 128 * sizeof( uint16_t ) );

            // interrutp[1].offset is used as a temporary???
            //0:0: offset, segment interrupt 0
            //0:4: offset, segment interrupt 1 <---
            //...
            const uint16_t intr1_offset_value = *e.word_ptr( 0, 4 );
            assert( intr1_offset_value == 0 ); // seems to be always 0
            e.sub( dest_buffer.offset, intr1_offset_value );
            normalize_ptr( e.es, e.di );
            e.add( dest_buffer.offset, intr1_offset_value );

            normalize_ptr( another_pointer2 );
            assert( !e.flags.dir );
            something_t something{};
            ::memcpy( &something, e.memory( another_pointer2 ), sizeof( something ) );
            another_pointer2 += sizeof( something );

            e.dx = something.len2 + 1;
            if( something.len1 != 0 )
            {
                assert( !e.flags.dir );
                ::memcpy( e.memory( e.ds, 0x201 ), e.memory( another_pointer2 ), something.len1 );
                another_pointer2 += something.len1;

                ::memcpy( e.memory( e.ds, 0x001 ), e.memory( another_pointer2 ), something.len1 );
                another_pointer2 += something.len1;

                ::memcpy( e.memory( e.ds, 0x101 ), e.memory( another_pointer2 ), something.len1 );
                another_pointer2 += something.len1;

                for( uint16_t i = 0; i < something.len1; ++i )
                {
                    const uint8_t ofs = i + 1;
                    e.ax = *e.byte_ptr( e.ds, ofs + 0x200 );
                    uint8_t* at0x301 = e.byte_ptr( e.ds, e.ax + 0x301 );
                    *e.byte_ptr( e.ds, ofs + 0x402 ) = *at0x301;
                    *at0x301 = ofs;
                }

                auto loc_128_block = [&e]() {
                    e.ax = ( e.bl << 8 ) + *e.byte_ptr( e.ds, e.bx + 0x100 );
                    e.push( e.ax ); // save ax for loc_572_block
                    e.ax = *e.byte_ptr( e.ds, e.bx );
                };

                auto loc_572_block = [&e, &dest_buffer]() {
                    assert( !e.flags.dir );
                    *e.byte_ptr( dest_buffer++ ) = e.al;

                    e.pop( e.ax ); // restore ax from loc_128_block
                    if( e.ax == 0 )
                    {
                        return true; // goto loc_124;
                    }

                    e.bl = e.ah;
                    e.ah = 0;
                    return false;
                };

                for( uint16_t i = something.len2; i > 0; --i )
                {
                    assert( !e.flags.dir );

                    e.al = *e.byte_ptr( another_pointer2++ );

                    e.bx = e.ax;
                    const uint8_t val301_0 = *e.byte_ptr( e.ds, e.bx + 0x301 );
                    if( val301_0 == 0 )
                    {
                        *e.byte_ptr( dest_buffer++ ) = e.al;
                    }
                    else
                    {
                        e.bl = val301_0;
                        e.ax = 0;

                        e.push( e.ax );
                        loc_128_block(); // also e.push(e.ax)

                        bool end_inner_loop = false;
                        while( true )
                        {
                            e.bp = e.ax;
                            const uint8_t val301_1 = *e.byte_ptr( e.ds, e.bp + 0x301 );

                            if( val301_1 == 0 )
                            {
                                if( loc_572_block() )
                                {
                                    end_inner_loop = true;
                                    break;
                                }
                            }
                            else if( e.bl > val301_1 )
                            {
                                e.bl = val301_1;

                                loc_128_block();
                            }
                            else
                            {
                                e.al = e.bl;
                                e.bl = val301_1;
                                while( true )
                                {
                                    e.bl = *e.byte_ptr( e.ds, e.bx + 0x402 );
                                    if( e.bl == 0 )
                                    {
                                        e.ax = e.bp;
                                        if( loc_572_block() )
                                        {
                                            end_inner_loop = true;
                                        }
                                        break;
                                    }
                                    else if( e.bl < e.al )
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
            else
            {
                assert( !e.flags.dir );
                ::memcpy( e.memory( dest_buffer ), e.memory( another_pointer2 ), something.len2 );
                e.si += something.len2;
                dest_buffer += something.len2;
                another_pointer2 += something.len2;
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
                                   const slice_t& executable_buffer_slice_,
                                   std::vector<uint8_t>& before_game_sub_3_ )
    {
        uint16_t word_44{};
        uint16_t word_45{};

        // these are not all ptr16 but more lo/hi int32 offsets (the pair looked liked pointers)
        emu_t::ptr16_t some_game_ptr;
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
            e.di = e.dx;
            e.cx = swap( *e.word_ptr( e.ds, e.di ) );
            e.di = e.cx;
            e.al = e.memory<config_tat_t::executable_info_t>( e.cs, e.si )->byte_12h;
            e.ah = 0;
            e.ax *= 4;
            e.dx = e.ax;
            e.cx = 0;
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

            e.si = e.dx;
            e.cx = swap( *e.word_ptr( e.ds, e.si ) );
            e.dx = swap( *e.word_ptr( e.ds, e.si + 2 ) );
            e.di = ( e.di * 4 ) + 2;
            e.add( e.dx, e.di );
            e.adc( e.cx, 0 );
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

        e.lds( e.bp, executable_buffer_ );
        e.ax = swap( *e.word_ptr( e.ds, e.bp + 0 ) );
        e.cx = swap( *e.word_ptr( e.ds, e.bp + 2 ) );
        word_44 = e.cx;
        word_45 = e.ax;
        e.ax = swap( *e.word_ptr( e.ds, e.bp + 4 ) );
        e.cx = swap( *e.word_ptr( e.ds, e.bp + 6 ) );
        some_game_ptr.offset = e.cx;
        some_game_ptr.segment = e.ax;
        e.si = e.cx;
        e.di = e.ax;
        e.lds( e.cx, executable_buffer_ );
        e.add( e.si, e.cx );
        e.adc( e.di, 0 );
        e.sub( e.si, word_44 );
        e.sbb( e.di, word_45 );
        e.si += 16;
        e.adc( e.di, 0 );
        e.cx = e.si;

        long_div( e.di, e.si, 16 );
        assert( e.di == 0 );

        e.ax = e.ds;
        e.add( e.ax, e.si );
        e.ds = e.ax;
        e.cx &= 0x0F;

        another_far_ptr.offset = e.cx;
        another_far_ptr.segment = e.ds;
        another_pointer2.offset = e.cx;
        another_pointer2.segment = e.ds;
        e.si = word_44;
        e.di = word_45;
        also_a_pointer.offset = 0;
        also_a_pointer.segment = 0;
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

        uint16_t* intr1_offset = e.word_ptr( 0, 4 );
        *intr1_offset = e.di;

        another_far_ptr.offset = e.di;
        another_far_ptr.segment = e.es;
        e.lds( e.si, some_game_ptr );
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

        //write_binary_file("d:/temp/out.before_game_sub_3_call.BIN", executable_buffer_slice_.data, executable_buffer_slice_.size);
        {
            auto begin = executable_buffer_slice_.data + 0x100;
            auto end = begin + executable_buffer_slice_.size - 0x100;
            before_game_sub_3_ = { begin, end };
        }

        // some sort of uncompression, after that the executable is +sizeof(PSP) behind executable_buffer_begin
        emu_t::ptr16_t src_buffer{ e.ds, e.si };
        emu_t::ptr16_t dest_buffer{ e.es, e.di };
        emu_GAME_START_sub_3( e, src_buffer, dest_buffer, another_pointer2, executable_buffer_slice_ );

        write_binary_file( "d:/temp/out.after_game_sub_3_call.BIN", executable_buffer_slice_.data,
                           executable_buffer_slice_.size );

        e.clc();
        return;
    }

} // namespace cleanup
