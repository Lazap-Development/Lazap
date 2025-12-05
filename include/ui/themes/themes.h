#pragma once
#include <cstdint>

#include "imgui.h"

namespace Themes {

constexpr uint32_t ACCENT_COLOR = 0x7E4CEA;

void setDefaultDarkColors();
ImVec2 getScale(float x, float y);

}  // namespace Themes
