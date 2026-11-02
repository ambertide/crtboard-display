#pragma once

#include <linux/fb.h>
#include <memory>
#include <stdint.h>

namespace crtboard {
class Render {
private:
  int renderFileDescriptor;
  bool locked = false;
  uint8_t *screenMemory;
  uint32_t memsize;
  struct fb_var_screeninfo userInfo;
  struct fb_fix_screeninfo fixedInfo;

public:
  Render();
  ~Render();

  /**
   * @brief Render a single frame to the frame buffer
   *
   * @warning This locks the frame buffer until the draw ends
   * intermingled frames are dropped!.
   * @param data Data to draw
   */
  void render(std::unique_ptr<uint8_t[]> data, int size);
};
} // namespace crtboard