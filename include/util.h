#pragma once

#include <cstdint>
#include <cstddef>

void pack_array(uint8_t *in, uint8_t *out, size_t s);
void unpack_array(uint8_t* in, uint8_t* out, size_t s);
int bitpop64(std::uint64_t x);
