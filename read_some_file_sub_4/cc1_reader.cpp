#include "helper.hpp"
#include <cassert>

#include "emu.hpp"
#include "types.hpp"
#include "helper.hpp"

#include "emu.hpp"
#include "types.hpp"
#include "helper.hpp"

#include <cstring>

namespace second_try
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
        e.xor( e.ax, e.ax );
        e.rep_stosw();

        e.pop( e.di );
        e.pop( e.es );

        e.push( e.es );

        e.xor( e.ax, e.ax );
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
        e.and( e.di, 0x0F );

        e.push( e.es );

        e.xor( e.ax, e.ax );
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
        e.and( e.si, 0x0F );
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

        e.xor( e.ch, e.ch );
        e.cl = byte_57[0];
        e.di = 0x201;
        e.ax = e.ds;
        e.es = e.ax;
        e.ds = another_pointer2.segment;
        e.si = another_pointer2.offset;
        e.add( another_pointer2.offset, e.cx );
        e.rep_movsb();
        e.cl = byte_57[0];
        e.xor( e.ch, e.ch );
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

        e.xor( e.ch, e.ch );
        e.cl = byte_57[0];
        e.xor( e.ah, e.ah );
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
        e.xor( e.ax, e.ax );
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
        e.or( e.bl, e.bl );
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
        e.xor( e.ah, e.ah );
        e.al = *e.byte_ptr( e.ds, e.bx );
        goto loc_129;

    loc_574:
        e.ax = e.bp;
    loc_572:
        e.stosb();
        e.pop( e.ax );
        e.or( e.ax, e.ax );
        if( e.jnz() )
            goto loc_576;
        goto loc_124;
    loc_576:
        e.bl = e.ah;
        e.xor( e.ah, e.ah );
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
        e.xor( e.ah, e.ah );
        e.shl( e.ax, 1 );
        e.shl( e.ax, 1 );
        e.dx = e.ax;
        e.xor( e.cx, e.cx );
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
        e.and( e.cx, 0x0F );

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
        e.and( e.dx, 0x0F );
        e.ax = 48000;
        e.sub( e.si, e.ax );
        e.sbb( e.di, 0 );
        if( e.jnb() )
            goto loc_583;
        e.add( e.si, e.ax );
        e.ax = e.si;
        e.xor( e.si, e.si );
        e.xor( e.di, e.di );

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
        e.or( e.ax, e.di );
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
        e.xor( e.ax, e.ax );
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
        e.and( e.bx, 0x0F );
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

} // namespace second_try


void emu_GAME_START_sub_3( std::vector<uint8_t>& uncompressed_buffer_, const std::vector<uint8_t>& compressed_buffer_ );

struct data_block_t
{
    uint32_t packed_size{};
    uint32_t unpacked_size{};
    std::vector<uint8_t> data;
};

std::vector<data_block_t> read_cc1_file( const std::string& filepath_ )
{
    const std::vector<uint8_t> content = read_binary_file( filepath_ );

    const uint8_t* current = content.data();

    uint16_t offset_count = swap( *(uint16_t*)current );
    current += sizeof( uint16_t );

    std::vector<uint32_t> offset_table( offset_count );
    for( size_t i = 0; i < offset_count; ++i )
    {
        uint32_t offset = swap( *(uint32_t*)current );
        current += sizeof( uint32_t );
        offset_table[i] = offset;
    }

    std::vector<data_block_t> data_blocks( offset_count );

    for( size_t i = 0; i < offset_count; ++i )
    {
        uint32_t data_size = swap( *(uint32_t*)current );
        current += sizeof( uint32_t );

        uint32_t packed_data_size = swap( *(uint32_t*)current );
        current += sizeof( uint32_t );

        std::vector<uint8_t> data( data_size );
        ::memcpy( data.data(), current, data_size );
        current += data_size;

        data_blocks[i] = { data_size, packed_data_size, data };
    }

    // size of the parts + offsets fits exact the file size? no gaps?
    size_t result_size = sizeof( uint16_t ) + offset_count * sizeof( uint32_t );
    for( size_t i = 0; i < offset_count; ++i )
    {
        result_size += sizeof( uint32_t ) + sizeof( uint32_t ) + data_blocks[i].packed_size;
    }
    assert( result_size == content.size() );

    return data_blocks;
}

std::vector<uint8_t> unpack( const std::string file_path_, const std::string& file_name_, size_t block_nr_ )
{
    emu_t e;

    e.dos_current_dir( file_path_ );

    std::fill( e.memory().begin(), e.memory().end(), 0xBB );

    config_tat_t::gfx_info_t blocks;
    const auto filename = blocks.executable_info_array[0].filename; // cc1-file
    ::strcpy( blocks.executable_info_array[0].filename.data(), file_name_.c_str() );
    blocks.executable_info_array[0].byte_12h = block_nr_;
    blocks.executable_info_array[0].byte_13h = 0x18;

    const emu_t::ptr16_t block_begin_ptr16( 0, 0x100D );
    const size_t blocks_begin = emu_t::offset32( block_begin_ptr16 );
    const size_t blockes_end = blocks_begin + sizeof( blocks );

    constexpr size_t executable_buffer_size = 0x9D160;
    const emu_t::ptr16_t executable_buffer_begin_ptr16( 0x2E9, 0 );
    const size_t executable_buffer_begin = emu_t::offset32( executable_buffer_begin_ptr16 );
    const size_t executable_buffer_end = executable_buffer_begin + executable_buffer_size;
    slice_t executable_buffer_slice{ e.byte_ptr( executable_buffer_begin ), executable_buffer_size };

    ::memcpy( e.memory( blocks_begin ), &blocks, sizeof( blocks ) );
    // 100% equal register content as in dosbox
    e.cs = block_begin_ptr16.segment;
    e.bx = block_begin_ptr16.offset + ( static_cast<uint16_t>( 0 ) * sizeof( config_tat_t::executable_info_t ) );

    emu_t::ptr16_t executable_buffer_ptr16 = executable_buffer_begin_ptr16;

    //emu_GAME_START_sub_6( e, executable_buffer_ptr16 );

    size_t unpacked_size = second_try::emu_read_some_file_sub_4(
        e, e.memory<config_tat_t::executable_info_t>( e.cs, e.bx ), executable_buffer_ptr16 );

    const size_t distance =
        emu_t::offset32( executable_buffer_ptr16 ) - emu_t::offset32( executable_buffer_begin_ptr16 );

    auto executable_begin = e.byte_ptr( executable_buffer_begin ) + 0x100;
    auto executable_end = executable_begin + unpacked_size;

    return { executable_begin, executable_end };
}

void cc1_read_test()
{
    const std::string game_root = R"(F:\projects\fun\dos_games_rev\alpha_waves_dev\tests\alpha)";

#if 1
    const std::vector<std::string> files{ "PROGS.CC1",   "GRAPHS.CC1",  "MUSIC_A.CC1",
                                          "MUSIC_B.CC1", "MUSIC_T.CC1", "TEXTES.CC1" };
#else
    const std::vector<std::string> files{ "TEXTES.CC1" };
#endif

    for( const auto& file : files )
    {
        const std::string in_filepath = game_root + "\\" + file;
        const std::string out_filepath = game_root + "\\cc1_extract\\" + file;
        printf( "%s\n", in_filepath.c_str() );
        std::vector<data_block_t> data_blocks = read_cc1_file( in_filepath );

#if 1
        //print info
        printf( "blocks\n" );
        for( size_t i = 0; i < data_blocks.size(); ++i )
        {
            auto& db = data_blocks[i];
            printf( "  [%u] packed_size: %u, unpacked_size: %u\n", i, db.packed_size, db.unpacked_size );

            //if( file == "TEXTES.CC1" && i == 0 )
            //{
            //    int brk = 1;
            //}


            //std::vector<uint8_t> unpacked = unpack( game_root, file, i );
            //assert( unpacked.size() == db.unpacked_size );

            // ???
            // my pure C++ Port works sometimes BETTER then my emulated original Version (wrong setup in this test?)
            // but my C++ Port fails with most of GRAPHS.CC1
            // ???

            //if( file == "TEXTES.CC1" )
            //{
            std::vector<uint8_t> unpacked2( db.unpacked_size );
            //std::fill( unpacked2.begin(), unpacked2.end(), 0xBB );
            emu_GAME_START_sub_3( unpacked2, db.data );
            int bd = 1;
            //}

    #if 1
            char filename[100]{};
            sprintf( filename, "%s_block%u.bin", out_filepath.c_str(), i );
            write_binary_file( filename, unpacked2.data(), unpacked2.size() );
    #endif
        }
#endif
    }

    int brk = 1;
}

uint16_t* to_uint16_ptr( uint32_t& value_ )
{
    return reinterpret_cast<uint16_t*>( &value_ );
}

struct dword_t : emu_t
{
    dword_t( uint32_t& value_ ) : value( value_ )
    {
    }

    uint32_t& value;
    uint16_t& lo = to_uint16_ptr( value )[0];
    uint16_t& hi = to_uint16_ptr( value )[1];
};

void another_test_emu( emu_t& e )
{
#if 0
    uint32_t x = 0xAABBCCDD;
    dword_t d( x );
    uint32_t v = d.value;
    uint16_t l = d.lo;
    uint16_t h = d.hi;
#endif

    uint16_t& word_1BAA2 = *e.word_ptr( e.cs, 0xBAA2 );
    dword_t dword_1BAA4{ *e.dword_ptr( e.cs, 0xBAA4 ) };
    uint16_t& word_1BA9C = *e.word_ptr( e.cs, 0xBA9C );
    uint8_t& byte_1BA9A = *e.byte_ptr( e.cs, 0xBA9A );
    uint8_t& byte_1BA9B = *e.byte_ptr( e.cs, 0xBA9B );

    const uint8_t* buf1 = e.byte_ptr( e.ds, e.si );
    const uint8_t* buf2 = e.byte_ptr( e.es, e.di );

    write_binary_file( "d:/temp/alpha/buf1.begin.bin", buf1, 4000 );
    write_binary_file( "d:/temp/alpha/buf2.begin.bin", buf2, 4000 );

begin:
#if 0
    e.push( e.es );
    e.push( e.di );

    e.cx = 0x80;
    e.ax = e.ds;
    e.es = e.ax;
    e.di = 0x301;
    e.xor( e.ax, e.ax );
    e.rep_stosw();

    e.pop( e.di );
    e.pop( e.es );
#else
    ::memset( e.byte_ptr( e.ds, 0x301 ), 0, 2 * 0x80 );
#endif

    e.sub( e.di, word_1BAA2 );
    e.ax = e.di;
    e.shr( e.ax, 1 );
    e.shr( e.ax, 1 );
    e.shr( e.ax, 1 );
    e.shr( e.ax, 1 );
    e.cx = e.es;
    e.add( e.cx, e.ax );
    e.es = e.cx;
    e.and( e.di, 0x0F );
    e.add( e.di, word_1BAA2 );

    e.push( e.ds );
    e.push( e.es );
    e.push( e.si );
    e.push( e.di );
    e.cx = 4;
    e.les( e.di, byte_1BA9A );
    e.lds( e.si, dword_1BAA4.value );
    e.ax = e.si;
    e.shr( e.ax, 1 );
    e.shr( e.ax, 1 );
    e.shr( e.ax, 1 );
    e.shr( e.ax, 1 );
    e.dx = e.ds;
    e.add( e.ax, e.dx );
    e.ds = e.ax;
    e.and( e.si, 0x0F );
    dword_1BAA4.lo = e.si;
    dword_1BAA4.hi = e.ds;
    e.add( dword_1BAA4.lo, e.cx );
    e.rep_movsb();
    e.pop( e.di );
    e.pop( e.si );
    e.pop( e.es );
    e.pop( e.ds );

    e.dx = word_1BA9C;
    e.inc( e.dx );
    e.cmp( byte_1BA9A, 0 );
    if( e.jnz() )
    {
        goto loc_1BB63;
    }
    goto loc_1BC52;

loc_1BB63:
    e.push( e.ds );
    e.push( e.es );
    e.push( e.di );
    e.xor( e.ch, e.ch );
    e.cl = byte_1BA9A;
    e.di = 0x201;
    e.ax = e.ds;
    e.es = e.ax;
    e.ds = dword_1BAA4.hi;
    e.si = dword_1BAA4.lo;
    e.add( dword_1BAA4.lo, e.cx );
    e.rep_movsb();
    e.cl = byte_1BA9A;
    e.xor( e.ch, e.ch );
    e.di = 1;
    e.add( dword_1BAA4.lo, e.cx );
    e.rep_movsb();
    e.cl = byte_1BA9A;
    e.di = 0x101;
    e.add( dword_1BAA4.lo, e.cx );
    e.rep_movsb();
    e.pop( e.di );
    e.pop( e.es );
    e.pop( e.ds );
    e.xor( e.ch, e.ch );
    e.cl = byte_1BA9A;
    e.xor( e.ah, e.ah );
    e.bx = 1;

loc_1BBB4:
    e.al = *e.byte_ptr( e.ds, e.bx + 0x200 );
    e.si = e.ax;
    e.dl = *e.byte_ptr( e.ds, e.si + 0x301 );
    *e.byte_ptr( e.ds, e.bx + 0x402 ) = e.dl;
    *e.byte_ptr( e.ds, e.si + 0x301 ) = e.bl;
    e.inc( e.bx );
    if( e.loop() )
    {
        goto loc_1BBB4;
    }
    e.dx = word_1BA9C;
    e.inc( e.dx );
    e.cx = 1;

loc_1BBD2:

    e.dec( e.dx );
    if( e.jnz() )
    {
        goto loc_1BBE1;
    }

loc_1BBD5:
    e.cmp( byte_1BA9B, 0 );
    if( e.jz() )
    {
        goto locret_1BBE0;
    }
    goto begin;

locret_1BBE0:
    write_binary_file( "d:/temp/alpha/buf1.end.bin", buf1, 4000 );
    write_binary_file( "d:/temp/alpha/buf2.end.bin", buf2, 4000 );
    return;

loc_1BBE1:
    e.push( e.ds );
    e.si = dword_1BAA4.hi;
    e.ds = e.si;
    e.si = dword_1BAA4.lo;
    e.lodsb();
    dword_1BAA4.lo = e.si;
    e.pop( e.ds );
    e.bx = e.ax;
    e.cmp( *e.byte_ptr( e.ds, e.bx + 0x301 ), 0 );
    if( e.jnz() )
    {
        goto loc_1BC01;
    }
    e.stosb();
    goto loc_1BBD2;

loc_1BC01:
    e.bl = *e.byte_ptr( e.ds, e.bx + 0x301 );
    e.xor( e.ax, e.ax );
    e.push( e.ax );
    goto loc_1BC35;

loc_1BC0A:
    e.bp = e.ax;
    e.cmp( *e.byte_ptr( e.ds, e.bp + 0x301 ), 0 );
    if( e.jz() )
    {
        goto loc_1BC44;
    }
    e.cmp( e.bl, *e.byte_ptr( e.ds, e.bp + 0x301 ) );
    if( e.ja() )
    {
        goto loc_1BC30;
    }
    e.al = e.bl;
    e.bl = *e.byte_ptr( e.ds, e.bp + 0x301 );

loc_1BC22:
    e.bl = *e.byte_ptr( e.ds, e.bx + 0x402 );
    e.or( e.bl, e.bl );
    if( e.jz() )
    {
        goto loc_1BC42;
    }
    e.cmp( e.bl, e.al );
    if( e.jb() )
    {
        goto loc_1BC35;
    }
    goto loc_1BC22;

loc_1BC30:
    e.bl = *e.byte_ptr( e.ds, e.bp + 0x301 );

loc_1BC35:
    e.al = *e.byte_ptr( e.ds, e.bx + 0x100 );
    e.ah = e.bl;
    e.push( e.ax );
    e.xor( e.ah, e.ah );
    e.al = *e.byte_ptr( e.ds, e.bx );
    goto loc_1BC0A;
loc_1BC42:
    e.ax = e.bp;

loc_1BC44:
    e.stosb();
    e.pop( e.ax );
    e.or( e.ax, e.ax );
    if( e.jnz() )
    {
        goto loc_1BC4C;
    }
    goto loc_1BBD2;

loc_1BC4C:
    e.bl = e.ah;
    e.xor( e.ah, e.ah );
    goto loc_1BC0A;

loc_1BC52:
    e.push( e.ds );
    e.push( e.es );
    e.cx = word_1BA9C;
    e.push( e.cx );
    e.ds = dword_1BAA4.hi;
    e.si = dword_1BAA4.lo;
    e.add( dword_1BAA4.lo, e.cx );
    e.rep_movsb();
    e.pop( e.cx );
    e.pop( e.es );
    e.pop( e.ds );
    goto loc_1BBD5;
}

void another_test()
{
    const std::string path = "D:\\temp\\alpha";

    // block0
    {
        const std::vector<uint8_t> dump_before = read_binary_file( path + "\\block_ 0_dump_before.bin" );
        const std::vector<uint8_t> dump_after = read_binary_file( path + "\\block_ 0_dump_after.bin" );

        emu_t e;

        // DS=0x184B, SI=0x000B, ES=0x1913, DI=0x0000, CS = 0x01A2
        e.ds = 0x184B;
        e.si = 0x000B;
        e.es = 0x1913;
        e.di = 0x0000;
        e.cs = 0x01A2;

        assert( e.memory().size() == dump_before.size() );
        e.memory() = dump_before;

        another_test_emu( e );

        write_binary_file( path + "\\block_ 0_dump_after_NEW.bin", e.memory().data(), e.memory().size() );
    }
}
