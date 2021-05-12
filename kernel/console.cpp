/**
 * @file console.cpp
 *
 * コンソール描画のプログラムを集めたファイル．
 */

#include "console.hpp"

#include <cstring>
#include "font.hpp"

// コンソール画面 - コンストラクタ
Console::Console(PixelWriter& writer,
    const PixelColor& fg_color, const PixelColor& bg_color)
    : writer_{writer}, fg_color_{fg_color}, bg_color_{bg_color},
      buffer_{}, cursor_row_{0}, cursor_column_{0} {
}

// コンソール画面 - 色を設定
void Console::SetFgColor(const PixelColor& fg_color) {
  fg_color_ = fg_color;
}

// コンソール画面 - 文字列を追加
void Console::PutString(const char* s) {
  while (*s) {
    if (*s == '\n') {
      // 改行
      Newline();
    } else if (cursor_column_ < kColumns - 1) {
      // 画面に文字列を描画、バッファに文字列を追加、カーソルを移動
      WriteAscii(writer_, 8 * cursor_column_, 16 * cursor_row_, *s, fg_color_);
      buffer_[cursor_row_][cursor_column_] = *s;
      ++cursor_column_;
    }
    ++s;
  }
}

// コンソール画面 - 改行
void Console::Newline() {
  cursor_column_ = 0;
  if (cursor_row_ < kRows - 1) {
    // 画面内ならカーソルを移動
    ++cursor_row_;
  } else {
    // 背景色でクリア
    for (int y = 0; y < 16 * kRows; ++y) {
      for (int x = 0; x < 8 * kColumns; ++x) {
        writer_.Write(x, y, bg_color_);
      }
    }
    // バッファ文字列の行をずらす、画面に文字列を表示
    for (int row = 0; row < kRows - 1; ++row) {
      memcpy(buffer_[row], buffer_[row + 1], kColumns + 1);
      WriteString(writer_, 0, 16 * row, buffer_[row], fg_color_);
    }
    memset(buffer_[kRows - 1], 0, kColumns + 1);
  }
}
