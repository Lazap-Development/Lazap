#pragma once
#include <string>

#include "clients/client.h"
#include "storage/storage.h"

namespace ui {

class Panel {
 public:
  Panel(const std::string& name = "Panel", Storage* storage = nullptr)
      : name_(name), storage_(storage) {}
  virtual ~Panel() = default;

  virtual void init() {}
  virtual void render() = 0;
  virtual void end() {}

  const std::string& getName() const { return name_; }
  bool visible() const { return visible_; }
  void setVisible(bool v) { visible_ = v; }

  void setOnRefresh(std::function<void()> cb) { onRefresh_ = cb; }

 protected:
  std::string name_;
  Storage* storage_;
  bool visible_ = true;
  ImVec2 scale_ = ImVec2(0, 0);

  std::function<void()> onRefresh_;
};

}  // namespace ui
