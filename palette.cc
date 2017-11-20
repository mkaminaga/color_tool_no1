  // @file palette.cc
  // @brief Palette class.
  // @author Mamoru Kaminaga
  // @date 2017-11-17
  // Copyright 2017 Mamoru Kaminaga
  // This program is provided with MIT license. See "LICENSE.md".
#include <assert.h>
#include <wchar.h>
#include <windows.h>
#include <vector>

#include "./palette.h"
#include "./utility.h"

#include "./resource.h"

Palette::Palette() = default;

void Palette::Create(
    HWND hwnd,
    const Vector2n& grid,
    bool read_file,
    const wchar_t* file_name) {
  // The palette size and color vector is set.
  grid_ = grid;
  color_num_ = grid_.x * grid_.y;
  colors_.resize(color_num_);

  // Window handle and size is acquired.
  HWND hwnd_palette = GetDlgItem(hwnd, IDC_PIC_PALETTE);
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

  // The colors are loaded.
  if (read_file) {
    if (!LoadColor(file_name)) {
      MessageBox(hwnd, L"Failed to open the file", L"Error", MB_OK);
    }
  }

  // Drawing tools are created.
  grid_pen_ = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));
  select_pen_ = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
  pens_.resize(color_num_);
  brushes_.resize(color_num_);
  COLORREF c;
  for (int color_id = 0; color_id < color_num_; ++color_id) {
    c = RGB(colors_[color_id].r, colors_[color_id].g, colors_[color_id].b);
    pens_[color_id] = CreatePen(PS_SOLID, 1, c);
    brushes_[color_id] = CreateSolidBrush(c);
  }

  // The default value is set.
  selected_color_id_ = 0;
}
void Palette::Paint(HWND hwnd) {
  // Window handle and size is acquired.
  HWND hwnd_palette = GetDlgItem(hwnd, IDC_PIC_PALETTE);

  // The color cells are drawn to the off-screen buffer.
  int x = 0;
  int y = 0;
  int x2 = 0;
  int y2 = 0;
  int color_id = 0;
  SelectObject(hdc_offscreen_, grid_pen_);
  for (int i = 0; i < grid_.y; ++i) {
    for (int j = 0; j < grid_.x; ++j) {
      // Draw target cell with tools.
      x = static_cast<int>(size_.x * j / static_cast<double>(grid_.x));
      y = static_cast<int>(size_.y * i / static_cast<double>(grid_.y));
      x2 = static_cast<int>(size_.x * (j + 1) / static_cast<double>(grid_.x));
      y2 = static_cast<int>(size_.y * (i + 1) / static_cast<double>(grid_.y));
      SelectObject(hdc_offscreen_, brushes_[color_id]);
      Rectangle(hdc_offscreen_, x, y, x2, y2);

      // Target cell is changed.
      ++color_id;
    }
  }

  // The selected grid guide is drawn.
  int ny = selected_color_id_ / grid_.x;
  int nx = selected_color_id_ - ny * grid_.x;
  x = static_cast<int>(size_.x * nx / static_cast<double>(grid_.x));
  y = static_cast<int>(size_.y * ny / static_cast<double>(grid_.y));
  x2 = static_cast<int>(size_.x * (nx + 1) / static_cast<double>(grid_.x));
  y2 = static_cast<int>(size_.y * (ny + 1) / static_cast<double>(grid_.y));
  SelectObject(hdc_offscreen_, select_pen_);
  SelectObject(hdc_offscreen_, brushes_[selected_color_id_]);
  Rectangle(hdc_offscreen_, x, y, x2, y2);

  // Flip screen.
  HDC hdc = GetDC(hwnd_palette);
  BitBlt(hdc, 0, 0, size_.x, size_.y, hdc_offscreen_, 0, 0, SRCCOPY);
  ReleaseDC(hwnd_palette, hdc);
}
void Palette::Destroy(HWND hwnd) {
  // Drawing tools are deleted.
  DeleteObject(grid_pen_);
  DeleteObject(select_pen_);
  for (int color_id = 0; color_id < color_num_; ++color_id) {
    DeleteObject(pens_[color_id]);
    DeleteObject(brushes_[color_id]);
  }

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

bool Palette::PickupColor(HWND hwnd, int mouse_x, int mouse_y) {
  // The relative mouse position is passed.
  RECT rc;
  GetWindowRect(GetDlgItem(hwnd, IDC_PIC_PALETTE), &rc);
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
  const int nx = static_cast<int>(grid_.x * x / static_cast<double>(size_.x));
  const int ny = static_cast<int>(grid_.y * y / static_cast<double>(size_.y));
  selected_color_id_ = ny * grid_.x + nx;
  return true;
}
RGBVecotr Palette::GetSelectedColor() const {
  return colors_[selected_color_id_];
}
int Palette::GetSelectedColorId() const {
  return selected_color_id_;
}
RGBVecotr Palette::GetColor(int color_id) const {
  return colors_[color_id];
}
Vector2n Palette::GetGrid() const {
  return grid_;
}
bool Palette::LoadColor(const wchar_t* file_name) {
  FILE* fp = nullptr;
  _wfopen_s(&fp, file_name, L"r");
  if (fp == nullptr) {
    return false;
  }
  int r = 0;
  int g = 0;
  int b = 0;
  int color_id = 0;
  while (fwscanf_s(fp, L"%d ,%d ,%d ,", &r, &g, &b) != EOF) {
    if (color_id >= color_num_) return true;
    colors_[color_id].r = r;
    colors_[color_id].g = g;
    colors_[color_id].b = b;
#ifdef DEBUG
    wprintf(L"color %d : %d, %d, %d\n", color_id, r, g, b);
#endif
    ++color_id;
  }
  fclose(fp);
  return true;
}
HBRUSH Palette::GetBrush(int color_id) const {
  return brushes_[color_id];
}
HPEN Palette::GetPen(int color_id) const {
  return pens_[color_id];
}
