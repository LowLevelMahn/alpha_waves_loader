#include "emu.hpp"

#include "types.hpp"
#include "helper.hpp"

#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include <functional>

namespace original
{
    size_t emu_read_some_file_sub_4( emu_t& e,
                                     config_tat_t::executable_info_t* exec_info_,
                                     emu_t::ptr16_t& executable_buffer_ );
}

namespace cleanup
{
    size_t emu_read_some_file_sub_4( emu_t& e,
                                     config_tat_t::executable_info_t* exec_info_,
                                     emu_t::ptr16_t& executable_buffer_ );
}

void emu_GAME_START_sub_6( emu_t& e, emu_t::ptr16_t& executable_buffer_ )
{
    uint8_t fake_PSP_DTA[] = {
        0xCD, 0x20, 0xE8, 0xA2, 0x00, 0xEA, 0xFF, 0xFF, 0xAD, 0xDE, 0x7A, 0x08, 0xDD, 0x01, 0x00, 0x00, 0x18, 0x01,
        0xA2, 0x0B, 0xDD, 0x01, 0xDD, 0x01, 0x01, 0x01, 0x01, 0x00, 0x02, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x18, 0x00,
        0xE9, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0x21, 0xCB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x02, 0x20, 0x34, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00 };

    void* dest = e.memory( executable_buffer_ );

    ::memcpy( dest, fake_PSP_DTA, sizeof( fake_PSP_DTA ) );

    executable_buffer_.segment += 16; //
}

// TODO:
// 1. unit-test frame for original and port
// 2. port to pure C/C++
//   extract executables
//   rewrite exeload.asm to 16bit C code

using Test_func_t =
    std::function<size_t( emu_t& e, config_tat_t::executable_info_t* exec_info_, emu_t::ptr16_t& executable_buffer_ )>;

std::vector<uint8_t> extract_executable( const std::string& current_dir_,
                                         config_tat_t::gfx_infos_t& gfx_infos_,
                                         const size_t gfx_nr_,
                                         const size_t exec_nr_,
                                         Test_func_t test_func_ )
{
    const auto& blocks = gfx_infos_[gfx_nr_];

    emu_t e;

    e.dos_current_dir( current_dir_ );

    std::fill( e.memory().begin(), e.memory().end(), 0xBB );

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
    e.bx = block_begin_ptr16.offset + ( static_cast<uint16_t>( exec_nr_ ) * sizeof( config_tat_t::executable_info_t ) );

    emu_t::ptr16_t executable_buffer_ptr16 = executable_buffer_begin_ptr16;

    emu_GAME_START_sub_6( e, executable_buffer_ptr16 );

    size_t unpacked_size =
        test_func_( e, e.memory<config_tat_t::executable_info_t>( e.cs, e.bx ), executable_buffer_ptr16 );

    const size_t distance =
        emu_t::offset32( executable_buffer_ptr16 ) - emu_t::offset32( executable_buffer_begin_ptr16 );

    auto executable_begin = e.byte_ptr( executable_buffer_begin ) + 0x100;
    auto executable_end = executable_begin + unpacked_size;
    return { executable_begin, executable_end };
}

std::vector<uint8_t> original_extract_executable( const std::string& current_dir_,
                                                  config_tat_t::gfx_infos_t& gfx_infos_,
                                                  const size_t gfx_nr_,
                                                  const size_t exec_nr_ )
{
    return extract_executable( current_dir_, gfx_infos_, gfx_nr_, exec_nr_, original::emu_read_some_file_sub_4 );
}

std::vector<uint8_t> cleanup_extract_executable( const std::string& current_dir_,
                                                 config_tat_t::gfx_infos_t& gfx_infos_,
                                                 const size_t gfx_nr_,
                                                 const size_t exec_nr_ )
{
    return extract_executable( current_dir_, gfx_infos_, gfx_nr_, exec_nr_, cleanup::emu_read_some_file_sub_4 );
}

namespace port
{
    std::vector<uint8_t> extract_executable( const std::string& game_dir_,
                                             config_tat_t::gfx_infos_t& gfx_infos_,
                                             const size_t gfx_nr_,
                                             const size_t exec_nr_,
                                             std::vector<uint8_t>& before_game_sub_3_ );
}

void protection_tests();
int example_main();
void cc1_read_test();

int main()
{
#if 1
    cc1_read_test();
    return 0;

    return example_main();
    protection_tests();
    return 0;
#endif

    const std::string alpha_waves_filepath = R"(F:\projects\fun\dos_games_rev\alpha_waves_dev\tests\alpha)";

    // read config.tat
    const std::string config_tat_filepath = alpha_waves_filepath + "\\CONFIG.TAT";
    std::vector<uint8_t> config_tat_content = read_binary_file( config_tat_filepath );
    assert( config_tat_content.size() == sizeof( config_tat_t ) );
    config_tat_t* config_tat = reinterpret_cast<config_tat_t*>( config_tat_content.data() );

    // as in original code - replace first blank with 0 in filename
    for( auto& gfx : config_tat->gfx_info )
    {
        for( auto& exec : gfx.executable_info )
        {
            for( size_t i = 0; i < exec.filename.size(); ++i )
            {
                if( exec.filename[i] == 0x20 )
                {
                    exec.filename[i] = 0;
                    break;
                }
            }
        }
    }

#define TEST_FULL() ( true )

#if TEST_FULL()
    for( size_t gfx_nr = 0; gfx_nr < GFX_COUNT; ++gfx_nr )
#else
    size_t gfx_nr = 0;
#endif
    {
        auto& blocks = config_tat->gfx_info[gfx_nr].executable_info;

#if TEST_FULL()
        for( int exec_nr = 0; exec_nr < config_tat_t::gfx_info_t::EXECUTABLE_COUNT; ++exec_nr )
#else
        int exec_nr = 2;
#endif
        {
            std::vector<uint8_t> org_executable =
                original_extract_executable( alpha_waves_filepath, config_tat->gfx_info, gfx_nr, exec_nr );

            std::vector<uint8_t> port_executable =
                cleanup_extract_executable( alpha_waves_filepath, config_tat->gfx_info, gfx_nr, exec_nr );

            assert( org_executable == port_executable );

#if 0
            char filename[100]{};
            sprintf( filename, "d:/temp/gfx_%u_block_%u.bin", gfx_nr, exec_nr );
            write_binary_file( filename, port_executable.data(), port_executable.size() );
#endif
        }
    }

#if 0
    auto extract = [&alpha_waves_filepath]( config_tat_t::gfx_infos_t& gfx_infos_, gfx_type_t gfx_type_,
                                            exec_type_t exec_type_ ) {
        return original_extract_executable( alpha_waves_filepath, gfx_infos_, static_cast<size_t>( gfx_type_ ),
                                            static_cast<size_t>( exec_type_ ) );
    };

    // sound TSR are the same for all gfx types
    std::vector<uint8_t> adlib_sound = extract( config_tat->gfx_info, gfx_type_t::VGA, exec_type_t::Adlib );
    std::vector<uint8_t> tandy_sound = extract( config_tat->gfx_info, gfx_type_t::VGA, exec_type_t::Tandy );
    std::vector<uint8_t> pc_buz_sound = extract( config_tat->gfx_info, gfx_type_t::VGA, exec_type_t::PcBuz );

    std::vector<uint8_t> cga_game = extract( config_tat->gfx_info, gfx_type_t::CGA, exec_type_t::Game );
    std::vector<uint8_t> ega_game = extract( config_tat->gfx_info, gfx_type_t::EGA, exec_type_t::Game );
    std::vector<uint8_t> tandy_game = extract( config_tat->gfx_info, gfx_type_t::Tandy, exec_type_t::Game );
    std::vector<uint8_t> hercules_game = extract( config_tat->gfx_info, gfx_type_t::Hercules, exec_type_t::Game );
    std::vector<uint8_t> vga_game = extract( config_tat->gfx_info, gfx_type_t::VGA, exec_type_t::Game );
#endif

    return 0;
}
