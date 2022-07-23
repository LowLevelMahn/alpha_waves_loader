// DO NOT CHANGE THIS INITAL VERSION

#include "emu.hpp"
#include "types.hpp"
#include "helper.hpp"

namespace original
{
    void emu_GAME_START_sub_3( emu_t& e, emu_t::ptr16_t another_pointer2 )
    {
        std::array<uint8_t, 4> byte_57{}; // gets written
        uint8_t& byte_569 = byte_57[1];
        uint16_t& word_60 = *reinterpret_cast<uint16_t*>( &byte_57[2] );

    start:
        e.push( e.es );
        e.push( e.di );

        e.cx = 128;
        e.ax = e.ds;
        e.es = e.ax;
        e.di = 0x301;
        e.xor_w( e.ax, e.ax );
        e.rep_stosw();

        e.pop( e.di );
        e.pop( e.es );

        e.push( e.es );

        e.xor_w( e.ax, e.ax );
        e.es = e.ax;
        e.sub( e.di, *e.word_ptr( e.es, 4 ) );

        e.pop( e.es );

        e.ax = e.di;
        e.shr( e.ax, 1 );
        e.shr( e.ax, 1 );
        e.shr( e.ax, 1 );
        e.shr( e.ax, 1 );
        e.cx = e.es;
        e.add( e.cx, e.ax );
        e.es = e.cx;
        e.and_w( e.di, 0x0F );

        e.push( e.es );

        e.xor_w( e.ax, e.ax );
        e.es = e.ax;
        e.add( e.di, *e.word_ptr( e.es, 4 ) );

        e.pop( e.es );

        e.push( e.ds );
        e.push( e.es );
        e.push( e.si );
        e.push( e.di );

        //--- rep movsb prepare
        e.cx = 4;
        e.di = 0x152; // offset byte_57; NOT NEEDED
        e.ax = e.cs;
        e.es = e.ax;
        //---
        e.lds( e.si, another_pointer2 );
        e.ax = e.si;
        e.shr( e.ax, 1 );
        e.shr( e.ax, 1 );
        e.shr( e.ax, 1 );
        e.shr( e.ax, 1 );
        e.dx = e.ds;
        e.add( e.ax, e.dx );
        e.ds = e.ax;
        e.and_w( e.si, 0x0F );
        another_pointer2.offset = e.si;
        another_pointer2.segment = e.ds;
        e.add( another_pointer2.offset, e.cx );
#if 0
	e.rep_movsb(); // 4 bytes from DS:SI => ES:DI
#else
        //e.rep_movsb_dest(byte_57); // dest is native
        //e.rep_movsb_src(byte_57); // src is native
        ::memcpy( &byte_57, e.memory( e.ds, e.si ), 4 );
        e.si += 4;
        e.di += 4;
        e.cx = 0;
#endif
        e.pop( e.di );
        e.pop( e.si );
        e.pop( e.es );
        e.pop( e.ds );

        e.dx = word_60;
        e.inc( e.dx );
        e.cmp( byte_57[0], 0 );
        if( e.jnz() )
            goto loc_565;
        goto loc_566;
    loc_565:
        e.push( e.ds );
        e.push( e.es );
        e.push( e.di );

        e.xor_b( e.ch, e.ch );
        e.cl = byte_57[0];
        e.di = 0x201;
        e.ax = e.ds;
        e.es = e.ax;
        e.ds = another_pointer2.segment;
        e.si = another_pointer2.offset;
        e.add( another_pointer2.offset, e.cx );
        e.rep_movsb();
        e.cl = byte_57[0];
        e.xor_b( e.ch, e.ch );
        e.di = 1;
        e.add( another_pointer2.offset, e.cx );
        e.rep_movsb();
        e.cl = byte_57[0];
        e.di = 0x101;
        e.add( another_pointer2.offset, e.cx );
        e.rep_movsb();

        e.pop( e.di );
        e.pop( e.es );
        e.pop( e.ds );

        e.xor_b( e.ch, e.ch );
        e.cl = byte_57[0];
        e.xor_b( e.ah, e.ah );
        e.bx = 1;

    loc_567:
        e.al = *e.byte_ptr( e.ds, e.bx + 0x200 );
        e.si = e.ax;
        e.dl = *e.byte_ptr( e.ds, e.si + 0x301 );
        *e.byte_ptr( e.ds, e.bx + 0x402 ) = e.dl;
        *e.byte_ptr( e.ds, e.si + 0x301 ) = e.bl;
        e.inc( e.bx );
        if( e.loop() )
            goto loc_567;
        e.dx = word_60;
        e.inc( e.dx );
        e.cx = 1;

    loc_124:
        e.dec( e.dx );
        if( e.jnz() )
            goto loc_568;

    loc_577:
        e.cmp( byte_569, 0 );
        if( e.jz() )
            goto locret_570;
        goto start;

    locret_570:
        return;

    loc_568:
        e.push( e.ds );
        e.si = another_pointer2.segment;
        e.ds = e.si;
        e.si = another_pointer2.offset;
        e.lodsb();
        another_pointer2.offset = e.si;
        e.pop( e.ds );
        e.bx = e.ax;
        e.cmp( *e.byte_ptr( e.ds, e.bx + 0x301 ), 0 );
        if( e.jnz() )
            goto loc_571;
        e.stosb();
        goto loc_124;

    loc_571:
        e.bl = *e.byte_ptr( e.ds, e.bx + 0x301 );
        e.xor_w( e.ax, e.ax );
        e.push( e.ax );
        goto loc_128;

    loc_129:
        e.bp = e.ax;
        e.cmp( *e.byte_ptr( e.ds, e.bp + 0x301 ), 0 );
        if( e.jz() )
            goto loc_572;
        e.cmp( e.bl, *e.byte_ptr( e.ds, e.bp + 0x301 ) );
        if( e.ja() )
            goto loc_573;
        e.al = e.bl;
        e.bl = *e.byte_ptr( e.ds, e.bp + 0x301 );

    loc_575:
        e.bl = *e.byte_ptr( e.ds, e.bx + 0x402 );
        e.or_b( e.bl, e.bl );
        if( e.jz() )
            goto loc_574;
        e.cmp( e.bl, e.al );
        if( e.jb() )
            goto loc_128;
        goto loc_575;

    loc_573:
        e.bl = *e.byte_ptr( e.ds, e.bp + 0x301 );

    loc_128:
        e.al = *e.byte_ptr( e.ds, e.bx + 0x100 );
        e.ah = e.bl;
        e.push( e.ax );
        e.xor_b( e.ah, e.ah );
        e.al = *e.byte_ptr( e.ds, e.bx );
        goto loc_129;

    loc_574:
        e.ax = e.bp;
    loc_572:
        e.stosb();
        e.pop( e.ax );
        e.or_w( e.ax, e.ax );
        if( e.jnz() )
            goto loc_576;
        goto loc_124;
    loc_576:
        e.bl = e.ah;
        e.xor_b( e.ah, e.ah );
        goto loc_129;

    loc_566:
        e.push( e.ds );
        e.push( e.es );
        e.cx = word_60;
        e.push( e.cx );
        e.ds = another_pointer2.segment;
        e.si = another_pointer2.offset;
        e.add( another_pointer2.offset, e.cx );
        e.rep_movsb();
        e.pop( e.cx );
        e.pop( e.es );
        e.pop( e.ds );
        goto loc_577;
    }

    size_t emu_read_some_file_sub_4( emu_t& e,
                                     config_tat_t::executable_info_t* exec_info_,
                                     emu_t::ptr16_t& executable_buffer_ )
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
        char* filename = e.memory<char>( e.ds, e.dx );
        e.ah = 0x3D;
        e.al = 0;
        e.intr_0x21();
        // DOS - 2 + -OPEN DISK FILE WITH HANDLE
        // DS:DX->ASCIZ filename
        // AL = access mode
        //  0 - read
        if( e.jnb() )
            goto loc_578;

    loc_136:
        assert( false );
#if 0
        e.stc();
        return;
#endif

    loc_578:
        e.si = e.bx;
        e.bx = e.ax;
        e.ah = 0x3F;
        e.cl = exec_info_->byte_13h;
        e.test( e.cl, 0x18 );
        if( e.jnz() )
            goto loc_579;

        assert( false );
#if 0
        e.lds( e.cx, executable_buffer_ );
        word_44 = 0xFFFF;
        word_45 = 0xFFFF;
        goto loc_580;
#endif

    loc_579:
        e.test( e.cl, 0x10 );
        if( e.jz() )
            goto loc_581;
        e.cx = 2;
        e.lds( e.dx, executable_buffer_ );
        e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
                       // BX = file handle, CX = number of bytes to read
                       // DS:DX->buffer
        if( e.jb() )
            goto loc_136;
        e.cmp( e.ax, 2 );

        if( e.jnz() )
            goto loc_136;
        e.di = e.dx;
        e.cx = *e.word_ptr( e.ds, e.di );
        e.xchg( e.cl, e.ch );
        e.di = e.cx;
        e.al = e.memory<config_tat_t::executable_info_t>( e.cs, e.si )->byte_12h;
        // e.al = block-nr
        e.xor_b( e.ah, e.ah );
        e.shl( e.ax, 1 );
        e.shl( e.ax, 1 );
        e.dx = e.ax;
        e.xor_w( e.cx, e.cx );
        e.al = 1;
        e.ah = 0x42;
        e.intr_0x21(); // DOS - 2 + -MOVE FILE READ / WRITE POINTER(LSEEK)
                       // AL = method: offset from present location
        if( e.jb() )
            goto loc_136;

        e.lds( e.dx, executable_buffer_ );
        e.ah = 0x3F;
        e.cx = 4;
        e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
                       // BX = file handle, CX = number of bytes to read
                       // DS:DX->buffer
        if( e.jb() )
            goto loc_136;
        e.cmp( e.ax, 4 );
        if( e.jnz() )
            goto loc_136;

        e.si = e.dx;
        e.cx = *e.word_ptr( e.ds, e.si );
        e.xchg( e.cl, e.ch );
        e.dx = *e.word_ptr( e.ds, e.si + 2 );
        e.xchg( e.dl, e.dh );
        e.shl( e.di, 1 );
        e.shl( e.di, 1 );
        e.add( e.di, 2 );
        e.add( e.dx, e.di );
        e.adc( e.cx, 0 );
        e.al = 0;
        e.ah = 0x42;
        e.intr_0x21(); // DOS - 2 + -MOVE FILE READ / WRITE POINTER(LSEEK)
                       // AL = method: offset from beginning of file
        if( e.jb() )
            goto loc_139;

    loc_581:
        e.lds( e.dx, executable_buffer_ );
        e.ah = 0x3F;
        e.cx = 8;
        e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
                       // BX = file handle, CX = number of bytes to read
                       // DS:DX->buffer
        if( e.jb() )
            goto loc_139;
        e.cmp( e.ax, 8 );

        if( e.jz() )
            goto loc_582;

    loc_139:
        assert( false );
#if 0
        e.stc();
        return;
#endif

    loc_582:
        e.lds( e.bp, executable_buffer_ );
        e.ax = *e.word_ptr( e.ds, e.bp + 0 );
        e.cx = *e.word_ptr( e.ds, e.bp + 2 );
        e.xchg( e.al, e.ah );
        e.xchg( e.cl, e.ch );
        word_44 = e.cx;
        word_45 = e.ax;
        const uint32_t packed_size = ( word_45 << 16 ) + word_44;

        e.ax = *e.word_ptr( e.ds, e.bp + 4 );
        e.cx = *e.word_ptr( e.ds, e.bp + 6 );
        e.xchg( e.al, e.ah );
        e.xchg( e.cl, e.ch );

        const uint32_t unpacked_size = ( e.ax << 16 ) + e.cx;

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

    loc_580:
        another_far_ptr.offset = e.cx;
        another_far_ptr.segment = e.ds;
        another_pointer2.offset = e.cx;
        another_pointer2.segment = e.ds;
        e.si = word_44;
        e.di = word_45;
        also_a_pointer.offset = 0;
        also_a_pointer.segment = 0;
        e.lds( e.dx, another_far_ptr );

    loc_586:
        e.cx = e.dx;
        e.shr( e.cx, 1 );
        e.shr( e.cx, 1 );
        e.shr( e.cx, 1 );
        e.shr( e.cx, 1 );
        e.ax = e.ds;
        e.add( e.ax, e.cx );
        e.ds = e.ax;
        e.and_w( e.dx, 0x0F );
        e.ax = 48000;
        e.sub( e.si, e.ax );
        e.sbb( e.di, 0 );
        if( e.jnb() )
            goto loc_583;
        e.add( e.si, e.ax );
        e.ax = e.si;
        e.xor_w( e.si, e.si );
        e.xor_w( e.di, e.di );

    loc_583:
        e.cx = e.ax;
        e.ah = 0x3F;
        e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
        // BX = file handle, CX = number of bytes to read
        // DS:DX->buffer
        if( e.jnb() )
            goto loc_584;
        assert( false );
#if 0
        return;
#endif

    loc_584:
        e.add( also_a_pointer.offset, e.ax );
        e.adc( also_a_pointer.segment, 0 );
        e.add( e.dx, e.ax );
        e.cmp( e.ax, e.cx );
        if( e.jnz() )
            goto loc_585;
        e.ax = e.si;
        e.or_w( e.ax, e.di );
        if( e.jnz() )
            goto loc_586;

    loc_585:
        e.ah = 0x3E;
        e.intr_0x21(); // DOS - 2 + -CLOSE A FILE WITH HANDLE
        // BX = file handle
        e.test( exec_info_->byte_13h, 0x18 );
        if( e.jz() )
            goto loc_587;
        e.les( e.di, executable_buffer_ );
        e.push( e.es );
        e.xor_w( e.ax, e.ax );
        e.es = e.ax;
        *e.word_ptr( e.es, 4 ) = e.di;
        e.pop( e.es );
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

        // some sort of uncompression, after that the executable is +sizeof(PSP) behind executable_buffer_begin
        emu_GAME_START_sub_3( e, another_pointer2 );

        e.clc();
        return unpacked_size;

    loc_587:
        assert( false );
#if 0
        e.lds( e.si, also_a_pointer.offset );
        some_game_ptr.offset = e.si;
        some_game_ptr.segment = e.ds;
        e.clc();
        return;
#endif
    }

} // namespace original
