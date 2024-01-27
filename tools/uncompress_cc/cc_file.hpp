#pragma once

// uncompressor for cc0 and cc1 files

#include <string>
#include <vector>
#include <cstdint>

int cc0_uncompress(const std::string& cc_filepath, const std::vector<uint8_t>& content, const bool extract);
int cc1_uncompress(const std::string& cc_filepath, const std::vector<uint8_t>& content, const bool extract);

// TDODO remove - debugging purpose

extern std::vector<std::string> filenames_after;
