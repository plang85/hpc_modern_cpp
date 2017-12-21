#pragma once

#include <array>

using cpu_info_t = std::array<uint32_t, 4>;


cpu_info_t cpu_info(int i);

void mult(float const *a, float const *b, float *c);
