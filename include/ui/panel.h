#pragma once
#include <string>
#include <vector>

#include "clients/client.h"

namespace ui {

class Panel {
 public:
  Panel(const std::string& name = "Panel") : name_(name) {}
  virtual ~Panel() = default;

  virtual void init() {}
  virtual void render() = 0;
  virtual void end() {}
  virtual void definePointers() = 0;

  const std::string& name() const { return name_; }
  bool visible() const { return visible_; }
  void setVisible(bool v) { visible_ = v; }

 protected:
  std::string name_;
  bool visible_ = true;
};

}  // namespace ui