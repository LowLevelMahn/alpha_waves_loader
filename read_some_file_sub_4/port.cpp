#include "emu.hpp"
#include "types.hpp"
#include "helper.hpp"

namespace port
{
    std::vector<uint8_t> extract_executable( const std::string& game_dir_,
                                             config_tat_t::gfx_infos_t& gfx_infos_,
                                             const size_t gfx_nr_,
                                             const size_t exec_nr_ )
    {
        constexpr size_t executable_buffer_size = 0x9D160;
        std::vector<uint8_t> executable_buffer( executable_buffer_size - 0x100 );

        const std::string progs_cc1_filepath = game_dir_ + "\\progs.cc1";
        std::vector<uint8_t> progs_cc1_content = read_binary_file( progs_cc1_filepath );

        const auto& blocks = gfx_infos_[gfx_nr_];
        const auto& block = blocks.executable_info[exec_nr_];

        return executable_buffer;
    }

    bool read_some_file_sub_4( config_tat_t::executable_info_t* block_, uint8_t* executable_buffer_ )
    {
        uint16_t di = 0;

        // offsets in executable_buffer_
        int word_44_45 = 0;
        int also_a_pointer = 0;
        int some_game_ptr = 0;
        int another_far_ptr = 0;

        //// mov ax, cs
        //// mov ds, ax
        //// mov dx, bx; bx = gfx - block start = offset filename
        //// mov ah, 3Dh
        //// mov al, 0
        //// int 21h; DOS - 2 + -OPEN DISK FILE WITH HANDLE
        //// ; DS:DX->ASCIZ filename
        //// ; AL = access mode
        //// ; 0 - read
        //// jnb short loc_578

        const std::string root = R"(F:\projects\fun\dos_games_rev\alpha_waves_dev\tests\alpha)";
        const std::string filename = reinterpret_cast<char*>( block_->filename.data() );
        const std::string filepath = root + "\\" + filename;

        //std::vector<uint8_t> content = read_file(filepath.c_str());

        FILE* fp = fopen( filepath.c_str(), "rb" );
        if( !fp )
        {
            //// loc_136 : ; CODE XREF : read_some_file_sub_4 + 45j
            //// ; read_some_file_sub_4 + 4Aj ...
            //// stc
            //// retn

            return false;
        }

        //// ; -------------------------------------------------------------------------- -
        ////
        //// loc_578:; CODE XREF : read_some_file_sub_4 + Cj
        //// mov si, bx
        //// mov bx, ax
        //// mov ah, 3Fh; read file
        //// mov cl, cs:byte_55
        //// test  cl, 18h
        //// jnz short loc_579

        if( ( block_->byte_13h & 0x18 ) != 0 )
            goto loc_579;

        //// lds cx, dword ptr cs : executable_buffer.ofs
        //// mov cs:word_44, 0FFFFh
        //// mov cs:word_45, 0FFFFh
        //// jmp loc_580
        word_44_45 = -1;
        goto loc_580;

    loc_579:
        //// test  cl, 10h
        //// jz  short loc_581
        if( ( block_->byte_13h & 0x10 ) == 0 )
            goto loc_581;

        //// mov cx, 2
        //// lds dx, dword ptr cs : executable_buffer.ofs
        //// int 21h; DOS - 2 + -READ FROM FILE WITH HANDLE
        //// ; BX = file handle, CX = number of bytes to read
        //// ; DS:DX->buffer
        //// cmp  ax, 2
        //// jnz short loc_136
        ////

        {
            size_t read_bytes = fread( executable_buffer_, 1, 2, fp );
            assert( read_bytes == 2 );
            printf( "%s\n", hexdump( executable_buffer_, 2, 2 ).c_str() );
        }

        //// mov di, dx
        //// mov cx, [di]
        //// xchg  cl, ch
        //// mov di, cx
        //// mov al, cs:[si + gfx_block_t.byte_12h]
        //// xor ah, ah
        //// shl ax, 1
        //// shl ax, 1
        //// mov dx, ax
        //// xor cx, cx
        //// mov al, 1
        //// mov ah, 42h
        //// int 21h; DOS - 2 + -MOVE FILE READ / WRITE POINTER(LSEEK)
        //// ; AL = method: offset from present location
        //// jb  short loc_136

        {
            di = swap( *(uint16_t*)executable_buffer_ );

            const int next_pos = block_->byte_12h * 4;

            int res = fseek( fp, next_pos, SEEK_CUR );
            assert( res == 0 );
        }

        //// lds dx, dword ptr cs : executable_buffer.ofs
        //// mov ah, 3Fh
        //// mov cx, 4
        //// int 21h; DOS - 2 + -READ FROM FILE WITH HANDLE
        //// ; BX = file handle, CX = number of bytes to read
        //// ; DS:DX->buffer
        //// jb  short loc_136
        //// cmp  ax, 4
        //// jnz short loc_136

        {
            size_t read_bytes = fread( executable_buffer_, 1, 4, fp );
            assert( read_bytes == 4 );
            printf( "%s\n", hexdump( executable_buffer_, 4, 4 ).c_str() );
        }

        //// mov si, dx
        //// mov cx, [si]
        //// xchg  cl, ch
        //// mov dx, [si + 2]
        //// xchg  dl, dh
        //// shl di, 1
        //// shl di, 1
        //// add di, 2
        //// add dx, di
        //// adc cx, 0
        // mov al, 0
        // mov ah, 42h
        // int 21h; DOS - 2 + -MOVE FILE READ / WRITE POINTER(LSEEK)
        // ; AL = method: offset from beginning of file
        // jb  short loc_139

        {
            const size_t ofs = swap( *(uint32_t*)executable_buffer_ );

            const int next_pos = ofs + ( ( di * 4 ) + 2 );

            int res = fseek( fp, next_pos, SEEK_SET );
            assert( res == 0 );
        }

    loc_581:
        //// lds dx, dword ptr cs : executable_buffer.ofs
        //// mov ah, 3Fh
        //// mov cx, 8
        //// int 21h; DOS - 2 + -READ FROM FILE WITH HANDLE
        //// ; BX = file handle, CX = number of bytes to read
        //// ; DS:DX->buffer
        //// jb  short loc_139
        //// cmp ax, 8
        //// jz  short loc_582

        {
            size_t read_bytes = fread( executable_buffer_, 1, 8, fp );
            assert( read_bytes == 8 );
            printf( "%s\n", hexdump( executable_buffer_, 8, 8 ).c_str() );
            goto loc_582;
        }

        // error is handled before
        //loc_139:
        //	//// stc
        //	//// retn
        //	return false;

    loc_582:
        //// lds bp, dword ptr cs : executable_buffer.ofs
        //// mov ax, ds:[bp + 0]
        //// mov cx, ds:[bp + 2]
        //// xchg  al, ah
        //// xchg  cl, ch
        //// mov cs:word_44, cx
        //// mov cs:word_45, ax
        //// mov ax, ds:[bp + 4]
        //// mov cx, ds:[bp + 6]
        //// xchg  al, ah
        //// xchg  cl, ch
        //// mov cs:some_game_ptr.ofs, cx
        //// mov cs:some_game_ptr.segm, ax

        {
            const auto vals32 = (uint32_t*)executable_buffer_;

            word_44_45 = swap( vals32[0] );
            some_game_ptr = swap( vals32[1] );
        }

        // normalize pointer (not needed for 32bit)
        //// mov si, cx ; lo(some_game_ptr)
        //// mov di, ax ; hi(some_game_ptr)
        //// lds cx, dword ptr cs:executable_buffer.ofs
        //// add si, cx ; lo
        //// adc di, 0
        //// sub si, cs:word_44 ; lo(word_44_45)
        //// sbb di, cs:word_45 ; hi(word_44_45)
        //// add si, 10h
        //// adc di, 0
        //// mov cx, si
        //// shr di, 1
        //// rcr si, 1
        //// shr di, 1
        //// rcr si, 1
        //// shr di, 1
        //// rcr si, 1
        //// shr di, 1
        //// rcr si, 1
        //// mov ax, ds
        //// add ax, si
        //// mov ds, ax
        //// and cx, 0Fh

    loc_580:
        // mov cs:another_far_ptr.ofs, cx
        // mov cs:another_far_ptr.segm, ds
        // mov cs:another_pointer2.ofs, cx
        // mov cs:another_pointer2.segm, ds
        // mov si, cs:word_44
        // mov di, cs:word_45

        //// mov cs:also_a_pointer.ofs, 0
        //// mov cs:also_a_pointer.segm, 0
        also_a_pointer = 0;

        //lds dx, dword ptr cs:another_far_ptr.ofs

        //loc_586:
        // INFO: split in max 48000 byte parts (due to DOS API constraints)

        //// mov cx, dx
        //// shr cx, 1
        //// shr cx, 1
        //// shr cx, 1
        //// shr cx, 1
        //// mov ax, ds
        //// add ax, cx
        //// mov ds, ax
        //// and dx, 0Fh
        //// mov ax, 48000
        //// sub si, ax
        //// sbb di, 0
        //// jnb short loc_583
        //// add si, ax
        //// mov ax, si
        //// xor si, si
        //// xor di, di

    loc_583:
        //// mov cx, ax
        //// mov ah, 3Fh
        //// int 21h; DOS - 2 + -READ FROM FILE WITH HANDLE
        //// ; BX = file handle, CX = number of bytes to read
        //// ; DS:DX->buffer
        //// jnb short loc_584
        //// retn

        {
            const int ofs = some_game_ptr - word_44_45 + 16;
            assert( ofs >= 0 );
            uint8_t* buffer = executable_buffer_ + ofs;

            size_t read_bytes = fread( buffer, 1, word_44_45, fp );
            assert( read_bytes == word_44_45 );
            printf( "%s\n", hexdump( buffer, word_44_45, 5 * 16 ).c_str() );
        }

        // read next part after 48000 bytes (not needed in 32bit)
        ////loc_584:
        //// add cs:also_a_pointer.ofs, ax
        //// adc cs:also_a_pointer.segm, 0
        //// add dx, ax
        //// cmp ax, cx
        //// jnz short loc_585
        //// mov ax, si
        //// or ax, di
        //// jnz short loc_586

    loc_585:
        //// mov ah, 3Eh
        //// int 21h; DOS - 2 + -CLOSE A FILE WITH HANDLE
        //// ; BX = file handle
        int res = fclose( fp );
        assert( res == 0 );

        //// test cs:byte_55, 18h
        //// jz  short loc_587
        if( ( block_->byte_13h & 0x18 ) == 0 )
            goto loc_587;

        //// les di, dword ptr cs : executable_buffer.ofs; maybe_dest_ofs1_@
        //// push  es
        //// xor ax, ax
        //// mov es, ax
        //// assume es : nothing
        //// mov es:4, di
        //// pop es; maybe_dest_seg_@
        //// assume es:nothing

        // interrupt[1].ofs = di ???

        //// mov cs:another_far_ptr.ofs, di
        //// mov cs:another_far_ptr.segm, es
        another_far_ptr = 0; // executable_buffer_ + 0;

        //// lds si, dword ptr cs:some_game_ptr.ofs
        //// mov cx, ds
        //// add si, 10h
        //// add cx, 0
        //// add si, di
        //// adc cx, 0
        //// mov bx, si
        //// shr cx, 1
        //// rcr si, 1
        //// shr cx, 1
        //// rcr si, 1
        //// shr cx, 1
        //// rcr si, 1
        //// shr cx, 1
        //// rcr si, 1; maybe_src_ofs_@
        //// mov ax, es
        //// add ax, si
        //// mov cs:executable_buffer.segm, ax ; <---
        //// inc ax
        //// mov ds, ax; maybe_src_seg_@
        //// and bx, 0Fh
        //// mov cs:executable_buffer.ofs, bx ; <----

        executable_buffer_ += some_game_ptr + 16;

        // cld
        // call GAME_START_sub_3

        //// clc
        //// retn
        return true;

    loc_587:
        //// lds si, dword ptr cs:also_a_pointer.ofs
        //// mov cs:some_game_ptr.ofs, si
        //// mov cs:some_game_ptr.segm, ds
        some_game_ptr = also_a_pointer;

        //// clc
        //// retn
        return true;
    }

} // namespace port
