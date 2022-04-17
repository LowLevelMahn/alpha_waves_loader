#include "emu.hpp"
#include "helper.hpp"

#define USE_INLINE_ASM() (true)

#if USE_INLINE_ASM
	#if defined( _WIN64 )
		#error inline asm only works with x86 builds
	#endif
#endif

namespace
{
	inline uint8_t* as_ptr(uint16_t& value_) noexcept
	{
		return reinterpret_cast<uint8_t*>(&value_);
	}

	inline uint8_t& low_ref(uint16_t& value_) noexcept
	{
		return as_ptr(value_)[0];
	}

	inline uint8_t& high_ref(uint16_t& value_) noexcept
	{
		return as_ptr(value_)[1];
	}

	uint16_t get_flags_value(const emu_t::flags_t& flags_)
	{
		// combine flags into word

		//0b0000000000000000
		//  RNNNoditszRaRpRc

		const uint16_t result = 0b1111000000101010 // all reserved = 1
			+ (flags_.carry << 0)
			+ (flags_.parity << 2)
			+ (flags_.adjust << 4)
			+ (flags_.zero << 6)
			+ (flags_.sign << 7)
			+ (flags_.trap << 8)
			+ (flags_.intr << 9)
			+ (flags_.dir << 10)
			+ (flags_.oflow << 11);

		return result;
	}

	void set_flags_value(emu_t::flags_t& flags_, const uint16_t& value_) noexcept
	{
		// 0100001000000010
		flags_.carry = value_ & 0x0001;

		// reserved
		//assert(value_ & 0x0002);

		flags_.parity = value_ & 0x0004;

		// reserved
		//assert(value_ & 0x0008);

		flags_.adjust = value_ & 0x0010;

		// reserved
		//assert(value_ & 0x0020);

		flags_.zero = value_ & 0x0040;
		flags_.sign = value_ & 0x0080;
		flags_.trap = value_ & 0x0100;
		flags_.intr = value_ & 0x0200;
		flags_.dir = value_ & 0x0400;
		flags_.oflow = value_ & 0x0800;
		//+286
		//iopl = (value_ & 0x3000) >> 12;
		//assert(((value_ & 0x3000) >> 12) == 0b11);
		//nt = value_ & 0x4000;
		//assert(value_ & 0x4000);
		//reserved = value_ & 0x8000;
		//assert(value_ & 0x8000);
	}
}

constexpr size_t MAX_FILE_HANDLE = 100;

emu_t::emu_t():
	ax{ m_regs[0] }, al{ low_ref(ax) }, ah{ high_ref(ax) },
	bx{ m_regs[1] }, bl{ low_ref(bx) }, bh{ high_ref(bx) },
	cx{ m_regs[2] }, cl{ low_ref(cx) }, ch{ high_ref(cx) },
	dx{ m_regs[3] }, dl{ low_ref(dx) }, dh{ high_ref(dx) },
	m_memory(0x100000) // 2^20 bytes
{
	for (size_t i = 0; i < MAX_FILE_HANDLE; ++i)
	{
		m_free_file_handles.push(i);
	}
}

#define ASM_OP_2_RES(OPERATION, OP1, OP2, REG1, REG2)\
{\
    auto REG1##_value = OP1; \
	uint16_t flags_value = get_flags_value(flags); \
    {\
	__asm pushf \
\
	__asm push flags_value \
    __asm popf \
\
	__asm mov REG1, REG1##_value \
	__asm mov REG2, OP2 \
	__asm OPERATION REG1, REG2 \
\
    __asm pushf \
    __asm pop flags_value \
\
    __asm mov REG1##_value, REG1 \
\
    __asm popf \
	} \
    OP1 = REG1##_value; \
    set_flags_value(flags, flags_value); \
}

#define ASM_OP16_2_RES(OPERATION, OP1, OP2)\
{\
	ASM_OP_2_RES(OPERATION, OP1, OP2, ax, bx)\
}

#define ASM_OP8_2_RES(OPERATION, OP1, OP2)\
{\
	ASM_OP_2_RES(OPERATION, OP1, OP2, al, bl)\
}

void emu_t::push(uint16_t value_)
{
	//SS:SP
	assert(m_stack_top < m_stack.size());
	m_stack[m_stack_top++] = value_;
}

void emu_t::pop(uint16_t& value_)
{
	//SS:SP
	assert(m_stack_top > 0);
	value_ = m_stack[--m_stack_top];
}

bool emu_t::loop()
{
	return --cx > 0;
}

int emu_t::dir_advance() const
{
	return flags.dir ? -1 : 1;
}

void emu_t::stosb()
{
	stos(al);
}

void emu_t::lodsb()
{
	lods(al);
}

void emu_t::stosw()
{
	stos(ax);
}

void emu_t::lodsw()
{
	lods(ax);
}

void emu_t::rep_stosb()
{
	for (; cx > 0; --cx)
	{
		stosb();
	}
}

void emu_t::rep_stosw()
{
	for (; cx > 0; --cx)
	{
		stosw();
	}
}

void emu_t::rep_movsb()
{
	for (; cx > 0; --cx)
	{
		lodsb();
		stosb();
	}
}

void emu_t::add(uint8_t& op1_, uint8_t op2_)
{
	ASM_OP8_2_RES(add, op1_, op2_)
}

void emu_t::sub(uint8_t& op1_, uint8_t op2_)
{
	ASM_OP8_2_RES(sub, op1_, op2_)
}

void emu_t::add(uint16_t& op1_, uint16_t op2_)
{
	ASM_OP16_2_RES(add, op1_, op2_)
}

void emu_t::and_w(uint16_t& op1_, uint16_t op2_)
{
	ASM_OP16_2_RES(and, op1_, op2_)
}

void emu_t::adc(uint16_t& op1_, uint16_t op2_)
{
	ASM_OP16_2_RES(adc, op1_, op2_)
}

void emu_t::sub(uint16_t& op1_, uint16_t op2_)
{
	ASM_OP16_2_RES(sub, op1_, op2_)
}

void emu_t::shr(uint16_t& op1_, uint8_t op2_)
{
	ASM_OP_2_RES(shr, op1_, op2_, ax, cl)
}

void emu_t::rcr(uint16_t& op1_, uint8_t op2_)
{
	ASM_OP_2_RES(rcr, op1_, op2_, ax, cl)
}

void emu_t::shl(uint16_t& op1_, uint8_t op2_)
{
	ASM_OP_2_RES(shl, op1_, op2_, ax, cl)
}

void emu_t::sbb(uint16_t& op1_, uint16_t op2_)
{
	ASM_OP16_2_RES(sbb, op1_, op2_)
}

#define ASM_OP_2(OPERATION, OP1, OP2, REG1, REG2)\
{\
    auto REG1##_value = OP1; \
	uint16_t flags_value = get_flags_value(flags);\
    {\
	__asm pushf \
\
	__asm push flags_value \
    __asm popf \
\
	__asm mov REG1, REG1##_value \
	__asm mov REG2, OP2 \
	__asm OPERATION REG1, REG2 \
\
    __asm pushf \
    __asm pop flags_value \
\
    __asm mov REG1##_value, REG1 \
\
    __asm popf \
	} \
/* there is no result to set */ \
   set_flags_value(flags, flags_value); \
}

#define ASM_OP16_2(OPERATION, OP1, OP2)\
{\
	ASM_OP_2(OPERATION, OP1, OP2, ax, bx)\
}

#define ASM_OP8_2(OPERATION, OP1, OP2)\
{\
	ASM_OP_2(OPERATION, OP1, OP2, al, bl)\
}

void emu_t::cmp(uint16_t op1_, uint16_t op2_)
{
	ASM_OP16_2(cmp, op1_, op2_)
}

void emu_t::cmp(uint8_t op1_, uint8_t op2_)
{
	ASM_OP8_2(cmp, op1_, op2_)
}

void emu_t::xchg(uint8_t& op1_, uint8_t& op2_)
{
	uint8_t tmp = op1_;
	op1_ = op2_;
	op2_ = tmp;
}

void emu_t::test(uint8_t op1_, uint8_t op2_)
{
	ASM_OP8_2(test, op1_, op2_)
}

void emu_t::xor_b(uint8_t& op1_, uint8_t op2_)
{
	ASM_OP8_2_RES(xor, op1_, op2_)
}

void emu_t::or_w(uint16_t& op1_, uint16_t op2_)
{
	ASM_OP16_2_RES(or, op1_, op2_)
}

void emu_t::or_b(uint8_t& op1_, uint8_t op2_)
{
	ASM_OP8_2_RES(or, op1_, op2_)
}

void emu_t::xor_w(uint16_t& op1_, uint16_t op2_)
{
	ASM_OP16_2_RES(xor, op1_, op2_)
}

#define ASM_OP_1_RES(OPERATION, OP1, REG1)\
{\
    auto REG1##_value = OP1; \
	uint16_t flags_value = get_flags_value(flags); \
    {\
	__asm pushf \
\
	__asm push flags_value \
    __asm popf \
\
	__asm mov REG1, REG1##_value \
	__asm OPERATION REG1 \
\
    __asm pushf \
    __asm pop flags_value \
\
    __asm mov REG1##_value, REG1 \
\
    __asm popf \
	} \
    OP1 = REG1##_value; \
    set_flags_value(flags, flags_value); \
}

#define ASM_OP16_1_RES(OPERATION, OP1)\
{\
	ASM_OP_1_RES(OPERATION, OP1, ax)\
}

#define ASM_OP8_1_RES(OPERATION, OP1)\
{\
	ASM_OP_1_RES(OPERATION, OP1, al, bl)\
}

void emu_t::inc(uint16_t& op_)
{
	ASM_OP16_1_RES(inc, op_)
}

void emu_t::dec(uint16_t& op_)
{
	ASM_OP16_1_RES(dec, op_)
}

void emu_t::intr_0x21()
{
	// DOS API implementation
	switch (ah)
	{
	case 0x3d:// file_open
	{
		assert(al == 0); // read only
		const std::string filename = memory<char>(ds, dx);

		int free_handle = m_free_file_handles.top();
		m_free_file_handles.pop();

		const std::string current_dir = R"(F:\projects\fun\dos_games_rev\alpha_waves_dev\tests\alpha)";
		const std::string file_path = current_dir + "\\" + filename;
		FILE* fp = fopen(file_path.c_str(), "rb");
		assert(fp);
		file_info_t fi{ file_path, fp };

		m_open_files.insert({ free_handle, fi });

		clc();
		ax = free_handle;
	}
	break;
	case 0x3f:// file_read
	{
		auto fi = get_file_info(bx);

		const int count = cx;
		uint8_t* buffer = memory<uint8_t>(ds, dx);

		size_t read_bytes = fread(buffer, 1, count, fi.fp);

		long at = ftell(fi.fp);

		printf("file_read size: %i, at: %i\n%s\n", count, at, hexdump(buffer, count, 5*16).c_str());

		ax = static_cast<uint16_t>(read_bytes);

		clc();
	}
	break;
	case 0x42:// file_seek
	{
		auto fi = get_file_info(bx);

		const int origin = [](uint8_t al) 
		{
			switch (al)
			{
			case 0: return SEEK_SET;
			case 1: return SEEK_CUR;
			case 2: return SEEK_END;
			default: assert(false);
			}
			return -1;
		}(al);

		int pos = (cx << 16) + dx;

		int res = fseek(fi.fp, pos, origin);
		assert(res == 0);

		long curr_pos = ftell(fi.fp);
		uint32_t cp = curr_pos;

		dx = hi(cp);
		ax = lo(cp);
		clc();
	}
	break;
	case 0x3E:// file_close
	{
		int file_handle = bx;

		auto fi_it = m_open_files.find(file_handle);
		assert(fi_it != m_open_files.end());

		int res = fclose(fi_it->second.fp);
		assert(res == 0);

		fi_it->second = {};

		m_open_files.erase(fi_it);
		m_free_file_handles.push(file_handle);

		clc();
	}
		break;
	default: assert(false);
	}

	// mem_alloc
	// mem_resize
	// mem_free
}

void load(uint16_t& segment_, uint16_t& offset_, emu_t::ptr16_t ptr_)
{
	segment_ = ptr_.segment;
	offset_ = ptr_.offset;
}

void load(uint16_t& segment_, uint16_t& offset_, uint32_t ptr_)
{
	segment_ = hi(ptr_);
	offset_ = lo(ptr_);
}

void emu_t::lds(uint16_t& ofs_, ptr16_t ptr_)
{
	load(ds, ofs_, ptr_);
}

void emu_t::lds(uint16_t& ofs_, uint32_t ptr_)
{
	load(ds, ofs_, ptr_);
}

void emu_t::les(uint16_t& ofs_, ptr16_t ptr_)
{
	load(es, ofs_, ptr_);
}

void emu_t::les(uint16_t& ofs_, uint32_t ptr_)
{
	load(es, ofs_, ptr_);
}

bool emu_t::jz() const
{
	return flags.zero;
}

bool emu_t::jnz() const
{
	return !jz();
}

bool emu_t::ja() const
{
	return !flags.carry && !flags.zero;
}

bool emu_t::jb() const
{
	return flags.carry;
}

bool emu_t::jnb() const
{
	return !jb();
}

void emu_t::cld()
{
	flags.dir = false;
}

void emu_t::std()
{
	flags.dir = true;
}

void emu_t::clc()
{
	flags.carry = false;
}

void emu_t::stc()
{
	flags.carry = true;
}

void* emu_t::ptr(size_t offset32_)
{
	return &m_memory[offset32_];
}

void* emu_t::ptr(uint16_t segment_, uint16_t offset_)
{
	return memory(offset32(segment_, offset_));
}

void* emu_t::memory(size_t offset32_)
{
	return ptr(offset32_);
}

size_t emu_t::offset32(uint16_t segment_, uint16_t offset_)
{
	return segment_ * 16 + offset_;
}

size_t emu_t::offset32(const ptr16_t& ptr_)
{
	return offset32(ptr_.segment, ptr_.offset);
}

void* emu_t::memory(uint16_t segment_, uint16_t offset_)
{
	return ptr(segment_, offset_);
}

void* emu_t::memory(const ptr16_t& ptr_)
{
	return ptr(ptr_.segment, ptr_.offset);
}

emu_t::ptr16_t emu_t::ptr16(size_t offset32_)
{
	size_t segment = offset32_ / 16;
	size_t offset = offset32_ % 16;
	return { static_cast<uint16_t>(offset), static_cast<uint16_t>(segment) };
}

emu_t::file_info_t& emu_t::get_file_info(int handle_)
{
	auto fi_it = m_open_files.find(handle_);
	assert(fi_it != m_open_files.end());
	return fi_it->second;
}

