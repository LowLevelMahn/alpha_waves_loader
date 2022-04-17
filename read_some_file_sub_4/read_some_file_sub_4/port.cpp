#include "emu.hpp"
#include "types.hpp"
#include "helper.hpp"

void emu_GAME_START_sub_3(emu_t& e, emu_t::ptr16_t another_pointer2, const slice_t& executable_buffer_slice_)
{
	std::array<uint8_t, 4> byte_57{}; // gets written
	uint8_t& byte_569 = byte_57[1];
	uint16_t& word_60 = *reinterpret_cast<uint16_t*>(&byte_57[2]);

start:
	e.push(e.es);
	e.push(e.di);

	e.cx = 128;
	e.ax = e.ds;
	e.es = e.ax;
	e.di = 0x301;
	e.xor_w(e.ax, e.ax);
	e.rep_stosw();

	e.pop(e.di);
	e.pop(e.es);

	e.push(e.es);

	e.xor_w(e.ax, e.ax);
	e.es = e.ax;
	e.sub(e.di, *e.memory<uint16_t>(e.es, 4));

	e.pop(e.es);

	e.ax = e.di;
	e.shr(e.ax, 1);
	e.shr(e.ax, 1);
	e.shr(e.ax, 1);
	e.shr(e.ax, 1);
	e.cx = e.es;
	e.add(e.cx, e.ax);
	e.es = e.cx;
	e.and_w(e.di, 0x0F);

	e.push(e.es);

	e.xor_w(e.ax, e.ax);
	e.es = e.ax;
	e.add(e.di, *e.memory<uint16_t>(e.es, 4));

	e.pop(e.es);

	e.push(e.ds);
	e.push(e.es);
	e.push(e.si);
	e.push(e.di);

	//--- rep movsb prepare
	e.cx = 4;
	e.di = 0x152; // offset byte_57; NOT NEEDED
	e.ax = e.cs;
	e.es = e.ax;
	//---
	e.lds(e.si, another_pointer2);
	e.ax = e.si;
	e.shr(e.ax, 1);
	e.shr(e.ax, 1);
	e.shr(e.ax, 1);
	e.shr(e.ax, 1);
	e.dx = e.ds;
	e.add(e.ax, e.dx);
	e.ds = e.ax;
	e.and_w(e.si, 0x0F);
	another_pointer2.offset = e.si;
	another_pointer2.segment = e.ds;
	e.add(another_pointer2.offset, e.cx);
#if 0
	e.rep_movsb(); // 4 bytes from DS:SI => ES:DI
#else
	//e.rep_movsb_dest(byte_57); // dest is native
	//e.rep_movsb_src(byte_57); // src is native
	::memcpy(&byte_57, e.memory(e.ds, e.si), 4);
	e.si += 4;
	e.di += 4;
	e.cx = 0;
#endif
	e.pop(e.di);
	e.pop(e.si);
	e.pop(e.es);
	e.pop(e.ds);

	e.dx = word_60;
	e.inc(e.dx);
	e.cmp(byte_57[0], 0);
	if (e.jnz()) goto loc_565;
	goto loc_566;
loc_565:
	e.push(e.ds);
	e.push(e.es);
	e.push(e.di);

	e.xor_b(e.ch, e.ch);
	e.cl = byte_57[0];
	e.di = 0x201;
	e.ax = e.ds;
	e.es = e.ax;
	e.ds = another_pointer2.segment;
	e.si = another_pointer2.offset;
	e.add(another_pointer2.offset, e.cx);
	e.rep_movsb();
	e.cl = byte_57[0];
	e.xor_b(e.ch, e.ch);
	e.di = 1;
	e.add(another_pointer2.offset, e.cx);
	e.rep_movsb();
	e.cl = byte_57[0];
	e.di = 0x101;
	e.add(another_pointer2.offset, e.cx);
	e.rep_movsb();

	e.pop(e.di);
	e.pop(e.es);
	e.pop(e.ds);

	e.xor_b(e.ch, e.ch);
	e.cl = byte_57[0];
	e.xor_b(e.ah, e.ah);
	e.bx = 1;

loc_567:
	e.al = *e.memory<uint8_t>(e.ds, e.bx + 0x200);
	e.si = e.ax;
	e.dl = *e.memory<uint8_t>(e.ds, e.si + 0x301);
	*e.memory<uint8_t>(e.ds, e.bx + 0x402) = e.dl;
	*e.memory<uint8_t>(e.ds, e.si + 0x301) = e.bl;
	e.inc(e.bx);
	if (e.loop()) goto  loc_567;
	e.dx = word_60;
	e.inc(e.dx);
	e.cx = 1;

loc_124:
	e.dec(e.dx);
	if (e.jnz()) goto loc_568;

loc_577:
	e.cmp(byte_569, 0);
	if (e.jz()) goto  locret_570;
	goto start;

locret_570:
	return;

loc_568:
	e.push(e.ds);
	e.si = another_pointer2.segment;
	e.ds = e.si;
	e.si = another_pointer2.offset;
	e.lodsb();
	another_pointer2.offset = e.si;
	e.pop(e.ds);
	e.bx = e.ax;
	e.cmp(*e.memory<uint8_t>(e.ds, e.bx + 0x301), 0);
	if (e.jnz()) goto  loc_571;
	e.stosb();
	goto  loc_124;

loc_571:
	e.bl = *e.memory<uint8_t>(e.ds, e.bx + 0x301);
	e.xor_w(e.ax, e.ax);
	e.push(e.ax);
	goto loc_128;

loc_129:
	e.bp = e.ax;
	e.cmp(*e.memory<uint8_t>(e.ds, e.bp + 0x301), 0);
	if (e.jz()) goto loc_572;
	e.cmp(e.bl, *e.memory<uint8_t>(e.ds, e.bp + 0x301));
	if (e.ja()) goto loc_573;
	e.al = e.bl;
	e.bl = *e.memory<uint8_t>(e.ds, e.bp + 0x301);

loc_575:
	e.bl = *e.memory<uint8_t>(e.ds, e.bx + 0x402);
	e.or_b(e.bl, e.bl);
	if (e.jz()) goto loc_574;
	e.cmp(e.bl, e.al);
	if (e.jb()) goto loc_128;
	goto loc_575;

loc_573:
	e.bl = *e.memory<uint8_t>(e.ds, e.bp + 0x301);

loc_128:
	e.al = *e.memory<uint8_t>(e.ds, e.bx + 0x100);
	e.ah = e.bl;
	e.push(e.ax);
	e.xor_b(e.ah, e.ah);
	e.al = *e.memory<uint8_t>(e.ds, e.bx);
	goto loc_129;

loc_574:
	e.ax = e.bp;
loc_572:
	e.stosb();
	e.pop(e.ax);
	e.or_w(e.ax, e.ax);
	if (e.jnz()) goto loc_576;
	goto  loc_124;
loc_576:
	e.bl = e.ah;
	e.xor_b(e.ah, e.ah);
	goto  loc_129;

loc_566:
	e.push(e.ds);
	e.push(e.es);
	e.cx = word_60;
	e.push(e.cx);
	e.ds = another_pointer2.segment;
	e.si = another_pointer2.offset;
	e.add(another_pointer2.offset, e.cx);
	e.rep_movsb();
	e.pop(e.cx);
	e.pop(e.es);
	e.pop(e.ds);
	goto loc_577;
}

void emu_read_some_file_sub_4(emu_t& e, const uint8_t byte_55_, emu_t::ptr16_t& executable_buffer_, const slice_t& executable_buffer_slice_)
{
	uint16_t word_44{};
	uint16_t word_45{};

	// these are lo/hi int32 offsets (the pair looked liked pointers)
	emu_t::ptr16_t some_game_ptr;
	emu_t::ptr16_t another_far_ptr;
	emu_t::ptr16_t another_pointer2;
	emu_t::ptr16_t also_a_pointer;

	e.ax = e.cs;
	e.ds = e.ax;
	e.dx = e.bx; // bx = gfx-block start = offset filename
	char* filename = e.memory<char>(e.ds, e.dx);
	e.ah = 0x3D;
	e.al = 0;
	e.intr_0x21();
	// DOS - 2 + -OPEN DISK FILE WITH HANDLE
	// DS:DX->ASCIZ filename
	// AL = access mode
	//  0 - read
	if (e.jnb()) goto loc_578;

loc_136:
	e.stc();
	return;

loc_578:
	e.si = e.bx;
	e.bx = e.ax;
	e.ah = 0x3F;
	e.cl = byte_55_;
	e.test(e.cl, 0x18);
	if (e.jnz()) goto loc_579;
	e.lds(e.cx, executable_buffer_);
	word_44 = 0xFFFF;
	word_45 = 0xFFFF;
	goto loc_580;

loc_579:
	e.test(e.cl, 0x10);
	if (e.jz()) goto loc_581;
	e.cx = 2;
	e.lds(e.dx, executable_buffer_);
	e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
					// BX = file handle, CX = number of bytes to read
					// DS:DX->buffer
	if (e.jb()) goto loc_136;
	e.cmp(e.ax, 2);

	if (e.jnz()) goto loc_136;
	e.di = e.dx;
	e.cx = *e.memory<uint16_t>(e.ds, e.di);
	e.xchg(e.cl, e.ch);
	e.di = e.cx;
	e.al = e.memory<gfx_block_t>(e.cs, e.si)->byte_12h;
	e.xor_b(e.ah, e.ah);
	e.shl(e.ax, 1);
	e.shl(e.ax, 1);
	e.dx = e.ax;
	e.xor_w(e.cx, e.cx);
	e.al = 1;
	e.ah = 0x42;
	e.intr_0x21(); // DOS - 2 + -MOVE FILE READ / WRITE POINTER(LSEEK)
					// AL = method: offset from present location
	if (e.jb()) goto loc_136;

	e.lds(e.dx, executable_buffer_);
	e.ah = 0x3F;
	e.cx = 4;
	e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
					// BX = file handle, CX = number of bytes to read
					// DS:DX->buffer
	if (e.jb()) goto loc_136;
	e.cmp(e.ax, 4);
	if (e.jnz()) goto loc_136;

	e.si = e.dx;
	e.cx = *e.memory<uint16_t>(e.ds, e.si);
	e.xchg(e.cl, e.ch);
	e.dx = *e.memory<uint16_t>(e.ds, e.si + 2);
	e.xchg(e.dl, e.dh);
	e.shl(e.di, 1);
	e.shl(e.di, 1);
	e.add(e.di, 2);
	e.add(e.dx, e.di);
	e.adc(e.cx, 0);
	e.al = 0;
	e.ah = 0x42;
	e.intr_0x21(); // DOS - 2 + -MOVE FILE READ / WRITE POINTER(LSEEK)
					// AL = method: offset from beginning of file
	if (e.jb()) goto loc_139;

loc_581:
	e.lds(e.dx, executable_buffer_);
	e.ah = 0x3F;
	e.cx = 8;
	e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
		// BX = file handle, CX = number of bytes to read
		// DS:DX->buffer
	if (e.jb()) goto loc_139;
	e.cmp(e.ax, 8);

	if (e.jz()) goto loc_582;

loc_139:
	e.stc();
	return;

loc_582:
	e.lds(e.bp, executable_buffer_);
	e.ax = *e.memory<uint16_t>(e.ds, e.bp + 0);
	e.cx = *e.memory<uint16_t>(e.ds, e.bp + 2);
	e.xchg(e.al, e.ah);
	e.xchg(e.cl, e.ch);
	word_44 = e.cx;
	word_45 = e.ax;
	e.ax = *e.memory<uint16_t>(e.ds, e.bp + 4);
	e.cx = *e.memory<uint16_t>(e.ds, e.bp + 6);
	e.xchg(e.al, e.ah);
	e.xchg(e.cl, e.ch);
	some_game_ptr.offset = e.cx;
	some_game_ptr.segment = e.ax;
	e.si = e.cx;
	e.di = e.ax;
	e.lds(e.cx, executable_buffer_);
	e.add(e.si, e.cx);
	e.adc(e.di, 0);
	e.sub(e.si, word_44);
	e.sbb(e.di, word_45);
	e.add(e.si, 0x10);
	e.adc(e.di, 0);
	e.cx = e.si;
	e.shr(e.di, 1);
	e.rcr(e.si, 1);
	e.shr(e.di, 1);
	e.rcr(e.si, 1);
	e.shr(e.di, 1);
	e.rcr(e.si, 1);
	e.shr(e.di, 1);
	e.rcr(e.si, 1);
	e.ax = e.ds;
	e.add(e.ax, e.si);
	e.ds = e.ax;
	e.and_w(e.cx, 0x0F);

loc_580:
	another_far_ptr.offset = e.cx;
	another_far_ptr.segment = e.ds;
	another_pointer2.offset = e.cx;
	another_pointer2.segment = e.ds;
	e.si = word_44;
	e.di = word_45;
	also_a_pointer.offset = 0;
	also_a_pointer.segment = 0;
	e.lds(e.dx, another_far_ptr);

loc_586:
	e.cx = e.dx;
	e.shr(e.cx, 1);
	e.shr(e.cx, 1);
	e.shr(e.cx, 1);
	e.shr(e.cx, 1);
	e.ax = e.ds;
	e.add(e.ax, e.cx);
	e.ds = e.ax;
	e.and_w(e.dx, 0x0F);
	e.ax = 48000;
	e.sub(e.si, e.ax);
	e.sbb(e.di, 0);
	if (e.jnb()) goto loc_583;
	e.add(e.si, e.ax);
	e.ax = e.si;
	e.xor_w(e.si, e.si);
	e.xor_w(e.di, e.di);

loc_583:
	e.cx = e.ax;
	e.ah = 0x3F;
	e.intr_0x21(); // DOS - 2 + -READ FROM FILE WITH HANDLE
	// BX = file handle, CX = number of bytes to read
	// DS:DX->buffer
	if (e.jnb()) goto loc_584;
	return;

loc_584:
	e.add(also_a_pointer.offset, e.ax);
	e.adc(also_a_pointer.segment, 0);
	e.add(e.dx, e.ax);
	e.cmp(e.ax, e.cx);
	if (e.jnz()) goto loc_585;
	e.ax = e.si;
	e.or_w(e.ax, e.di);
	if (e.jnz()) goto loc_586;

loc_585:
	e.ah = 0x3E;
	e.intr_0x21(); // DOS - 2 + -CLOSE A FILE WITH HANDLE
	// BX = file handle
	e.test(byte_55_, 0x18);
	if (e.jz()) goto loc_587;
	e.les(e.di, executable_buffer_);
	e.push(e.es);
	e.xor_w(e.ax, e.ax);
	e.es = e.ax;
	*e.memory<uint16_t>(e.es, 4) = e.di;
	e.pop(e.es);
	another_far_ptr.offset = e.di;
	another_far_ptr.segment = e.es;
	e.lds(e.si, some_game_ptr);
	e.cx = e.ds;
	e.add(e.si, 0x10);
	e.add(e.cx, 0);
	e.add(e.si, e.di);
	e.adc(e.cx, 0);
	e.bx = e.si;
	e.shr(e.cx, 1);
	e.rcr(e.si, 1);
	e.shr(e.cx, 1);
	e.rcr(e.si, 1);
	e.shr(e.cx, 1);
	e.rcr(e.si, 1);
	e.shr(e.cx, 1);
	e.rcr(e.si, 1);
	e.ax = e.es;
	e.add(e.ax, e.si);
	executable_buffer_.segment = e.ax;
	e.inc(e.ax);
	e.ds = e.ax;
	e.and_w(e.bx, 0x0F);
	executable_buffer_.offset = e.bx;
	e.cld();

	//write_binary_file("d:/temp/out.before_game_sub_3_call.BIN", executable_buffer_slice_.data, executable_buffer_slice_.size);

	emu_GAME_START_sub_3(e, another_pointer2, executable_buffer_slice_);

	//write_binary_file("d:/temp/out.after_game_sub_3_call.BIN", executable_buffer_slice_.data, executable_buffer_slice_.size);

	e.clc();
	return;

loc_587:
	e.lds(e.si, also_a_pointer.offset);
	some_game_ptr.offset = e.si;
	some_game_ptr.segment = e.ds;
	e.clc();
	return;
}

#if 0
bool read_some_file_sub_4(gfx_block_t* block_, uint8_t* executable_buffer_)
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
	const std::string filename = (const char*)block_->filename;
	const std::string filepath = root + "\\" + filename;

	//std::vector<uint8_t> content = read_file(filepath.c_str());

	FILE* fp = fopen(filepath.c_str(), "rb");
	if (!fp)
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

	if ((block_->byte_13h & 0x18) != 0) goto loc_579;

	//// lds cx, dword ptr cs : executable_buffer.ofs
	//// mov cs:word_44, 0FFFFh
	//// mov cs:word_45, 0FFFFh
	//// jmp loc_580
	word_44_45 = -1;
	goto loc_580;

loc_579:
	//// test  cl, 10h
	//// jz  short loc_581
	if ((block_->byte_13h & 0x10) == 0) goto loc_581;

	//// mov cx, 2
	//// lds dx, dword ptr cs : executable_buffer.ofs
	//// int 21h; DOS - 2 + -READ FROM FILE WITH HANDLE
	//// ; BX = file handle, CX = number of bytes to read
	//// ; DS:DX->buffer
	//// cmp  ax, 2
	//// jnz short loc_136
	//// 

	{
		size_t read_bytes = fread(executable_buffer_, 1, 2, fp);
		assert(read_bytes == 2);
		printf("%s\n", hexdump(executable_buffer_, 2, 2).c_str());
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
		di = swap(*(uint16_t*)executable_buffer_);

		const int next_pos = block_->byte_12h * 4;

		int res = fseek(fp, next_pos, SEEK_CUR);
		assert(res == 0);
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
		size_t read_bytes = fread(executable_buffer_, 1, 4, fp);
		assert(read_bytes == 4);
		printf("%s\n", hexdump(executable_buffer_, 4, 4).c_str());
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
		const size_t ofs = swap(*(uint32_t*)executable_buffer_);

		const int next_pos = ofs + ((di * 4) + 2);

		int res = fseek(fp, next_pos, SEEK_SET);
		assert(res == 0);
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
		size_t read_bytes = fread(executable_buffer_, 1, 8, fp);
		assert(read_bytes == 8);
		printf("%s\n", hexdump(executable_buffer_, 8, 8).c_str());
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

		word_44_45 = swap(vals32[0]);
		some_game_ptr = swap(vals32[1]);
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
		assert(ofs >= 0);
		uint8_t* buffer = executable_buffer_ + ofs;

		size_t read_bytes = fread(buffer, 1, word_44_45, fp);
		assert(read_bytes == word_44_45);
		printf("%s\n", hexdump(buffer, word_44_45, 5 * 16).c_str());
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
	int res = fclose(fp);
	assert(res == 0);

	//// test cs:byte_55, 18h
	//// jz  short loc_587
	if ((block_->byte_13h & 0x18) == 0) goto loc_587;

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
#endif