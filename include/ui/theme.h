#pragma once
#include <cstdint>

#include "imgui.h"

namespace Themes {

inline uint32_t ACCENT_COLOR = 0x7E4CEA;
inline ImU32 ACCENT_COLOR_IMGUI = IM_COL32(0x7E, 0x4C, 0xEA, 255);
inline float BG_OPACITY = 0.8;

void setDefaultDarkColors();

void drawInputBorder(float thickness = 2.0f, float rounding = 5.0f);

}  // namespace Themes
