#include "ui/panels/settings.h"

#include <imgui.h>

using namespace ui;

void SettingsPanel::render() {
  ImGui::Begin(name_.c_str(), nullptr, false);
  ImGui::Text("Settings");
  ImGui::End();
}
