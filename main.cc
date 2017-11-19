  // @file main.cc
  // @brief Entry point.
  // @author Mamoru Kaminaga
  // @date 2017-11-17
  // Copyright 2017 Mamoru Kaminaga
  // This program is provided with MIT license. See "LICENSE.md".
#include <assert.h>
#include <wchar.h>
#include <windows.h>
#include <windowsx.h>
#include <memory>

#include "./canvas.h"
#include "./palette.h"
#include "./range.h"
#include "./utility.h"

#include "./resource.h"

namespace {
  // File scope variables.
std::unique_ptr<Palette> palette;
std::unique_ptr<Range> range;
std::unique_ptr<Canvas> canvas;

BOOL OnCreate(HWND hwnd, HWND hwnd_forcus, LPARAM lp) {
  // The palette class is created.
  const Vector2n pallete_grids(16, 16);
  palette.reset(new Palette());
  palette->Create(hwnd, pallete_grids, false, nullptr);

  // The range class is created.
  const int range_grids = 20;
  range.reset(new Range());
  range->Create(hwnd, range_grids, L"./data/normal_distribution.png");

  // The canvas class is created.
  const Vector2n pixel(64, 64);
  canvas.reset(new Canvas());
  canvas->Create(hwnd, pixel, *palette.get(), *range.get());

  // The WM_PAINT message is sent to the client window.
  InvalidateRect(hwnd, nullptr, FALSE);

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd_forcus);
  UNREFERENCED_PARAMETER(lp);
  return TRUE;
}
void OnDestroy(HWND hwnd) {
  // The palette class is destroyed.
  palette->Destroy(hwnd);
  palette.reset();

  // The range class is destroyed.
  range->Destroy(hwnd);
  range.reset();

  // The canvas class is destroyed.
  canvas->Destroy(hwnd);
  canvas.reset();

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd);
}
void OnCommand(HWND hwnd, int id, HWND hwnd_ctrl, UINT code_notify) {
  switch (id) {
    case IDC_LOADCOLOR:
      {
        // The color palette file name is acquired.
        wchar_t file_name[MAX_PATH] = {0};
        if (!GetPaletteFileName(hwnd, file_name)) return;

        // Palette is restarted.
        const Vector2n pallete_grids(16, 16);
        palette->Destroy(hwnd);
        palette->Create(hwnd, pallete_grids, true, file_name);

        // The WM_PAINT message is sent to the client window.
        InvalidateRect(hwnd, nullptr, FALSE);
      }
      break;
    case IDC_GENERATE:
      // The canvas pixels are drawn in random following normal distribution.
      canvas->Update(*palette.get(), *range.get());

      // The WM_PAINT message is sent to the client window.
      InvalidateRect(hwnd, nullptr, FALSE);
      break;
    case IDC_EXPORT:
      {
        // The file name is acquired.
        wchar_t file_name[MAX_PATH] = {0};
        if (!GetExportFileName(hwnd, file_name)) return;

        int file_name_length = wcslen(file_name);
        if (wcscmp(&file_name[file_name_length - 3], L"bmp") == 0) {
          // RGB buffer is created.
          Vector2n pixel = canvas->GetPixels();
          int array_size = pixel.x * pixel.y;
          std::vector<RGBVecotr> array(array_size);
          for (int pixel_id = 0; pixel_id < array_size; ++pixel_id) {
            array[pixel_id] = palette->GetColor(canvas->GetColorId(pixel_id));
          }

          // The file is created.
          bool result = CreateBitmapFile(
              file_name,
              pixel.x,
              pixel.y,
              array.data(),
              array_size);
          if (!result) {
            MessageBox(hwnd, L"Failed to create bitmap file", L"Error", MB_OK);
          }
          return;
        }
      }
      break;
    default:
      // No implementation.
      break;
  }

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd_ctrl);
  UNREFERENCED_PARAMETER(code_notify);
}
  // This function fills the selected range with the selected color.
void OnLButtonDown(HWND hwnd, BOOL double_click, int x, int y, UINT key_flags) {
  if (palette->PickupColor(hwnd, x, y)) {
    // The mouse position is passed to the palette class.
    range->SetColor(hwnd, *palette.get());
  } else {
    // The mouse position is passed to the range class.
    range->SelectGrid(hwnd, x, y);
  }

  // The WM_PAINT message is sent to the client window.
  InvalidateRect(hwnd, nullptr, FALSE);

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(double_click);
  UNREFERENCED_PARAMETER(key_flags);
}
  // This function fills all the ranges with the selected color.
void OnRButtonDown(HWND hwnd, BOOL double_click, int x, int y, UINT key_flags) {
  if (palette->PickupColor(hwnd, x, y)) {
    // The mouse position is passed to the palette class.
    range->SetAllColor(hwnd, *palette.get());
  }

  // The WM_PAINT message is sent to the client window.
  InvalidateRect(hwnd, nullptr, FALSE);

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(double_click);
  UNREFERENCED_PARAMETER(key_flags);
}
void OnClose(HWND hwnd) {
  // The main dialog is a modal dialog.
  EndDialog(hwnd, TRUE);
}
void OnPaint(HWND hwnd) {
  // The palette is drawn.
  palette->Paint(hwnd);

  // The range is drawn.
  range->Paint(hwnd, *palette.get());

  // The canvas is drawn.
  canvas->Paint(hwnd, *palette.get());

  // BeginPaint and EndPaint is called to clear WM_PAINT message.
  PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);
  EndPaint(hwnd, &ps);
}
INT_PTR CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
    HANDLE_DLG_MSG(hwnd, WM_INITDIALOG, OnCreate);
    HANDLE_DLG_MSG(hwnd, WM_DESTROY, OnDestroy);
    HANDLE_DLG_MSG(hwnd, WM_COMMAND, OnCommand);
    HANDLE_DLG_MSG(hwnd, WM_LBUTTONDOWN, OnLButtonDown);
    HANDLE_DLG_MSG(hwnd, WM_RBUTTONDOWN, OnRButtonDown);
    HANDLE_DLG_MSG(hwnd, WM_PAINT, OnPaint);
    HANDLE_DLG_MSG(hwnd, WM_CLOSE, OnClose);
    default:
      return FALSE;
  }
}
}  // namespace

int WINAPI wWinMain(
    HINSTANCE hinstance,
    HINSTANCE not_used,
    LPTSTR cmdline,
    int cmdshow) {

#ifdef DEBUG
  FILE* fp = nullptr;
  AllocConsole();
  _wfreopen_s(&fp, L"CONOUT$", L"w", stdout);
  _wfreopen_s(&fp, L"CONIN$", L"r", stdin);
#endif

  // The main dialog is opened.
  DialogBoxParam(
      hinstance,
      MAKEINTRESOURCE(IDD_DIALOG1),
      nullptr,
      &DialogProc,
      NULL);

#ifdef DEBUG
  FreeConsole();
#endif

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(not_used);
  UNREFERENCED_PARAMETER(cmdline);
  UNREFERENCED_PARAMETER(cmdshow);
  return 0;
}
