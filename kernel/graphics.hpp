#pragma once

#include "frame_buffer_config.hpp"

struct PixelColor {
  uint8_t r, g, b;
};

// ピクセル描画機能のためのベースクラス
class PixelWriter {
 public:
  PixelWriter(const FrameBufferConfig& config) : config_{config} {
  }
  virtual ~PixelWriter() = default;
  virtual void Write(int x, int y, const PixelColor& c) = 0;

 protected:
  uint8_t* PixelAt(int x, int y) {
    return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
  }

 private:
  const FrameBufferConfig& config_;
};

// ピクセルデータ形式"RGB 8bitColor"の派生クラス
class RGBResv8BitPerColorPixelWriter : public PixelWriter {
 public:
  using PixelWriter::PixelWriter;
  virtual void Write(int x, int y, const PixelColor& c) override;
};

// ピクセルデータ形式"BGR 8bitColor"の派生クラス
class BGRResv8BitPerColorPixelWriter : public PixelWriter {
 public:
  using PixelWriter::PixelWriter;
  virtual void Write(int x, int y, const PixelColor& c) override;
};

// 2次元ベクトルのデータ構造
template <typename T>
struct Vector2D {
  T x, y;
};

// 長方形の枠を描画
void DrawRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                   const Vector2D<int>& size, const PixelColor& c);

// 塗りつぶした長方形を描画
void FillRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                   const Vector2D<int>& size, const PixelColor& c);
