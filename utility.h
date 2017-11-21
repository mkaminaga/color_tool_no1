  // @file utility.h
  // @brief Useful functions.
  // @author Mamoru Kaminaga
  // @date 2017-11-17
  // Copyright 2017 Mamoru Kaminaga
  // This program is provided with MIT license. See "LICENSE.md".
#ifndef UTILITY_H_
#define UTILITY_H_

#include <wchar.h>
#include <windows.h>
#include <stdint.h>

  // Message cracker is used for dialog messages with this macro function.
#define HANDLE_DLG_MSG(hwnd, msg, fn)\
  case (msg): return SetDlgMsgResult((hwnd), (msg), \
      HANDLE_##msg((hwnd), (wp), (lp), (fn)));

struct RGBVecotr {
  int r;
  int g;
  int b;
  RGBVecotr() : r(0), g(0), b(0) { }
  RGBVecotr(int r0, int g0, int b0) : r(r0), g(g0), b(b0) { }
};

template<class TYPE>
struct Vector2 {
  TYPE x;
  TYPE y;
  Vector2() : x(0), y(0) { }
  Vector2(TYPE x0, TYPE y0) : x(x0), y(y0) { }
};
typedef Vector2<int> Vector2n;
typedef Vector2<double> Vector2d;

enum FILTERINDEX {
  FILTERINDEX_COLOR_TEXT,
  FILTERINDEX_WIN_8BIT_BITMAP,
  FILTERINDEX_WIN_24BIT_BITMAP,
};

bool GetPaletteFileName(HWND hwnd, wchar_t* file_name);
bool GetExportFileName(HWND hwnd, wchar_t* file_name, FILTERINDEX* index);

bool CreateBitmapWin24(
    const wchar_t* file_name,
    int width,
    int height,
    const RGBVecotr* array,
    int array_size);

bool CreateBitmapWin8(
    const wchar_t* file_name,
    int width,
    int height,
    const RGBVecotr* colors,
    int color_num,
    const uint8_t* indeces,
    int index_num);

#endif  // UTILITY_H_
