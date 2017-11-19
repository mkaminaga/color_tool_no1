  // @file canvas.h
  // @brief Canvas class.
  // @author Mamoru Kaminaga
  // @date 2017-11-17
  // Copyright 2017 Mamoru Kaminaga
  // This program is provided with MIT license. See "LICENSE.md".
#ifndef CANVAS_H_
#define CANVAS_H_

#include <wchar.h>
#include <windows.h>
#include <vector>

#include "./palette.h"
#include "./range.h"

#include "./utility.h"

class Canvas {
 public:
  Canvas();

  void Create(
      HWND hwnd,
      const Vector2n& pixel,
      const Palette& palette,
      const Range& range);
  void Paint(HWND hwnd, const Palette& palette);
  void Destroy(HWND hwnd);

  void Update(const Palette& palette, const Range& range);
  Vector2n GetPixels() const;
  int GetColorId(int pixel_id) const;

 private:
  HDC hdc_offscreen_;
  HBITMAP hdc_bitmap_;

  int pixel_num_;
  Vector2n pixel_;
  Vector2n size_;
  std::vector<int> color_id_;
};

#endif  // CANVAS_H_
