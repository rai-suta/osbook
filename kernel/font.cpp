/**
 * @file font.cpp
 *
 * フォント描画のプログラムを集めたファイル.
 */

#include "font.hpp"

// フォントデータ(hankaku.o)への外部参照
extern const uint8_t _binary_hankaku_bin_start;
extern const uint8_t _binary_hankaku_bin_end;
extern const uint8_t _binary_hankaku_bin_size;

// 文字コードからフォントデータを返す
const uint8_t* GetFont(char c) {
  auto index = 16 * static_cast<unsigned int>(c);
  if (index >= reinterpret_cast<uintptr_t>(&_binary_hankaku_bin_size)) {
    return nullptr;
  }
  return &_binary_hankaku_bin_start + index;
}

// 画面にASCII文字を描画する
void WriteAscii(PixelWriter& writer, int x, int y, char c, const PixelColor& color) {
  const uint8_t* font = GetFont(c);
  if (font == nullptr) {
    return;
  }
  for (int dy = 0; dy < 16; ++dy) {
    for (int dx = 0; dx < 8; ++dx) {
      if ((font[dy] << dx) & 0x80u) {
        writer.Write(x + dx, y + dy, color);
      }
    }
  }
}

// 画面に文字列を描画する
void WriteString(PixelWriter& writer, int x, int y, const char* s, const PixelColor& color) {
  for (int i = 0; s[i] != '\0'; ++i) {
    WriteAscii(writer, x + 8 * i, y, s[i], color);
  }
}
