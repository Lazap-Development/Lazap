#pragma once
#include <string>
#include <vector>

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

  const std::string& name() const { return name_; }
  bool visible() const { return visible_; }
  void setVisible(bool v) { visible_ = v; }

 protected:
  std::string name_;
  Storage* storage_;
  bool visible_ = true;
};

}  // namespace ui
