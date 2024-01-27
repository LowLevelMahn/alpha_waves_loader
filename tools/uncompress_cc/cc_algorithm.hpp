#pragma once

#include "helper.hpp"

// cc0 and cc1 uncompression algorithm

#include <vector>
#include <cstdint>

std::vector<uint8_t> uncompress(const std::vector<uint8_t>& packed_data_, const size_t unpacked_size_);

//TODO: still missing to prove that the uncompression is correctly reverse engineered and compatible in all game data cases
//std::vector<uint8_t> compress(const std::vector<uint8_t>& unpacked_data_);
