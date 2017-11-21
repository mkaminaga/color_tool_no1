  // @file range.cc
  // @brief Range class.
  // @author Mamoru Kaminaga
  // @date 2017-11-17
  // Copyright 2017 Mamoru Kaminaga
  // This program is provided with MIT license. See "LICENSE.md".
#include <wchar.h>
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <memory>

#include "./range.h"
#include "./utility.h"

#include "./resource.h"

Range::Range() = default;

void Range::Create(HWND hwnd, int grid, const wchar_t* file_name) {
  // The palette size and color vector is set.
  selected_grid_id_ = 0;
  grid_ = grid;
  color_num_ = grid_;
  color_id_.resize(grid_);

  // Window handle and size is acquired.
  HWND hwnd_palette = GetDlgItem(hwnd, IDC_PIC_RANGE);
  RECT rc;
  GetClientRect(hwnd_palette, &rc);
  size_.x = (rc.right - rc.left);
  size_.y = (rc.bottom - rc.top);

  // The off-screen draw buffer is created.
  hdc_offscreen_ = CreateCompatibleDC(nullptr);
  HDC hdc = GetDC(hwnd_palette);
  hdc_bitmap_ = CreateCompatibleBitmap(hdc, size_.x, size_.y);
  ReleaseDC(hwnd_palette, hdc);
  SelectObject(hdc_offscreen_, hdc_bitmap_);

  // Drawing tools are created, initial brush color is black.
  grid_pen_ = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));
  select_pen_ = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

  // The GDI+ object is created.
  Gdiplus::GdiplusStartupInput gdi_startup_info;
  Gdiplus::GdiplusStartup(&gdi_token_, &gdi_startup_info, nullptr);
  graph_image_.reset(new Gdiplus::Image(file_name));

  // The default value is set.
  selected_grid_id_ = 0;
}
void Range::Paint(HWND hwnd, const Palette& palette) {
  // Window handle and size is acquired.
  HWND hwnd_palette = GetDlgItem(hwnd, IDC_PIC_RANGE);

  // The color cells are drawn to the off-screen buffer.
  int x = 0;
  int x2 = 0;
  SelectObject(hdc_offscreen_, grid_pen_);
  for (int grid_id = 0; grid_id < grid_; ++grid_id) {
    // Draw target cell with tools.
    x = static_cast<int>(size_.x * grid_id / static_cast<double>(grid_));
    x2 = static_cast<int>(size_.x * (grid_id + 1) / static_cast<double>(grid_));
    SelectObject(hdc_offscreen_, palette.GetBrush(color_id_[grid_id]));
    Rectangle(hdc_offscreen_, x, 0, x2, size_.y);
  }

  // The selected grid guide is drawn.
  int nx = selected_grid_id_;
  x = static_cast<int>(size_.x * nx / static_cast<double>(grid_));
  x2 = static_cast<int>(size_.x * (nx + 1) / static_cast<double>(grid_));
  SelectObject(hdc_offscreen_, select_pen_);
  SelectObject(hdc_offscreen_, palette.GetBrush(color_id_[selected_grid_id_]));
  Rectangle(hdc_offscreen_, x, 0, x2, size_.y);

  // The image of distribution graph is drawn.
  Gdiplus::Graphics graphics(hdc_offscreen_);
  graphics.DrawImage(graph_image_.get(), 0, 0, size_.x, size_.y);

  // Flip screen.
  HDC hdc = GetDC(hwnd_palette);
  BitBlt(hdc, 0, 0, size_.x, size_.y, hdc_offscreen_, 0, 0, SRCCOPY);
  ReleaseDC(hwnd_palette, hdc);
}
void Range::Destroy(HWND hwnd) {
  // Drawing tools are deleted.
  DeleteObject(grid_pen_);
  DeleteObject(select_pen_);

  // The off-screen draw buffer is released.
  if (hdc_offscreen_) {
    DeleteDC(hdc_offscreen_);
    hdc_offscreen_ = nullptr;
  }
  if (hdc_bitmap_) {
    DeleteObject(hdc_bitmap_);
    hdc_bitmap_ = nullptr;
  }

  // The GDI+ object is deleted.
  graph_image_.reset(nullptr);
  Gdiplus::GdiplusShutdown(gdi_token_);

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd);
}

bool Range::SelectGrid(HWND hwnd, int mouse_x, int mouse_y) {
  // The relative mouse position is passed.
  RECT rc;
  GetWindowRect(GetDlgItem(hwnd, IDC_PIC_RANGE), &rc);
  POINT pt;
  pt.x = rc.left;
  pt.y = rc.top;
  ScreenToClient(hwnd, &pt);
  int x = mouse_x - pt.x;
  int y = mouse_y - pt.y;

  // Illegal cases.
  if (x < 0) return false;
  if (x > size_.x) return false;
  if (y < 0) return false;
  if (y > size_.y) return false;

  // Position to select.
  const int nx = static_cast<int>(grid_ * x / static_cast<double>(size_.x));
  selected_grid_id_ = nx;
  return true;
}
void Range::SetColor(HWND hwnd, const Palette& palette) {
  // Store color id.
  color_id_[selected_grid_id_] = palette.GetSelectedColorId();

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd);
}
void Range::SetAllColor(HWND hwnd, const Palette& palette) {
  for (int grid_id = 0; grid_id < grid_; ++grid_id) {
    // Store color id.
    color_id_[grid_id] = palette.GetSelectedColorId();
  }

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd);
}
int Range::GetColorId(int grid_id) const {
  return color_id_[grid_id];
}
int Range::GetGrid() const {
  return grid_;
}
