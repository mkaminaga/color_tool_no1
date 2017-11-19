  // @file range.h
  // @brief Range class.
  // @author Mamoru Kaminaga
  // @date 2017-11-17
  // Copyright 2017 Mamoru Kaminaga
  // This program is provided with MIT license. See "LICENSE.md".
#ifndef RANGE_H_
#define RANGE_H_

#include <wchar.h>
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <memory>

#include "./palette.h"

#include "./utility.h"

class Range {
 public:
  Range();

  void Create(HWND hwnd, int grid, const wchar_t* file_name);
  void Paint(HWND hwnd, const Palette& palette);
  void Destroy(HWND hwnd);

  bool SelectGrid(HWND hwnd, int mouse_x, int mouse_y);
  void SetColor(HWND hwnd, const Palette& palette);
  void SetAllColor(HWND hwnd, const Palette& palette);

  int GetColorId(int grid_id) const;
  int GetGrid() const;

 private:
  HDC hdc_offscreen_;
  HBITMAP hdc_bitmap_;
  HPEN grid_pen_;
  HPEN select_pen_;

  // Data for GDI+.
  ULONG_PTR gdi_token_;
  std::unique_ptr<Gdiplus::Image> graph_image_;

  int grid_;
  int color_num_;
  int selected_grid_id_;
  Vector2n size_;
  std::vector<int> color_id_;
};

#endif  // RANGE_H_
