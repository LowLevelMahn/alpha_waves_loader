#include <string>
#include <cstdint>
#include <vector>
#include <cassert>
#include <cstdio>
#include <fstream>

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

	{
		// remove interrupt 97h dependency for feature_flag

		// int 97h gets called in ega_vga.exe from file offset: 0x57DA

		// replaces this code...
		//     CD 97 : int 97h
		//     8B D8 : mov bx,ax
		const std::vector<uint8_t> org_code{ 0xCD, 0x97, 0x8B, 0xD8 };
		if (::memcmp(org_code.data(), &content[0x57DA], org_code.size()) != 0)
		{
			printf("wrong original code for interrupt 97h call\n");
			return 4;
		}

		std::vector<uint8_t> new_code{ 0xB8, 0x01, 0xC0, 0x90 };
		// ...with this
		//     B8 01 C0 : mov ax,0xC001  
		//     90       : nop
		::memcpy(&content[0x57DA], new_code.data(), new_code.size());
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
		::memcpy(&content[0x2B1B],new_code.data(), new_code.size());
	}

	// write file complete
	if (!write_binary_file(content, out_path))
	{
		return 6;
	}
}
