#pragma once

#include <imgui.h>

#include "ui/panel.h"
#include "utils/image_manager.h"

namespace ui {

class AddGameDialog : public Panel {
 public:
  AddGameDialog(Storage* storage);

  void render() override;
  void end() override {};

  void open();
  void close();
  bool isOpen() const { return isOpen_; }

  void requestRefresh() { requestRefresh_ = true; }
  bool requestRefresh_ = false;

 private:
  bool isOpen_ = false;
  ImVec2 size_;

  char gameName_[256] = {};
  char filePath_[1024] = {};
  char bannerPath_[1024] = {};

  Texture bannerTexture_;
};

}  // namespace ui
