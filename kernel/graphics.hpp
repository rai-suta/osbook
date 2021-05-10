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
