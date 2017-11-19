  // @file canvas.cc
  // @brief Canvas class.
  // @author Mamoru Kaminaga
  // @date 2017-11-17
  // Copyright 2017 Mamoru Kaminaga
  // This program is provided with MIT license. See "LICENSE.md".
#include <wchar.h>
#include <windows.h>
#include <cmath>
#include <random>
#include <vector>

#include "./canvas.h"
#include "./palette.h"
#include "./range.h"
#include "./utility.h"

#include "./resource.h"

  // Fixed parameters for normal distribution used in this program.
#define NORMAL_DIST_MUE     (0.0)
#define NORMAL_DIST_SIGMA   (1.0)
#define NORMAL_DIST_RANGE   (11.5)

Canvas::Canvas() = default;

void Canvas::Create(
      HWND hwnd,
      const Vector2n& pixel,
      const Palette& palette,
      const Range& range) {
  // The palette size and color vector is set.
  pixel_ = pixel;
  pixel_num_ = pixel_.x * pixel_.y;
  color_id_.resize(pixel_num_);

  // Window handle and size is acquired.
  HWND hwnd_canvas = GetDlgItem(hwnd, IDC_PIC_CANVAS);
  RECT rc;
  GetClientRect(hwnd_canvas, &rc);
  size_.x = (rc.right - rc.left);
  size_.y = (rc.bottom - rc.top);

  // The off-screen draw buffer is created.
  hdc_offscreen_ = CreateCompatibleDC(nullptr);
  HDC hdc = GetDC(hwnd_canvas);
  hdc_bitmap_ = CreateCompatibleBitmap(hdc, size_.x, size_.y);
  ReleaseDC(hwnd_canvas, hdc);
  SelectObject(hdc_offscreen_, hdc_bitmap_);

  // The initial setup.
  Update(palette, range);
}
void Canvas::Paint(HWND hwnd, const Palette& palette) {
  // Window handle and size is acquired.
  HWND hwnd_canvas = GetDlgItem(hwnd, IDC_PIC_CANVAS);

  // The color cells are drawn to the off-screen buffer.
  int x = 0;
  int y = 0;
  int x2 = 0;
  int y2 = 0;
  int pixel_id = 0;
  for (int i = 0; i < pixel_.y; ++i) {
    for (int j = 0; j < pixel_.x; ++j) {
      // Draw target cell with tools.
      x = static_cast<int>(size_.x * j / static_cast<double>(pixel_.x));
      y = static_cast<int>(size_.y * i / static_cast<double>(pixel_.y));
      x2 = static_cast<int>(size_.x * (j + 1) / static_cast<double>(pixel_.x));
      y2 = static_cast<int>(size_.y * (i + 1) / static_cast<double>(pixel_.y));
      SelectObject(hdc_offscreen_, palette.GetPen(color_id_[pixel_id]));
      SelectObject(hdc_offscreen_, palette.GetBrush(color_id_[pixel_id]));
      Rectangle(hdc_offscreen_, x, y, x2, y2);
      // Target cell is changed.
      ++pixel_id;
    }
  }

  // Flip screen.
  HDC hdc = GetDC(hwnd_canvas);
  BitBlt(hdc, 0, 0, size_.x, size_.y, hdc_offscreen_, 0, 0, SRCCOPY);
  ReleaseDC(hwnd_canvas, hdc);
}
void Canvas::Destroy(HWND hwnd) {
  // The off-screen draw buffer is released.
  if (hdc_offscreen_) {
    DeleteDC(hdc_offscreen_);
    hdc_offscreen_ = nullptr;
  }
  if (hdc_bitmap_) {
    DeleteObject(hdc_bitmap_);
    hdc_bitmap_ = nullptr;
  }

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd);
}

void Canvas::Update(const Palette& palette, const Range& range) {
  // Grid of palette and range is acquired.
  int range_grid = range.GetGrid();
  Vector2n pallet_grids = palette.GetGrid();
  const int color_num = pallet_grids.x * pallet_grids.y;

  // Colors are distributed according to the normal distribution.
  const double delta = NORMAL_DIST_RANGE /
    static_cast<double>(range_grid);
  std::random_device seed_gen;
  std::default_random_engine engine(seed_gen());
  std::normal_distribution<> dist(NORMAL_DIST_MUE, NORMAL_DIST_SIGMA);

  double result = 0.0;
  bool exit = false;
  for (int pixel_id = 0; pixel_id < pixel_num_; ++pixel_id) {
    exit = false;
    color_id_[pixel_id] = 0;
    result = dist(engine);
    color_id_[pixel_id] = range.GetColorId(range_grid - 1);
    for (int grid_id = 0; (grid_id < range_grid) && !exit; ++grid_id) {
      if (std::fabs(result) < (delta * (grid_id + 1))) {
        color_id_[pixel_id] = range.GetColorId(grid_id);
        exit = true;
      }
    }
  }
}
Vector2n Canvas::GetPixels() const {
  return pixel_;
}
int Canvas::GetColorId(int pixel_id) const {
  return color_id_[pixel_id];
}
