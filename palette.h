  // @file palette.h
  // @brief Palette class.
  // @author Mamoru Kaminaga
  // @date 2017-11-17
  // Copyright 2017 Mamoru Kaminaga
  // This program is provided with MIT license. See "LICENSE.md".
#ifndef PALETTE_H_
#define PALETTE_H_

#include <wchar.h>
#include <windows.h>
#include <vector>

#include "./utility.h"

class Palette {
 public:
  Palette();

  void Create(
      HWND hwnd,
      const Vector2n& grid,
      bool read_file,
      const wchar_t* file_name);
  void Paint(HWND hwnd);
  void Destroy(HWND hwnd);

  bool PickupColor(HWND hwnd, int mouse_x, int mouse_y);

  RGBVecotr GetSelectedColor() const;
  int GetSelectedColorId() const;
  RGBVecotr GetColor(int color_id) const;
  Vector2n GetGrid() const;

  HBRUSH GetBrush(int color_id) const;
  HPEN GetPen(int color_id) const;

 private:
  bool LoadColor(const wchar_t* file_name);

 private:
  HDC hdc_offscreen_;
  HBITMAP hdc_bitmap_;
  HPEN grid_pen_;
  HPEN select_pen_;
  std::vector<HBRUSH> brushes_;
  std::vector<HPEN> pens_;

  int selected_color_id_;
  int color_num_;
  Vector2n grid_;
  Vector2n size_;
  std::vector<RGBVecotr> colors_;
};

#endif  // PALETTE_H_
