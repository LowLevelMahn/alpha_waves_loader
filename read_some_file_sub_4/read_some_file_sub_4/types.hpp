#pragma once

#include <cstdint>

struct gfx_block_t
{
	uint8_t filename[18]{};
	uint8_t byte_12h{};
	uint8_t byte_13h{};
	uint8_t byte_14h{};
	uint8_t byte_15h{};
	uint16_t word_16h{};
};
static_assert(sizeof(gfx_block_t) == 0x18);


struct slice_t
{
	void* data{};
	size_t size{};
};

