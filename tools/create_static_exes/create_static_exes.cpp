#include <string>
#include <cstdint>
#include <vector>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <cstring>

bool read_binary_file(const std::string& filename_, std::vector<uint8_t>& content_)
{
	std::ifstream file(filename_, std::ios::binary);
	if (!file.is_open())
	{
		return false;
	}
	content_ = std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return true;
}

bool write_binary_file(const std::vector<uint8_t>& content_, const std::string& filename_)
{
	std::ofstream file(filename_, std::ios::binary);
	if (!file.is_open())
	{
		return false;
	}
	file.write(reinterpret_cast<const char*>(content_.data()), content_.size());
	return true;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("invalid parameter\n");
		printf("  create_static_exes [path extracted exes]\n");
		return 1;
	}

	const std::string exe_path = argv[1];

	const std::string in_path = exe_path + "/ega_vga.exe";

	bool output_vga = true;

	const std::string out_path = exe_path + "/" + (output_vga ? "vga.exe" : "ega.exe");

	//read file complete
	std::vector<uint8_t> content;
	if (!read_binary_file(in_path, content))
	{
		return 2;
	}

	if (content.size() != 89900)
	{
		printf("ega_vga.exe is of wrong size\n");
		return 3;
	}

	auto patch = [&content](size_t offset_, const std::vector<uint8_t> org_, const std::vector<uint8_t> new_)
	{
		if (::memcmp(org_.data(), &content[offset_], org_.size()) != 0)
		{
			return false;
		}
		::memcpy(&content[offset_], new_.data(), new_.size());
		return true;
	};

	auto patch_nops = [&content](size_t offset_, const std::vector<uint8_t> org_)
	{
		if (::memcmp(org_.data(), &content[offset_], org_.size()) != 0)
		{
			return false;
		}
		::memset(&content[offset_], 0x90, org_.size());
		return true;
	};
	
		auto nop_out = [&content](size_t begin_, size_t end_)
		{
			size_t size = end_ - begin_;
			::memset(&content[begin_], 0x90, size);
		}; 

		auto nop_overwrite = [&content](size_t begin_, size_t end_, const std::vector<uint8_t>& new_code_)
		{
			size_t size = end_ - begin_;
			assert(new_code_.size() <= size);

			::memset(&content[begin_], 0x90, size);
			::memcpy(&content[begin_], new_code_.data(), new_code_.size());
		};  

	{
		// remove interrupt 97h dependency for feature_flag

		// int 97h gets called in ega_vga.exe from file offset: 0x57DA

		//found with dosbox debugger bpint plus some hex-editor searching

		// replaces this code...
		//     CD 97 : int 97h
		//     8B D8 : mov bx,ax
		// ...with this
		//     B8 01 C0 : mov ax,0xC001  
		//     90       : nop

		if (!patch(0x57DA, { 0xCD, 0x97, 0x8B, 0xD8 }, { 0xB8, 0x01, 0xC0, 0x90 }))
		{
			printf("wrong original code for interrupt 97h call\n");
			return 4;
		}
	}

	{
		// remove interrupt 0x9F.offset dependency for gfx_mode
		// used in function at file offset: 0x2B1B
		// 
		// replace this code...
		// seg000:291B                         get_gfx_mode_sub_1291B proc near        ; CODE XREF: _main+18p
		//   06                                      push    es
		//   1E                                      push    ds
		//   56                                      push    si
		//   57                                      push    di
		//   33 C0                                   xor     ax, ax
		//   8E D8                                   mov     ds, ax
		//                                           assume ds:nothing
		//   A1 7C 02                                mov     ax, ds:27Ch     ; gfx_mode
		//   3D 04 00                                cmp     ax, 4           ; == 4 == VGA
		//   74 07                                   jz      short is_vga
		//   33 C0                                   xor     ax, ax          ; return 0
		//   5F                                      pop     di
		//   5E                                      pop     si
		//   1F                                      pop     ds
		//                                           assume ds:dseg
		//   07                                      pop     es
		//   C3                                      retn
		//                           ; ---------------------------------------------------------------------------
		//   
		//                           is_vga:                                 ; CODE XREF: get_gfx_mode_sub_1291B+Ej
		//   B8 FF FF                                mov     ax, 0FFFFh      ; return -1
		//   5F                                      pop     di
		//   5E                                      pop     si
		//   1F                                      pop     ds
		//   07                                      pop     es
		//   C3                                      retn
		//                               get_gfx_mode_sub_1291B endp

		//found in disassembly searching for 27Ch (= 0x9F*4) access
		const std::vector<uint8_t> org_code
		{
		  0x06, 0x1E, 0x56, 0x57, 0x33, 0xC0, 0x8E, 0xD8,
		  0xA1, 0x7C, 0x02, 0x3D, 0x04, 0x00, 0x74, 0x07,
		  0x33, 0xC0, 0x5F, 0x5E, 0x1F, 0x07, 0xC3, 0xB8,
		  0xFF, 0xFF, 0x5F, 0x5E, 0x1F, 0x07, 0xC3
		};

		if (::memcmp(org_code.data(), &content[0x2B1B], org_code.size()) != 0)
		{
			printf("wrong original code for interrupt 9Fh offset use\n");
			return 5;
		}

		// ...with that
		// NOP the complete function
		::memset(&content[0x2B1B], 0x90, org_code.size());

		// the only code difference between ega and vga
		std::vector<uint8_t> new_code;
		if (output_vga)
		{
			// VGA
			new_code = { 0xB8, 0xFF, 0xFF, 0xC3 };
			//     B8 FF FF : mov ax, 0FFFFh
			//     C3       : retn
		}
		else
		{
			// EGA
			new_code = { 0x33, 0xC0, 0xC3 };
			//     33 C0    : xor ax, ax
			//     C3       : retn
		}
		::memcpy(&content[0x2B1B], new_code.data(), new_code.size());
	}

	// disable copy protection
	{
		/*
		seg000:288E                         code_question_two_sub_1288E proc near   ; CODE XREF: code_question_screen_sub_17709+188p
		seg000:288E
		seg000:288E                         code_answer_    = dword ptr  4
		seg000:288E
		seg000:288E 55                                      push    bp
		seg000:288F 8B EC                                   mov     bp, sp
		seg000:2891 06                                      push    es
		seg000:2892 1E                                      push    ds
		seg000:2893 56                                      push    si
		seg000:2894 57                                      push    di
		seg000:2895 C4 76 04                                les     si, [bp+code_answer_] ; input in code-question dialog:
		seg000:2895                                                                 ;
		seg000:2895                                                                 ; 4321 gives here bytes: 1F 1E 1D 1C
		seg000:2895                                                                 ; 0000 gives here bytes: 1B 1B 1B 1B
		seg000:2895                                                                 ; 9999 gives here bytes: 24 24 24 24
		seg000:2895                                                                 ;
		seg000:2895                                                                 ; 0-9 in question-dialog => 1B-24 here
		seg000:2898 BF 76 7D                                mov     di, offset code_answer_pairs ; 80 words
		seg000:289B
		seg000:289B                         loc_1289B:                              ; DATA XREF: code_question_screen_sub_1283E+1Dw
		seg000:289B B8 34 12                                mov     ax, 1234h       ; !!! code modified by code_question_screen_sub_1283E
		seg000:289B                                                                 ;
		seg000:289B                                                                 ; always range: 0..9
		seg000:289B                                                                 ;
		seg000:289B                                                                 ; mov ax,0..9
		seg000:289E D1 E0                                   shl     ax, 1           ; ax *= 16 ==> * TABLE_ROW_BYTE_SIZE == 8*2
		seg000:28A0 D1 E0                                   shl     ax, 1
		seg000:28A2 D1 E0                                   shl     ax, 1
		seg000:28A4 D1 E0                                   shl     ax, 1
		seg000:28A6
		seg000:28A6                         loc_128A6:                              ; DATA XREF: code_question_screen_sub_12865+1Dw
		seg000:28A6 BB 34 12                                mov     bx, 1234h       ; !!! code modified by code_question_screen_sub_12865
		seg000:28A6                                                                 ;
		seg000:28A6                                                                 ; always range: 0...8
		seg000:28A6                                                                 ;
		seg000:28A6                                                                 ; mov bx,0...8
		seg000:28A9 D1 E3                                   shl     bx, 1
		seg000:28AB 03 C3                                   add     ax, bx
		seg000:28AD 03 F8                                   add     di, ax
		seg000:28AF 33 D2                                   xor     dx, dx
		seg000:28B1 8B 0D                                   mov     cx, [di]        ; cx = code_answer_pairs[ax][bx]
		seg000:28B3 BF 43 2B                                mov     di, 2B43h       ; constant
		seg000:28B6 26 8A 04                                mov     al, es:[si]     ; code_answer_[0]
		seg000:28B9 2C 1B                                   sub     al, 1Bh         ; transform to 0-9
		seg000:28BB 8A E0                                   mov     ah, al
		seg000:28BD D0 E4                                   shl     ah, 1
		seg000:28BF D0 E4                                   shl     ah, 1
		seg000:28C1 02 E0                                   add     ah, al
		seg000:28C3 02 E4                                   add     ah, ah
		seg000:28C5 26 8A 44 01                             mov     al, es:[si+1]   ; code_answer_[1]
		seg000:28C9 2C 1B                                   sub     al, 1Bh         ; transform to 0-9
		seg000:28CB 02 E0                                   add     ah, al
		seg000:28CD BB D2 2A                                mov     bx, 2AD2h       ; constant
		seg000:28D0 42                                      inc     dx
		seg000:28D1 3A E1                                   cmp     ah, cl
		// !!!!!!!!!!! NOP this to disable first decimal pair check
		// file-offset: 0x2AD3
		seg000:28D3 75 0F                                   jnz     short loc_128E4
		seg000:28D5 BF D5 24                                mov     di, (offset loc_124D3+2) ; also used for code modification?
		seg000:28D8 D1 E0                                   shl     ax, 1
		seg000:28DA D0 E4                                   shl     ah, 1
		seg000:28DC 42                                      inc     dx
		seg000:28DD 02 E0                                   add     ah, al
		seg000:28DF 03 C7                                   add     ax, di
		seg000:28E1 83 C6 04                                add     si, 4           ; at the end
		seg000:28E4
		seg000:28E4                         loc_128E4:                              ; CODE XREF: code_question_two_sub_1288E+45
		seg000:28E4 26 8A 44 FE                             mov     al, es:[si-2]   ; code_answer_[2]
		seg000:28E8 2C 1B                                   sub     al, 1Bh         ; transform to 0-9
		seg000:28EA 8A E0                                   mov     ah, al
		seg000:28EC D0 E4                                   shl     ah, 1
		seg000:28EE D0 E4                                   shl     ah, 1
		seg000:28F0 02 E0                                   add     ah, al
		seg000:28F2 02 E4                                   add     ah, ah
		seg000:28F4 26 8A 44 FF                             mov     al, es:[si-1]   ; code_answer_[3]
		seg000:28F8 2C 1B                                   sub     al, 1Bh         ; transform to 0-9
		seg000:28FA 02 E0                                   add     ah, al
		seg000:28FC 42                                      inc     dx
		seg000:28FD BB D2 2A                                mov     bx, 2AD2h       ; constant, bx not in use, or used by caller?
		seg000:2900 3A E5                                   cmp     ah, ch

		 // !!!!!!!!!!! NOP this to disable second decimal pair check
		// file-offset: 0x2B02
		seg000:2902 75 0C                                   jnz     short loc_12910

		seg000:2904 BB B1 21                                mov     bx, (offset loc_121B0+1) ; modified place - here only read
		seg000:2907 2E 8B 05                                mov     ax, cs:[di]     ; prepare for modification - only read here
		seg000:290A D1 E8                                   shr     ax, 1
		seg000:290C 42                                      inc     dx
		seg000:290D 2E D1 27                                shl     word ptr cs:[bx], 1 ; modifies code at always fix point cs:21B1
		seg000:2910
		seg000:2910                         loc_12910:                              ; CODE XREF: code_question_two_sub_1288E+74
		seg000:2910 2E 01 05                                add     cs:[di], ax     ; modifies code around
		seg000:2913 8B C2                                   mov     ax, dx          ; code question result: dx = 2 = wrong, dx = 4 = correct
		seg000:2915 5F                                      pop     di
		seg000:2916 5E                                      pop     si
		seg000:2917 1F                                      pop     ds
		seg000:2918 07                                      pop     es
		seg000:2919 5D                                      pop     bp
		seg000:291A C3                                      retn
		seg000:291A                         code_question_two_sub_1288E endp
		*/

		::memset(&content[0x2AD3], 0x90, 2);
		::memset(&content[0x2B02], 0x90, 2);

		// nop-overwrite code_answer_pairs table with 80*2 NOPs - not used anymore
		::memset(&content[0x156E6], 0x90, 80 * 2);

		// TODO: remove runtime code modification completely

		//first steps

		//patch cs:loc_124D3+2 to be always {0x0C 0x00}
		if (!patch(0x26D5, { 0x80, 0x00 }, { 0x0C, 0x00 }))
		{
			printf("wrong original code at #1\n");
			return 5;
		}

		//patch cs:loc_121B0+1 to be always {0x14 0x00}
		  // file-offset: 23B1
		if (!patch(0x23B1, { 0x80, 0x00 }, { 0x14, 0x00 }))
		{
			printf("wrong original code at #2\n");
			return 5;
		}

		//nop-overwrite code_question_two_sub_1288E with
		// begin: seg000:288E (file-offset: 2A8E), end: seg000:291B (file-offset: 2B1B) => size = 0x8D(141 bytes)
		//  mov ax,4 ; success
		//  ret
		nop_overwrite(0x2A8E, 0x2B1B, { 0xB8, 0x04, 0x00, 0xC3 });

		//nop-overwrite code_question_screen_sub_1283E with
		// begin: seg000:283E (file-offset: 2A3E), end: seg000:2865 (file-offset: 2A65) => size = 39 bytes
		//  mov ax,1 ; 0+1
		//  ret
		nop_overwrite(0x2A3E, 0x2A65, { 0xB8, 0x01, 0x00, 0xC3 });

		//nop-overwrite code_question_screen_sub_12865 with
		// begin: seg000:2865 (file-offset: 2A65), end: seg000:288E (file-offset: 2A8E) => 
		//  mov ax,1Ch ; 0+1Ch
		//  ret
		nop_overwrite(0x2A65, 0x2A8E, { 0xB8, 0x1C, 0x00, 0xC3 });

		// disable code modification in 
		// seg000:2AD2                         code_modifiying_sub

		nop_out(0x2CD5, 0x2D11);

		if (!patch(0x2DBB, { 0x34, 0x12 }, { 0x0A, 0x00 }))
		{
			printf("wrong original code at #2\n");
			return 5;
		}

		if (!patch(0x2DC0, { 0x34, 0x12 }, { 0x35, 0x01 }))
		{
			printf("wrong original code at #2\n");
			return 5;
		}

		if (!patch(0x2D80, { 0x34, 0x12 }, { 0x9F, 0x00 }))
		{
			printf("wrong original code at #2\n");
			return 5;
		}

		if (!patch(0x2DC6, { 0x34, 0x12 }, { 0x38, 0x00 }))
		{
			printf("wrong original code at #2\n");
			return 5;
		}

		if (!patch(0x2DCC, { 0x34, 0x12 }, { 0x4F, 0x00 }))
		{
			printf("wrong original code at #2\n");
			return 5;
		}

		if (!patch(0x2D69, { 0x34, 0x12 }, { 0x43, 0x00 }))
		{
			printf("wrong original code at #2\n");
			return 5;
		}

		if (!patch(0x5239, { 0x55, 0x8B, 0xEC, 0x8B, 0x46, 0x04, 0x2E, 0xA2, 0xE5, 0x50, 0x5D, 0xC3}, { 0xC3 }))
		{
			printf("wrong original code at #2\n");
			return 5;
		}

#if 0
		// remove modification of modifies_code_5_sub_15022

		//!!!
		// !!!!! test with and without loadfix before - the following copy protection code is load segment related !!!!!
		//!!!
		//! 

		// the "correct" positions but the values are load segment releated

		if (!patch(0x5222, {0x55, 0x8B, 0xEC, 0x1E, 0xC5, 0x46, 0x04, 0x2D, 0x20, 0x00, 0x2E, 0xA3,	0xD3, 0x50, 0x8C, 0xD8, 0x2E, 0xA3, 0xCE, 0x50, 0x1F, 0x5D, 0xC3}, 
			{ 0xC3 }))
		{
			printf("wrong original code at #2\n");
			return 5;
		}

		// im patching the code so that the code modification will be replaced by global variable use

		//code segment is in dosbox currently from 01ED:0 of size 0xD770
		//memdumpbin on main, before code-question and after shows that the code
		//modification starts also before the question is asked

/*
seg000:50CD                         loc_150CD:                              ; DATA XREF: modifies_code_5_sub_15022+10w
seg000:50CD BA 34 12                                mov     dx, 1234h       ; segment address for later lodsb
seg000:50D0 8E DA                                   mov     ds, dx
seg000:50D2                                         assume ds:nothing
seg000:50D2
seg000:50D2                         loc_150D2:                              ; DATA XREF: modifies_code_5_sub_15022+Aw
seg000:50D2 BE 34 12                                mov     si, 1234h
seg000:50D5 BA CF 03                                mov     dx, 3CFh
seg000:50D8 D1 E0                                   shl     ax, 1
seg000:50DA D1 E0                                   shl     ax, 1
seg000:50DC D1 E0                                   shl     ax, 1
seg000:50DE D1 E0                                   shl     ax, 1
seg000:50E0 D1 E0                                   shl     ax, 1
seg000:50E2 03 F0                                   add     si, ax 
*/

		/*
0x0000000000000000:  BA 77 0D       mov dx, 0x0d77h ; seg dseg
0x0000000000000003:  8E DA          mov ds, dx
0x0000000000000005:  8B 16 68 8B    mov dx, word ptr [0x8b68] <-- global var with segment
0x0000000000000009:  8B 36 66 8B    mov si, word ptr [0x8b66] <-- global var with offset
0x000000000000000d:  8E DA          mov ds, dx
0x000000000000000f:  BA CF 03       mov dx, 0x3cf
0x0000000000000012:  C1 E0 05       shl ax, 5
0x0000000000000015:  01 C6          add si, ax
		*/

		nop_overwrite(0x52CD, 0x52E4, {
			0xBA, 0x77, 0x0D, 0x8E, 0xDA, 0x8B, 0x16, 0x68, 0x8B, 0x8B, 0x36, 0x66,
			0x8B, 0x8E, 0xDA, 0xBA, 0xCF, 0x03, 0xC1, 0xE0, 0x05, 0x01, 0xC6
		});

//seg000:0000                         start           proc near
//seg000:0000 BA 77 1D                                mov     dx, seg dseg

		//dseg:0  => IDA: 1D770 = dos: 1D77*10h

		//we need a relocation entry that targets the 77 0D part of the first {BA 77 0D} opcodes

		// OK
		if (!patch(0x6, { 0x3C }, { 0x3D })) // one entry more
		{
			printf("wrong original code at #2\n");
			return 5;
		}

		// OK
		if (!patch(0x110, { 0x00,0x00,0x00,0x00 }, { 0x00,0x00,0xCE,0x50 })) // one entry more
		{
			printf("wrong original code at #2\n");
			return 5;
		}
#endif
	}

	// write file complete
	if (!write_binary_file(content, out_path))
	{
		return 6;
	}
}
