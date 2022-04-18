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
    void emu_GAME_START_sub_3( emu_t& e, emu_t::ptr16_t another_pointer2, const slice_t& executable_buffer_slice_ )
    {
        std::array<uint8_t, 4> byte_57{}; // gets written, some sort of length
        uint8_t& byte_569 = byte_57[1];
        uint16_t& word_60 = *reinterpret_cast<uint16_t*>( &byte_57[2] );

    start:
        assert( !e.flags.dir );
        ::memset( e.memory( e.ds, 0x301 ), 0, 128 * sizeof( uint16_t ) );

        // interrutp[1].offset is used as a temporary???
        //0:0: offset, segment interrupt 0
        //0:4: offset, segment interrupt 1
        //...
        const uint16_t intr1_offset_value = *e.word_ptr( 0, 4 );
        assert( intr1_offset_value == 0 ); // seems to be always 0
        e.sub( e.di, intr1_offset_value );
        normalize_ptr( e.es, e.di );
        e.add( e.di, intr1_offset_value );

        normalize_ptr( another_pointer2 );
        assert( !e.flags.dir );
        ::memcpy( &byte_57, e.memory( another_pointer2 ), 4 );
        e.add( another_pointer2.offset, 4 );

        e.dx = word_60 + 1;
        if( byte_57[0] != 0 )
        {
            assert( !e.flags.dir );
            ::memcpy( e.memory( e.ds, 0x201 ), e.memory( another_pointer2 ), byte_57[0] );
            another_pointer2.offset += byte_57[0];

            ::memcpy( e.memory( e.ds, 1 ), e.memory( another_pointer2 ), byte_57[0] );
            another_pointer2.offset += byte_57[0];

            ::memcpy( e.memory( e.ds, 0x101 ), e.memory( another_pointer2 ), byte_57[0] );
            another_pointer2.offset += byte_57[0];

            for( uint16_t i = 0; i < byte_57[0]; ++i )
            {
                const uint8_t ofs = i + 1;
                e.ax = *e.byte_ptr( e.ds, ofs + 0x200 );
                uint8_t* at0x301 = e.byte_ptr( e.ds, e.ax + 0x301 );
                *e.byte_ptr( e.ds, ofs + 0x402 ) = *at0x301;
                *at0x301 = ofs;
            }

            e.dx = word_60;
            ++e.dx;

        loc_124:
            --e.dx;
            if( e.dx != 0 )
            {
                goto loc_568;
            }

        loc_577:
            if( byte_569 == 0 )
            {
                return;
            }
            goto start;

        loc_568:
            assert( !e.flags.dir );

            e.al = *e.byte_ptr( another_pointer2 );
            ++another_pointer2.offset;

            e.bx = e.ax;
            if( *e.byte_ptr( e.ds, e.bx + 0x301 ) != 0 )
            {
                e.bl = *e.byte_ptr( e.ds, e.bx + 0x301 );
                e.ax = 0;
                e.push( e.ax );
                goto loc_128;
            }

            *e.byte_ptr( e.es, e.di ) = e.al;
            ++e.di;

            goto loc_124;

        loc_129:
            e.bp = e.ax;
            uint8_t val0x301 = *e.byte_ptr( e.ds, e.bp + 0x301 );
            if( val0x301 == 0 )
            {
                goto loc_572;
            }
            if( e.bl > val0x301 )
            {
                goto loc_573;
            }
            e.al = e.bl;
            e.bl = val0x301;

        loc_575:
            e.bl = *e.byte_ptr( e.ds, e.bx + 0x402 );
            if( e.bl == 0 )
            {
                goto loc_574;
            }
            if( e.bl < e.al )
            {
                goto loc_128;
            }
            goto loc_575;

        loc_573:
            e.bl = *e.byte_ptr( e.ds, e.bp + 0x301 );

        loc_128:
            e.al = *e.byte_ptr( e.ds, e.bx + 0x100 );
            e.ah = e.bl;
            e.push( e.ax );
            e.ah = 0;
            e.al = *e.byte_ptr( e.ds, e.bx );
            goto loc_129;

        loc_574:
            e.ax = e.bp;
        loc_572:
            assert( !e.flags.dir );
            *e.byte_ptr( e.es, e.di ) = e.al;
            ++e.di;

            e.pop( e.ax );
            if( e.ax != 0 )
            {
                e.bl = e.ah;
                e.ah = 0;
                goto loc_129;
            }

            goto loc_124;
        }

        assert( !e.flags.dir );
        ::memcpy( e.memory( e.es, e.di ), e.memory( another_pointer2 ), word_60 );
        e.si += word_60;
        e.di += word_60;
        another_pointer2.offset += word_60;
        goto loc_577;
    }

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
        if( e.flags.carry )
        {
            assert( false );
        }

        e.si = e.bx;
        e.bx = e.ax;
        e.ah = 0x3F;
        e.cl = byte_55_;
        if( ( e.cl & 0x18 ) == 0 )
        {
            assert( false );
        }

        if( ( e.cl & 0x10 ) != 0 )
        {
            e.cx = 2;
            e.lds( e.dx, executable_buffer_ );
            e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
                           // BX = file handle, CX = number of bytes to read
                           // DS:DX->buffer
            if( e.flags.carry || ( e.ax != 2 ) )
            {
                assert( false );
            }
            e.di = e.dx;
            e.cx = *e.word_ptr( e.ds, e.di );
            e.xchg( e.cl, e.ch );
            e.di = e.cx;
            e.al = e.memory<config_tat_t::executable_info_t>( e.cs, e.si )->byte_12h;
            e.ah = 0;
            e.shl( e.ax, 2 );
            e.dx = e.ax;
            e.cx = 0;
            e.al = 1;
            e.ah = 0x42;
            e.intr_0x21(); // DOS - 2 + -MOVE FILE READ / WRITE POINTER(LSEEK)
                           // AL = method: offset from present location
            if( e.flags.carry )
            {
                assert( false );
            }

            e.lds( e.dx, executable_buffer_ );
            e.ah = 0x3F;
            e.cx = 4;
            e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
                           // BX = file handle, CX = number of bytes to read
                           // DS:DX->buffer
            if( e.flags.carry || ( e.ax != 4 ) )
            {
                assert( false );
            }

            e.si = e.dx;
            e.cx = *e.word_ptr( e.ds, e.si );
            e.xchg( e.cl, e.ch );
            e.dx = *e.word_ptr( e.ds, e.si + 2 );
            e.xchg( e.dl, e.dh );
            e.shl( e.di, 2 );
            e.add( e.di, 2 );
            e.add( e.dx, e.di );
            e.adc( e.cx, 0 );
            e.al = 0;
            e.ah = 0x42;
            e.intr_0x21(); // DOS - 2 + -MOVE FILE READ / WRITE POINTER(LSEEK)
                           // AL = method: offset from beginning of file
            if( e.flags.carry )
            {
                assert( false );
            }
        }

        e.lds( e.dx, executable_buffer_ );
        e.ah = 0x3F;
        e.cx = 8;
        e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
                       // BX = file handle, CX = number of bytes to read
                       // DS:DX->buffer
        if( e.flags.carry || ( e.ax != 8 ) )
        {
            assert( false );
        }

        e.lds( e.bp, executable_buffer_ );
        e.ax = *e.word_ptr( e.ds, e.bp + 0 );
        e.cx = *e.word_ptr( e.ds, e.bp + 2 );
        e.xchg( e.al, e.ah );
        e.xchg( e.cl, e.ch );
        word_44 = e.cx;
        word_45 = e.ax;
        e.ax = *e.word_ptr( e.ds, e.bp + 4 );
        e.cx = *e.word_ptr( e.ds, e.bp + 6 );
        e.xchg( e.al, e.ah );
        e.xchg( e.cl, e.ch );
        some_game_ptr.offset = e.cx;
        some_game_ptr.segment = e.ax;
        e.si = e.cx;
        e.di = e.ax;
        e.lds( e.cx, executable_buffer_ );
        e.add( e.si, e.cx );
        e.adc( e.di, 0 );
        e.sub( e.si, word_44 );
        e.sbb( e.di, word_45 );
        e.add( e.si, 0x10 );
        e.adc( e.di, 0 );
        e.cx = e.si;
        e.shr( e.di, 1 );
        e.rcr( e.si, 1 );
        e.shr( e.di, 1 );
        e.rcr( e.si, 1 );
        e.shr( e.di, 1 );
        e.rcr( e.si, 1 );
        e.shr( e.di, 1 );
        e.rcr( e.si, 1 );
        e.ax = e.ds;
        e.add( e.ax, e.si );
        e.ds = e.ax;
        e.and_w( e.cx, 0x0F );

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
            e.cx = e.dx;
            e.shr( e.cx, 4 );
            e.ax = e.ds;
            e.add( e.ax, e.cx );
            e.ds = e.ax;
            e.and_w( e.dx, 0x0F );
            e.ax = 48000;
            e.sub( e.si, e.ax );
            e.sbb( e.di, 0 );
            if( !e.jnb() )
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
            if( e.flags.carry )
            {
                assert( false );
            }

            e.add( also_a_pointer.offset, e.ax );
            e.adc( also_a_pointer.segment, 0 );
            e.add( e.dx, e.ax );
            if( e.ax != e.cx )
            {
                break;
            }
            e.ax = e.si;
        } while( ( e.ax | e.di ) != 0 );

        e.ah = 0x3E;
        e.intr_0x21(); // DOS - 2 + -CLOSE A FILE WITH HANDLE
        // BX = file handle
        if( ( byte_55_ & 0x18 ) == 0 )
        {
            assert( false );
        }
        e.les( e.di, executable_buffer_ );

        uint16_t* intr1_offset = e.word_ptr( 0, 4 );
        *intr1_offset = e.di;

        another_far_ptr.offset = e.di;
        another_far_ptr.segment = e.es;
        e.lds( e.si, some_game_ptr );
        e.cx = e.ds;
        e.add( e.si, 0x10 );
        e.add( e.cx, 0 );
        e.add( e.si, e.di );
        e.adc( e.cx, 0 );
        e.bx = e.si;
        e.shr( e.cx, 1 );
        e.rcr( e.si, 1 );
        e.shr( e.cx, 1 );
        e.rcr( e.si, 1 );
        e.shr( e.cx, 1 );
        e.rcr( e.si, 1 );
        e.shr( e.cx, 1 );
        e.rcr( e.si, 1 );
        e.ax = e.es;
        e.add( e.ax, e.si );
        executable_buffer_.segment = e.ax;
        e.inc( e.ax );
        e.ds = e.ax;
        e.and_w( e.bx, 0x0F );
        executable_buffer_.offset = e.bx;
        e.cld();

        //write_binary_file("d:/temp/out.before_game_sub_3_call.BIN", executable_buffer_slice_.data, executable_buffer_slice_.size);
        {
            auto begin = executable_buffer_slice_.data + 0x100;
            auto end = begin + executable_buffer_slice_.size - 0x100;
            before_game_sub_3_ = { begin, end };
        }

        // some sort of uncompression, after that the executable is +sizeof(PSP) behind executable_buffer_begin
        emu_GAME_START_sub_3( e, another_pointer2, executable_buffer_slice_ );

        write_binary_file( "d:/temp/out.after_game_sub_3_call.BIN", executable_buffer_slice_.data,
                           executable_buffer_slice_.size );

        e.clc();
        return;
    }

} // namespace cleanup
