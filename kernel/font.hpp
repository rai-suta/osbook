#pragma once

#include <cstdint>
#include "graphics.hpp"

// 画面にASCII文字を描画する
void WriteAscii(PixelWriter& writer, int x, int y, char c, const PixelColor& color);
