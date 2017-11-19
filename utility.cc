  // @file utility.h
  // @brief Useful functions.
  // @author Mamoru Kaminaga
  // @date 2017-11-17
  // Copyright 2017 Mamoru Kaminaga
  // This program is provided with MIT license. See "LICENSE.md".
#include <assert.h>
#include <wchar.h>
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <vector>

#define BITMAP_FILE_HEADER_SIZE   (14)
#define BITMAP_INFO_HEADER_SIZE   (40)
#define BITMAP_HEADER_SIZE  (BITMAP_FILE_HEADER_SIZE + BITMAP_INFO_HEADER_SIZE)

#include "./utility.h"

bool GetPaletteFileName(HWND hwnd, wchar_t* file_name) {
  assert(file_name);

  // The color palette file name is acquired.
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrTitle = L"Open color palette";
  ofn.lpstrFilter = L"Color palette(*.txt)\0*.txt\0\0";
  ofn.lpstrFile = file_name;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrInitialDir = L"C:\\";
  ofn.lpstrDefExt = L"*.txt";
  ofn.Flags = OFN_FILEMUSTEXIST;
  if (GetOpenFileName(&ofn) == 0) {
    return false;
  }

#ifdef DEBUG
  wprintf(L"Load: %s\n", file_name);
#endif

  return true;
}
bool GetExportFileName(HWND hwnd, wchar_t* file_name) {
  assert(file_name);

  // The file name is acquired.
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrTitle = L"Export data";
  ofn.lpstrFilter = L"Bitmap 24bit(*.bmp)\0*.bmp\0\0";
  ofn.lpstrFile = file_name;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrInitialDir = L"C:\\";
  ofn.lpstrDefExt = L"*.edg";
  if (GetSaveFileName(&ofn) == 0) {
    return false;
  }

#ifdef DEBUG
  wprintf(L"Save: %s\n", file_name);
#endif

  return true;
}
bool CreateBitmapFile(
    const wchar_t* file_name,
    int width,
    int height,
    const RGBVecotr* array,
    int array_size) {
  assert(file_name);
  assert(array);

  // Width must be multiple of 4.
  uint8_t bitmap_header[BITMAP_HEADER_SIZE] = {0};

  // The file type.
  bitmap_header[0] = L'B';
  bitmap_header[1] = L'M';

  // The file size
  int byte_width = width * 3 + width % 4;  // RGB + Padding.
  uint32_t file_size = byte_width * height + BITMAP_HEADER_SIZE;
  memcpy_s(&bitmap_header[2], 4, &file_size, 4);

  // The offset.
  uint32_t offset_to_image = 54;
  memcpy_s(&bitmap_header[10], 4, &offset_to_image, 4);

  // The info header size.
  uint32_t info_header_size = 40;
  memcpy_s(&bitmap_header[14], 4, &info_header_size, 4);

  // The image width and height.
  memcpy_s(&bitmap_header[18], 4, &width, 4);
  memcpy_s(&bitmap_header[22], 4, &height, 4);

  // The number of planed.
  int plane_num = 1;
  memcpy_s(&bitmap_header[26], 4, &plane_num, 4);

  // The bit number.
  int bit_number = 24;
  memcpy_s(&bitmap_header[28], 4, &bit_number, 4);

  // The number of pixels of the image.
  int image_byte_num = width * height * 3;
  memcpy_s(&bitmap_header[34], 4, &image_byte_num, 4);

  // Image is created form RGB data array.
  std::vector<uint8_t> image(byte_width * height);
  int index = 0;
  int pixel_id = 0;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      index = (height - i - 1) * byte_width + j * 3;
      pixel_id = i * width + j;
      if (pixel_id >= array_size) {
        return false;
      }
      image[index] = static_cast<BYTE>(array[pixel_id].b);
      image[index + 1] = static_cast<BYTE>(array[pixel_id].g);
      image[index + 2] = static_cast<BYTE>(array[pixel_id].r);
    }
  }

  // The file is written in binary mode.
  FILE* fp = nullptr;
  _wfopen_s(&fp, file_name, L"wb");
  if (fp == nullptr) return false;
  fwrite(bitmap_header, BITMAP_HEADER_SIZE, 1, fp);
  fwrite(image.data(), byte_width * width, 1, fp);
  fclose(fp);

  return true;
}
