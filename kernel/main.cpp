/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたファイル．
 */

#include <cstdint>
#include <cstddef>
#include <cstdio>

#include "frame_buffer_config.hpp"
#include "graphics.hpp"
#include "font.hpp"
#include "console.hpp"

// ヒープを使用しない配置new
void* operator new(size_t size, void* buf) {
  return buf;
}

void operator delete(void* obj) noexcept {
}

// インスタンス アロケート用バッファ、配置newで使用する
char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_writer;

// コンソール画面
char console_buf[sizeof(Console)];
Console* console;

// コンソール画面へ書式指定文字列を出力する
int printk(const char* format, ...) {
  va_list ap;
  int result;
  char s[1024];

  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap);

  console->PutString(s);
  return result;
}

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
  // ピクセル形式に合わせて派生クラスのコンストラクタを実行
  switch (frame_buffer_config.pixel_format) {
    case kPixelRGBResv8BitPerColor:
      pixel_writer = new(pixel_writer_buf)
        RGBResv8BitPerColorPixelWriter{frame_buffer_config};
      break;
    case kPixelBGRResv8BitPerColor:
      pixel_writer = new(pixel_writer_buf)
        BGRResv8BitPerColorPixelWriter{frame_buffer_config};
      break;
  }

#if 0
  // フレームバッファを白で塗り物す
  for (int x = 0; x < frame_buffer_config.horizontal_resolution; ++x) {
    for (int y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
      pixel_writer->Write(x, y, {255, 255, 255});
    }
  }
#else
  for (int x = 0; x < frame_buffer_config.horizontal_resolution; ++x)
  for (int y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
    pixel_writer->Write(x, y, {0, 0, 0});
  }
#endif

  // コンソール画面
  console = new(console_buf) Console{*pixel_writer, {255, 255, 255}, {10, 10, 10}};
  printk("Hyper Operation System Ver1.0 for ALL LABORS\n");
  printk("(c)1999 Shinohara Heavy Industry)\n");
  printk("\n");

  printk("attach cd 01 /");
  printk("enter author password");
  printk("pass: E.HOBA");
  printk("\n");

  printk("Go to, let us go down, and there confound\n");
  printk("their language, that they may not understand\n");
  printk("one another's speech.\n");
  printk("\n");

  console->SetFgColor({255, 0, 0});
  char line[128];
  for (int i = 0, cur = 0; i < (80)/6; ++i)
    cur += sprintf(line+cur, "BABEL ");
  for (int i = 0; i < 16; ++i) {
    if (i%2)
      printk("%s\n", line);
    else
      printk(" %s\n", line);
  }

  while (1) __asm__("hlt");
}
