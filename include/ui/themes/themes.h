#pragma once
#include <cstdint>

#include "imgui.h"

namespace Themes {

constexpr uint32_t ACCENT_COLOR = 0x7E4CEA;
constexpr ImU32 ACCENT_COLOR_IMGUI = IM_COL32(0x7E, 0x4C, 0xEA, 255);

void setDefaultDarkColors();

}  // namespace Themes
