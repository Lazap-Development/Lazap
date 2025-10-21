#pragma once
#include <vector>

#include "clients/client.h"

namespace ui {

class Panel {
 public:
  virtual ~Panel() = default;
  virtual void init() {}
  virtual void render() = 0;
  virtual void end() {}
  virtual void definePointers() = 0;
};

}  // namespace ui