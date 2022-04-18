#pragma once

#include <cstdint>
#include <cassert>
#include <array>
#include <vector>
#include <stack>
#include <map>
#include <string>

class emu_t
{
public:
    emu_t();

public:
    struct ptr16_t
    {
        ptr16_t() = default;
        ptr16_t( uint16_t segment_, uint16_t offset_ ) : offset( offset_ ), segment( segment_ )
        {
        }

        // in memory order
        uint16_t offset{};
        uint16_t segment{};
    };

public:
    // operations
    void push( uint16_t value_ );
    void pop( uint16_t& value_ );

    bool loop();

    void stosb();
    void lodsb();

    void stosw();
    void lodsw();

    void rep_stosb();
    void rep_stosw();
    void rep_movsb();

    void add( uint8_t& op1_, uint8_t op2_ );
    void sub( uint8_t& op1_, uint8_t op2_ );

    void add( uint16_t& op1_, uint16_t op2_ );
    void and_w( uint16_t& op1_, uint16_t op2_ );
    void adc( uint16_t& op1_, uint16_t op2_ );
    void sub( uint16_t& op1_, uint16_t op2_ );
    void shr( uint16_t& op1_, uint8_t op2_ );
    void rcr( uint16_t& op1_, uint8_t op2_ );
    void shl( uint16_t& op1_, uint8_t op2_ );
    void sbb( uint16_t& op1_, uint16_t op2_ );
    void cmp( uint16_t op1_, uint16_t op2_ );
    void cmp( uint8_t op1_, uint8_t op2_ );

    void xchg( uint8_t& op1_, uint8_t& op2_ );
    void test( uint8_t op1_, uint8_t op2_ );
    void xor_b( uint8_t& op1_, uint8_t op2_ );

    void or_w( uint16_t& op1_, uint16_t op2_ );
    void or_b( uint8_t& op1_, uint8_t op2_ );
    void xor_w( uint16_t& op1_, uint16_t op2_ );
    void inc( uint16_t& op_ );
    void dec( uint16_t& op_ );

    void intr_0x21();

    void lds( uint16_t& ofs_, ptr16_t ptr_ );
    void lds( uint16_t& ofs_, uint32_t ptr_ );

    void les( uint16_t& ofs_, ptr16_t ptr_ );
    void les( uint16_t& ofs_, uint32_t ptr_ );

    bool jz() const;
    bool jnz() const;
    bool ja() const;
    bool jb() const;
    bool jnb() const;

    void cld();
    void std();

    void clc();
    void stc();

public:
    // registers
    // direct references - better for debugging (variable watch + speed) in compare to methods or array access
    uint16_t& ax;
    uint8_t& al;
    uint8_t& ah;

    uint16_t& bx;
    uint8_t& bl;
    uint8_t& bh;

    uint16_t& cx;
    uint8_t& cl;
    uint8_t& ch;

    uint16_t& dx;
    uint8_t& dl;
    uint8_t& dh;

    // segment registers
    uint16_t cs{};
    uint16_t ds{};
    uint16_t es{};
    uint16_t ss{};

    uint16_t di{};
    uint16_t si{};
    uint16_t bp{};
    uint16_t sp{};

    struct flags_t
    {
        // https://en.wikipedia.org/wiki/FLAGS_register
        bool carry{}; // 0
        // reserved // 1
        bool parity{}; // 2
        // reserved // 3
        bool adjust{}; // 4
        // reserved // 5
        bool zero{};  // 6
        bool sign{};  // 7
        bool trap{};  // 8
        bool intr{};  // 9
        bool dir{};   // 10
        bool oflow{}; // 11
        //+286
        //int iopl{}; //12-13
        //bool nested{}; //14
        //reserved //15
    };

    flags_t flags;

private:
    int dir_advance() const;

    template <typename Type>
    void stos( const Type& value_ )
    {
        auto target = memory<Type>( es, di );
        *target = value_;
        di += dir_advance() * sizeof( value_ );
    }

    template <typename Type>
    void lods( Type& value_ )
    {
        auto src = memory<Type>( ds, si );
        value_ = *src;
        si += dir_advance() * sizeof( value_ );
    }

    void* ptr( size_t offset32_ );
    void* ptr( uint16_t segment_, uint16_t offset_ );

public:
    void* memory( size_t offset32_ );
    static size_t offset32( uint16_t segment_, uint16_t offset_ );
    static size_t offset32( const ptr16_t& ptr_ );
    void* memory( uint16_t segment_, uint16_t offset_ );
    void* memory( const ptr16_t& ptr_ );
    static ptr16_t ptr16( size_t offset32_ );

    template <typename Type>
    Type* memory( size_t offset32_ )
    {
        return reinterpret_cast<Type*>( ptr( offset32_ ) );
    }

    template <typename Type>
    Type* memory( uint16_t segment_, uint16_t offset_ )
    {
        return reinterpret_cast<Type*>( ptr( segment_, offset_ ) );
    }

    uint8_t* byte_ptr( uint16_t segment_, uint16_t offset_ )
    {
        return memory<uint8_t>( segment_, offset_ );
    }

    uint8_t* byte_ptr( const ptr16_t ptr_ )
    {
        return byte_ptr( ptr_.segment, ptr_.offset );
    }

    uint8_t* byte_ptr( size_t offset32_ )
    {
        return memory<uint8_t>( offset32_ );
    }

    uint16_t* word_ptr( uint16_t segment_, uint16_t offset_ )
    {
        return memory<uint16_t>( segment_, offset_ );
    }

    uint16_t* word_ptr( const ptr16_t ptr_ )
    {
        return word_ptr( ptr_.segment, ptr_.offset );
    }

    uint16_t* word_ptr( size_t offset32_ )
    {
        return memory<uint16_t>( offset32_ );
    }

    std::vector<uint8_t>& memory()
    {
        return m_memory;
    }

public:
    void dos_current_dir( const std::string& current_dir_ );
    const std::string& dos_current_dir() const;

private:
    static constexpr size_t REG_COUNT = 4;

    //16 bits : AX    BX    CX    DX
    //8 bits :  AH AL BH BL CH CL DH DL
    std::array<uint16_t, REG_COUNT> m_regs{};

    std::vector<uint8_t> m_memory;

    std::array<uint16_t, 1000> m_stack;
    uint16_t m_stack_top{ 0 };

    // DOS emulation
    std::stack<int> m_free_file_handles;
    struct file_info_t
    {
        std::string filename;
        FILE* fp{};
    };
    std::map<int, file_info_t> m_open_files;

    std::string m_dos_current_dir;

private:
    file_info_t& get_file_info( int handle_ );
};
