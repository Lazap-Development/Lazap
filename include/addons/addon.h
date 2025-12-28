#pragma once

class Addon {
 public:
  virtual ~Addon() = default;

  virtual bool onLoad() = 0;
  virtual void onUpdate(double deltaTime) = 0;
  virtual void onUnload() = 0;
};
