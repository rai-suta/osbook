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

  // フレームバッファを白で塗り物す
  for (int x = 0; x < frame_buffer_config.horizontal_resolution; ++x) {
    for (int y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
      pixel_writer->Write(x, y, {10, 10, 10});
    }
  }

  // コンソール画面
  console = new(console_buf) Console{*pixel_writer, {255, 128, 0}, {10, 10, 10}};
  console->PutString("Genetic Lifeform and Disk Operationg System [Version 1.09]\n");
  console->PutString("Copyright (c) 1982 Aperture Science, Inc.\n");
  console->PutString("              .,-:;//;:=,\n");
  console->PutString("          . :H@@@MM@M#H/.,+;,\n");
  console->PutString("       ,/X+ +M@@M@MM=,-HMMM@X/,\n");
  console->PutString("     -+@MM; $M@@MH+-,;XMMMM@MMMM@+-\n");
  console->PutString("    ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/.\n");
  console->PutString("  ,MM@@MH ,@=             .---=-=:=,.\n");
  console->PutString("  =@#@@@MX.,                -HX$$%:;\n");
  console->PutString(" =-./@M@M$                   .;@MMMM@MM:\n");
  console->PutString(" X@/ -$MM/                    . +MM@@@M$\n");
  console->PutString(",@M@H: :@:                    . =X#@@@@-\n");
  console->PutString(",@@@MMX, .                    /H- ;@M@M=\n");
  console->PutString(".H@@@@M@+,                    MM+..$.\n");
  console->PutString(" /MMMM@MMH/.                  XM@MH; =;\n");
  console->PutString("  /%$XHH@$=              , .H@@@@MX,\n");
  console->PutString("   .=--------.           -H.,@@@@@MX,\n");
  console->PutString("   .MM@@@HHHXX$$$:$MMX =M@@MM\n");
  console->PutString("     =XMMM@MM@MM#H;,-+HMM@M+ /MMMX=\n");
  console->PutString("       =@M@M#@$-.=$@MM@@@M; M=\n");
  console->PutString("         ,:+$+-,/H#MMMMMMM@= =,\n");
  console->PutString("               =++%%+/:-.\n");
  console->PutString("\n");
  console->PutString("GLaDOS>\n");

  while (1) __asm__("hlt");
}
