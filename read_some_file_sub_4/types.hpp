#pragma once

#include <array>
#include <cstdint>
#include <cstddef>

struct slice_t
{
    uint8_t* data{};
    size_t size{};
};

enum class gfx_type_t : size_t
{
    CGA = 0,
    EGA = 1,
    Tandy = 2,
    Hercules = 3,
    VGA = 4
};

enum class exec_type_t : size_t
{
    Adlib = 0,
    Tandy = 1,
    PcBuz = 2,
    Game = 3
};

constexpr size_t GFX_COUNT = 5; // 0=CGA, 1=EGA, 2=Tandy, 3=Hercules, 4=VGA

#pragma pack( push, 1 )

// i don't like file-layouts as packed structs
// buts its easier to document findings with them :)

// HIGHSCOR.QB file layout

struct highscore_line_t
{
    char name[30]; // filled with 0x20
    char value[8];
    uint8_t end; // 0 byte as end mark
};

struct highscor_qb_t
{
    highscore_line_t line[14];
};
static_assert( sizeof( highscor_qb_t ) == 546, "wrong size" );

// CONFIG.TAT file layout

struct config_tat_t
{
    struct executable_info_t
    {
        std::array<char, 18> filename;
        uint8_t byte_12h{};
        uint8_t byte_13h{};
        uint8_t byte_14h{};
        uint8_t byte_15h{};
        uint16_t word_16h{};
    };
    static_assert( sizeof( executable_info_t ) == 0x18, "wrong size" );

    struct gfx_info_t
    {
        static constexpr size_t EXECUTABLE_COUNT = 4;
        // Sound COM TSR (always the same for every gfx version)
        //   0=Adlib
        //   1=Tandy
        //   2=PC Buz
        // 3=Game EXE

        using executable_info_t = std::array<executable_info_t, EXECUTABLE_COUNT>;
        executable_info_t executable_info;
        uint8_t end_mark; // always 0xFF
    };
    static_assert( sizeof( gfx_info_t ) == 4 * sizeof( executable_info_t ) + 1, "wrong size" );

    uint16_t offset_gfx_offset_table; // 0x000E
    uint16_t offset_game_name;        // 0x01FD
    uint16_t offset_publisher;        // 0x021E
    uint16_t offset_disk_name;        // 0x0209
    uint16_t offset_file_end;         // 0x0233

    uint16_t unknown0; // 0xFF 0xFF
    uint16_t unknown1; // 0x00 0x00

    // offset: 0x000E
    std::array<uint16_t, GFX_COUNT> gfx_offset_table;
    //[0] = 0x0018
    //[1] = 0x00DA
    //[2] = 0x013B
    //[3] = 0x0079
    //[4] = 0x019C

    using gfx_infos_t = std::array<gfx_info_t, GFX_COUNT>;
    gfx_infos_t gfx_info;
    // offset: 0x0018 CGA
    // offset: 0x00DA EGA
    // offset: 0x013B Tandy
    // offset: 0x0079 Hercules
    // offset: 0x019C VGA

    std::array<char, 12> game_name; // "ALPHA WAVES.\0x0"
    std::array<char, 21> publisher; // "\0x1F(c) Infogrames 1990\0x0"
    std::array<char, 21> disk_name; // " \0x2ALPHA WAVES DISK 1\0x0"
};
static_assert( sizeof( config_tat_t ) == 0x233, "wrong size" );

// PROGS.CC1 file layout

//000000: uint16_t = 6
//
//uint32_t[6]
//000002: uint32_t PC Buz data offset, if executable_info_t::byte_12h = 0
//000006: uint32_t Tandy data offset, if executable_info_t::byte_12h = 1
//000010: uint32_t Adlib data offset, if executable_info_t::byte_12h = 2
//000014: uint32_t CGA/Hercules data offset, if executable_info_t::byte_12h = 3
//000018: uint32_t EGA/VGA, if executable_info_t::byte_12h = 4
//000022: uint32_t Tandy, if executable_info_t::byte_12h = 5
//
//000026: 2*uint32_t PC Buz
//000034: 2422
//
//002456: 2*uint32_t
//002464: 2885        Tandy
//
//005349: 2*uint32_t
//005357: 3053        Adlib
//
//008410: 2*uint32_t
//008418: 51265       CGA,Hercules
//
//059683: 2*uint32_t
//059691: 51720       Tandy
//
//111411: 2*uint32_t
//111419: 50404       EGA,VGA

template <size_t Size>
struct exec_data_t
{
    uint32_t data_size;
    uint32_t unpacked_data_size; // maybe
    std::array<uint8_t, Size> data;
};

struct progs_cc1_t
{
    //0:
    uint16_t exec_table_count;
    //2:
    std::array<uint32_t, 6> execs_offset_table;
    //26
    //26 + execs_offset_table[n] offset
    struct sound_t
    {
        exec_data_t<2422> pc_buz;
        exec_data_t<2885> tandy;
        exec_data_t<3053> adlib;
    };

    sound_t sound;

    struct gfx_t
    {
        exec_data_t<51265> cga_hercules;
        exec_data_t<51720> ega_vga;
        exec_data_t<50404> tandy;
    };
    gfx_t gfx;
};

static_assert( sizeof( progs_cc1_t ) == 161823, "wrong size" );
static_assert( offsetof( progs_cc1_t, exec_table_count ) == 0, "wrong offset" );
static_assert( offsetof( progs_cc1_t, execs_offset_table ) == 2, "wrong offset" );
static_assert( offsetof( progs_cc1_t, sound.pc_buz ) == 26, "wrong offset" );
static_assert( offsetof( progs_cc1_t, sound.tandy ) == 2456, "wrong offset" );
static_assert( offsetof( progs_cc1_t, sound.adlib ) == 5349, "wrong offset" );
static_assert( offsetof( progs_cc1_t, gfx.cga_hercules ) == 8410, "wrong offset" );
static_assert( offsetof( progs_cc1_t, gfx.ega_vga ) == 59683, "wrong offset" );
static_assert( offsetof( progs_cc1_t, gfx.tandy ) == 111411, "wrong offset" );

#pragma pack( pop )
