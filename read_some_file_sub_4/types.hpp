#pragma once

#include <array>
#include <cstdint>

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

#pragma pack( push, 1 )

struct config_tat_t
{
    static constexpr size_t GFX_COUNT = 5; // 0=CGA, 1=EGA, 2=Tandy, 3=Hercules, 4=VGA

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

#pragma pack( pop )
