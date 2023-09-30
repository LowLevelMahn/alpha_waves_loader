#include "emu.hpp"

#include <ctime>
#include <cstring>

uint16_t code_question_two_sub_1288E_original(emu_t& e, char* code_answer_, uint16_t row_, uint16_t column_)
{
	e.di = 0x7D76;

	e.ax = row_;       // !!! code modified by code_question_screen_sub_1283E
							//
							// always range: 0..9
							//
							// mov ax,0..9
	e.shl(e.ax, 4);           // ax *= 16 ==> * TABLE_ROW_BYTE_SIZE == 8*2

	e.bx = column_;     // !!! code modified by code_question_screen_sub_12865
							//
							// always range: 0...8
							//
							// mov bx,0...8
	e.si = 0;

	e.shl(e.bx, 1);
	e.add(e.ax, e.bx);
	e.add(e.di, e.ax);
	e.dx = 0;
	e.cx = *e.word_ptr(e.ds, e.di);        // cx = code_answer_pairs[ax][bx]
	e.di = 0x2B43;       // constant
	e.al = code_answer_[e.si];
	e.sub(e.al, 0x1B);         // transform to 0-9
	e.ah = e.al;
	e.shl(e.ah, 2);
	e.add(e.ah, e.al);
	e.add(e.ah, e.ah);
	e.al = code_answer_[e.si+1];
	e.sub(e.al, 0x1B);         // transform to 0-9
	e.add(e.ah, e.al);
	e.bx = 0x2AD2;       // constant
	e.inc(e.dx);
	e.cmp(e.ah, e.cl);
	if (e.jnz()) goto loc_128E4;
	e.di = 0x24D3 + 2; // also used for code modification?
	e.shl(e.ax, 1);
	e.shl(e.ah, 1);
	e.inc(e.dx);
	e.add(e.ah, e.al);
	e.add(e.ax, e.di);
	e.add(e.si, 4);          // at the end

loc_128E4:                              // CODE XREF: code_question_two_sub_1288E+45
	e.al = code_answer_[e.si-2];
	e.sub(e.al, 0x1B);         // transform to 0-9
	e.ah = e.al;
	e.shl(e.ah, 2);
	e.add(e.ah, e.al);
	e.add(e.ah, e.ah);
	e.al = code_answer_[e.si-1];
	e.sub(e.al, 0x1B);         // transaform to 0-9
	e.add(e.ah, e.al);
	e.inc(e.dx);
	e.bx = 0x2AD2;       // constant, bx not in use, or used by caller?
	e.cmp(e.ah, e.ch);
	if (e.jnz()) goto loc_12910;
	e.bx = 0x21B0 + 1; // modified place - here only read
	assert((e.di == 0x2B43) || (e.di == 0x24D3 + 2));
	e.ax = *e.word_ptr(e.cs, e.di);     // prepare for modification - only read here
	e.shr(e.ax, 1);
	e.inc(e.dx);
	assert(e.bx == 0x21B0 + 1);
	e.shl(*e.word_ptr(e.cs, e.bx), 1); // modifies code at always fix point cs:21B1

loc_12910:                              // CODE XREF: code_question_two_sub_1288E+74
	assert((e.di == 0x2B43) || (e.di == 0x24D3 + 2));
	e.add(*e.word_ptr(e.cs, e.di), e.ax);     // modifies code around
	e.ax = e.dx;          // code question result: dx = 2 = wrong, dx = 4 = correct

	return e.ax;
}

//struct answer_pair_t
//{
//	uint8_t lo;
//	uint8_t hi;
//};

constexpr size_t TABLE_WIDTH = 8;
constexpr size_t TABLE_HEIGHT = 10;
//using answer_table_t = answer_pair_t[TABLE_HEIGHT][TABLE_WIDTH];
using answer_table_t = uint16_t[TABLE_HEIGHT][TABLE_WIDTH];
static_assert(sizeof(answer_table_t) == 160, "wrong size");

uint16_t code_question_two_sub_1288E_ported(emu_t& e, char* code_answer_, uint16_t row_, uint16_t column_)
{
	auto dec_pair = [&e](uint8_t v0_, uint8_t v1_)
	{
		e.al = v1_ - 0x1B;
		e.ah = 10 * (v0_ - 0x1B) + e.al;
		//e.al needs to be set (maybe just a side effect of the obfuscation)
	};

	e.dx = 0;
	
	const answer_table_t& some_table = *e.memory<const answer_table_t>(e.ds, 0x7D76);
	e.cx = some_table[row_][column_];
	
	e.di = 0x2B43;       // constant

	dec_pair(code_answer_[0], code_answer_[1]);

	++e.dx;

	if (e.ah == e.cl)
	{
		e.di = 0x24D3 + 2; // also used for code modification?
		++e.dx;
	}

	dec_pair(code_answer_[2], code_answer_[3]);

	++e.dx;

	uint16_t* target0 = e.word_ptr(e.cs, e.di);
	if (e.ah == e.ch)
	{
		assert(e.di == 0x24D3 + 2);
		e.ax = *target0 / 2;     // prepare for modification - only read here
     	++e.dx;
		*e.word_ptr(e.cs, 0x21B0 + 1) *= 2; // modifies code at always fix point cs:21B1
	}

	assert((e.di == 0x2B43) || (e.di == 0x24D3 + 2));
	*target0 += e.ax;     // modifies code around

	return e.dx;          // code question result: dx = 2 = wrong, dx = 4 = correct
}

#if 0

uint8_t create_dec_pair(uint8_t v0_, uint8_t v1_)
{
	// v0 = x, v1 = y becomes xy
	return 10 * (v0_ - 0x1B) + (v1_ - 0x1B);
};

uint8_t code_question_two_sub_1288E_ported2(emu_t& e, char* code_answer_, uint16_t row_, uint16_t column_)
{
	const answer_table_t& some_table = *e.memory<const answer_table_t>(e.ds, 0x7D76);

	const answer_pair_t& dec_pair = some_table[row_][column_];

	uint8_t res = 2;

	// first number correct?

	uint16_t ofs = 0x24D3;

	if (create_dec_pair(code_answer_[0], code_answer_[1]) == dec_pair.lo)
	{
		ofs = 0x24D3 + 2;
		++res;
	}

	uint16_t val = create_dec_pair(code_answer_[2], code_answer_[3]);

	uint16_t* target = e.word_ptr(e.cs, ofs);

	// second number correct?

	if (val == dec_pair.hi)
	{
		//e.di what range?
		val = *target / 2;     // prepare for modification - only read here
		++res;
		*target *= 2; // modifies code at always fix point cs:21B1
	}

	*target += val; // modifies code around

	return res;
}

#endif

uint16_t random_range(uint16_t min_, uint16_t max_)
{
	return rand() % max_ + min_;
}

void protection_tests()
{
	srand(time(NULL));  //Changed from rand(). srand() seeds rand for you.

	{
		emu_t e;

		// 80 words (10*8 code words table)

	/*
			1    2    3    4    5    6    7    8
	 1  A 0813 1820 1314 0817 0414 1605 0410 1017
	 2  B 1012 0310 1712 1308 0816 1018 0512 1218
	 3  C 1805 1705 0515 1006 1005 2010 0312 1618
	 4  D 1320 1309 1304 1720 1210 1708 0516 1307
	 5  E 0612 1505 1612 1020 1220 1218 0618 0307
	 6  F 1308 0508 1508 1806 0416 1808 0810 0805
	 7  G 1520 0820 0507 0320 1710 1306 1016 0616
	 8  H 0513 1013 1215 2013 0315 1208 1206 0510
	 9  I 0812 2018 1318 2005 0610 1008 1606 0420
	10  J 1015 1810 2016 1620 0818 0814 1610 1219

	*/

#if 0
	// each byte is a number in the code pair - not just a uint16_t
	// first-pair second-pair
		int i = 0;
		for (int r = 0; r < 10; ++r)
		{
			for (int c = 0; c < 8; ++c)
			{
				printf("%02u", some_table[i]);
				i += 1;
				printf("%02u", some_table[i]);
				i += 1;
				printf(" ");
			}
			printf("\n");
		}

#endif
		const std::vector<uint8_t> org_code_0x24D3 { 0x81, 0xEE, 0x80, 0x00 };
		const std::vector<uint8_t> org_code_0x2B43 { 0x55, 0x8B  };
		const std::vector<uint8_t> org_code_0x21B0 { 0xBA, 0x80, 0x00 };

		auto prepare_mem = [org_code_0x24D3, org_code_0x2B43, org_code_0x21B0](emu_t& e)
		{

			::memset(e.memory().data(), 0, e.memory().size());

			e.ax = 0;
			e.bx = 0;
			e.cx = 0;
			e.dx = 0;

			e.sp = 0;
			e.si = 0;
			e.di = 0;

			e.es = 0;
			e.cs = 0x01ED;
			e.ds = 0x0F64;

			const uint8_t some_table[160] = {
			0x08, 0x0D, 0x12, 0x14, 0x0D, 0x0E, 0x08, 0x11, 0x04, 0x0E, 0x10, 0x05,	0x04, 0x0A, 0x0A, 0x11,
			0x0A, 0x0C, 0x03, 0x0A, 0x11, 0x0C, 0x0D, 0x08,	0x08, 0x10, 0x0A, 0x12, 0x05, 0x0C, 0x0C, 0x10,
			0x12, 0x05, 0x11, 0x05,	0x05, 0x0F, 0x0A, 0x06, 0x0A, 0x05, 0x14, 0x0A, 0x03, 0x0C, 0x10, 0x12,
			0x0D, 0x14, 0x0D, 0x09, 0x0D, 0x04, 0x11, 0x14, 0x0C, 0x0A, 0x11, 0x08,	0x05, 0x10, 0x0D, 0x07,
			0x06, 0x0C, 0x0F, 0x05, 0x10, 0x0C, 0x0A, 0x14,	0x0C, 0x14, 0x0C, 0x10, 0x06, 0x12, 0x03, 0x07,
			0x0D, 0x08, 0x05, 0x08,	0x0F, 0x09, 0x12, 0x06, 0x04, 0x10, 0x10, 0x06, 0x08, 0x0A, 0x08, 0x05,
			0x0F, 0x14, 0x08, 0x14, 0x05, 0x07, 0x03, 0x14, 0x11, 0x0A, 0x0D, 0x06,	0x0A, 0x10, 0x06, 0x10,
			0x05, 0x0D, 0x0A, 0x0D, 0x0C, 0x0F, 0x14, 0x0D,	0x03, 0x0F, 0x0C, 0x08, 0x0C, 0x06, 0x05, 0x0A,
			0x08, 0x0C, 0x14, 0x12,	0x0D, 0x12, 0x14, 0x05, 0x06, 0x0A, 0x0A, 0x08, 0x10, 0x08, 0x04, 0x14,
			0x0A, 0x0F, 0x12, 0x0A, 0x14, 0x10, 0x10, 0x14, 0x08, 0x12, 0x08, 0x0E,	0x10, 0x0A, 0x0C, 0x13
			};
			::memcpy(e.byte_ptr(e.ds, 0x7D76), some_table, sizeof(some_table));

			//cs:24D3
			{
				::memcpy(e.byte_ptr(e.cs, 0x24D3), org_code_0x24D3.data(), org_code_0x24D3.size());
			}

			//cs:2B43
			{
				uint8_t code[] = { 0x55, 0x8B }; // modified
				::memcpy(e.byte_ptr(e.cs, 0x2B43), org_code_0x2B43.data(), org_code_0x2B43.size());
			}

			//cs:21B0
			{
				uint8_t code[] = { 0xBA, 0x80, 0x00 }; // modified
				::memcpy(e.byte_ptr(e.cs, 0x21B0), org_code_0x21B0.data(), org_code_0x21B0.size());
			}
		};

		auto check_bytes = [](void* ptr_, const std::vector<uint8_t> data_)
		{
#if 1
			std::vector<uint8_t> copy(data_.size());
			::memcpy(copy.data(), ptr_, data_.size());
#endif
			assert(::memcmp(data_.data(),ptr_, data_.size()) == 0);
		};

		auto create_answer_code = [](std::string answer_str_)
		{
			std::vector<uint8_t> tmp(4);
			assert(answer_str_.size() == tmp.size());
			for (size_t i = 0; i < tmp.size(); ++i)
			{
				char a = answer_str_[i];
				assert(a >= '0' && a <= '9');
				uint8_t nr = a - '0';
				uint8_t code = nr + 0x1B;
				tmp[i] = code;
			}
			return tmp;
		};

		auto answer_code_to_str = [](const char* answer_code_)
		{
			std::string tmp;
			for (size_t i = 0; i < 4; ++i)
			{
				uint8_t code = answer_code_[i];
				uint8_t nr = code - 0x1B;
				char a = nr + '0';
				assert(a >= '0' && a <= '9');
				tmp += a;
			}
			return tmp;
		};

		assert(answer_code_to_str("\x1C\x24\x1D\x23")=="1928");
		assert(answer_code_to_str("\x1D\x1E\x1D\x1B")=="2320");
		assert(answer_code_to_str("\x1C\x21\x1C\x22")=="1617");
		assert(answer_code_to_str("\x1B\x23\x1C\x1D")=="0812");

		{
			//prepare_mem(e);
			//uint8_t res0 = code_question_two_sub_1288E_ported(e, "\x1C\x1B\x1B\x21", 2, 3);
			//assert(res0 == 4);
		}

		//03 0F 04 05 FF 00 00 25 00 21 00 00 00 00 00 02
		//                              -------------- answer-code buffer (only 4 bytes used of 5)

		std::vector<uint8_t> prepared_stack;

		auto prepare_answer = [create_answer_code, &prepared_stack](const std::string& answer_code_str_)
		{
			const std::vector<uint8_t> org_prepared_stack{ 0x03, 0x0F, 0x04, 0x05, 0xFF, 0x00, 0x00, 0x25, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };
			prepared_stack = org_prepared_stack;

			char* answer_code = (char*)prepared_stack.data()+0x0A; // 5 bytes
			auto code = create_answer_code(answer_code_str_);
			::memcpy(answer_code, code.data(), code.size());
			
			return answer_code;
		};

		{
			//correct: 1210 
			//first wrong: 2310

			// stack of code_question_two_sub_1288E calling function
			auto answer_code = prepare_answer("2310");
			
			prepare_mem(e);
			uint16_t res1 = code_question_two_sub_1288E_original(e, answer_code, 3, 4);
			assert(res1 == 2);
			check_bytes(e.byte_ptr(e.cs, 0x24D3), org_code_0x24D3);
			//check_bytes(e.byte_ptr(e.cs, 0x2B43), { 0x17, 0xe2 }); // ???
			//check_bytes(e.byte_ptr(e.cs, 0x21B0), org_code_0x21B0);
		}

		{
//wanted I7: 1606
//given: 1617 - second pair wrong
//ax=8, bx=6
//
//cs:24D3: 81 EE 0F 11 
//cs:2B43: 55 8B EC 06
//cs:21B0: B8 0A 00 B9

			// stack of code_question_two_sub_1288E calling function
			auto answer_code = prepare_answer("1617");

			prepare_mem(e);
			uint16_t res2 = code_question_two_sub_1288E_original(e, answer_code, 8, 6);
			//assert(res2 == 2);
			//check_bytes(e.byte_ptr(e.cs, 0x24D3), org_code_0x24D3);
			//check_bytes(e.byte_ptr(e.cs, 0x2B43), {0x55, 0x8B});
			//check_bytes(e.byte_ptr(e.cs, 0x21B0), {0xB8, 0x0A, 0x00, 0xB9});
		}

		{
		/*
wanted I1 : 0812 
given: 0812 - correct
ax=8, bx=0

cs:24D3: 81 EE 0C 00
cs:2B43: 55 8B EC 06
cs:21B0: BA 14 00 B9 
		*/

			// stack of code_question_two_sub_1288E calling function
			auto answer_code = prepare_answer("0812");

			prepare_mem(e);
			uint16_t res3 = code_question_two_sub_1288E_original(e, answer_code, 8, 0);
			assert(res3 == 4);
			check_bytes(e.byte_ptr(e.cs, 0x24D3), { 0x81, 0xEE, 0xC0, 0x00 });
			check_bytes(e.byte_ptr(e.cs, 0x2B43), org_code_0x2B43);
			check_bytes(e.byte_ptr(e.cs, 0x21B0), { 0xBA, 0x00, 0x01 });
		}
	}

	return;

	//for (uint16_t rand1 = 0; rand1 < 10; ++rand1)
	//{
	//	for (uint16_t rand2 = 0; rand2 < 9; ++rand2)
	//	{
	//		uint8_t res = code_question_two_sub_1288E("\x10\x20\x30\x40", rand1, rand2);
	//	}
	//}
}

