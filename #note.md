# 第２章 EDK Ⅱ入門とメモリマップ

## 2.2 EDK Ⅱでハローワールド (osbook_day02a)
ソースコードを準備
```
~/workspace/osbook/MikanLoaderPkg/
  MikanLoader.dec
  MikanLoader.dsc
  Loader.inf
  Main.c
```

## ビルド対象へのシンボリックリンクを作成
```
$ cd ~/edk2
$ ln -s ~/workspace/osbook/MikanLoaderPkg ./
```

## edk をセットアップ、ビルド
```
$ cd ~/edk2
$ source edksetup.sh
$ code ./Conf/target.txt  # 表2.1 を参照して書き換える
$ build
```

## QEMU で実行
```
$ ~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi
```

## 2.5 メモリマップの取得 (osbook_day02b)

UEFI からメモリマップを取得して、ファイルに保存するプログラムを作成する

[2] UEFI Specification Version 2.8 (Errata B) (released June 2020)

> EFI_BOOT_SERVICES.GetMemoryMap() : Returns the current memory map.

> EFI_BOOT_SERVICES.OpenProtocol() : Queries a handle to determine if it supports a specified protocol. 
If the protocol is supported by the handle, it opens the protocol on behalf of the calling agent. 
This is an extended version of the EFI boot service EFI_BOOT_SERVICES.HandleProtocol(). 

> EFI_FILE_PROTOCOL.Open() : Opens a new file relative to the source file’s location.

Main.c へ追加

- MemoryMap : 'EFI_BOOT_SERVICES.GetMemoryMap()' から取得する情報を格納するデータ構造
- GetMemoryMap() : UEFI からメモリマップを取得
- GetMemoryTypeUnicode() : EFI_MEMORY_TYPE 型から要約文を取得
- SaveMemoryMap() : メモリマップをCSVファイルに変換して保存する
- OpenRootDir() : UEFI からルートディレクトリへのハンドルを取得

Loader.inf へ追加

```
[Protocols]
  gEfiLoadedImageProtocolGuid
  gEfiLoadFileProtocolGuid
  gEfiSimpleFileSystemProtocolGuid
```

## 2.7 メモリマップの確認

Main.c をビルド
```
$ cd ~/edk2
$ source edksetup.sh
$ build
```

QEMU で Loader.efi を起動
```
$ ~/osbook/devenv/run_qemu.sh  Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi
```

QEMU を起動するとファイルシステムイメージが disk.img として作成されるのでこれをマウントして中身を確認できる。
```
$ mkdir -p mnt
$ sudo mount -o loop disk.img mnt
$ ls mnt
```

プログラムによって作成されたメモリマップのCSVファイルの中身を確認
```
$ cat mnt/memmap
```

ディスクイメージをアンマウント
```
$ sudo umount mnt
```

# 第３章 画面表示の練習とブートローダ

## 3.3 初めてのカーネル (osbook_day03a)

"kernel/main.cpp - KernelMain()" をカーネルとして追加

main.cpp をビルド
```
$ cd ~/workspace/osbook/kernel
$ clang++ -O2 -Wall -g ... -c main.cpp
$ ld.lld --entry KernelMain ... -o kernel.elf main.o
```

"Main.c - UefiMain()" にてカーネル読み込み処理を追加
- カーネルのファイルハンドルを開く
- カーネルファイルの情報を取得
- カーネル格納先のメモリ領域をアロケートする
- ブートサービスを停止
- カーネルのエントリポイントをコール

Loader.inf へGUIDを追加
```
[Guids]
  gEfiFileInfoGuid
```

ブートローダのビルドとカーネルを起動
```
$ cd ~/edk2
$ build
$ ~/osbook/devenv/run_qemu.sh  Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi ~/workspace/osbook/kernel/kernel.elf
```

「EDK II Module Information (INF) File Specification」
 - 2.11 [Guids] Section (https://edk2-docs.gitbook.io/edk-ii-inf-specification/2_inf_overview/211_-guids-_section)

## 3.4 ブートローダからピクセルを描く (osbook_day03b)

"Main.c - OpenGOP()" にて GOP を取得する処理を追加

"Main.c - GetPixelFormatUnicode()" にて PixelFormat から文字列に変換する処理を追加

"Main.c - UefiMain()" にて画面描画処理を追加
 - GOP(Graphics Output Protocol) を取得して情報を出力
 - フレームバッファを白で塗りつぶす

## 3.5 カーネルからピクセルを描く (osbook_day03c)

"main.cpp - KernelMain()" にてフレームバッファに模様を書き込む

カーネルのビルドのためにビルドオプション($CPPFLAGS)を作成
カーネルのビルドを実施
```
$ cd ~/workspace/osbook/kernel
$ source ~/osbook/devenv/buildenv.sh 
$ echo $CPPFLAGS
$ clang++ $CPPFLAGS -O2 --target=x86_64-elf -fno-exceptions -ffreestanding -c main.cpp
$ ld.lld --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o
```

"Main.c - UefiMain()" にてカーネル呼び出し時にGOPを渡すように変更

ブートローダのビルドとカーネルを起動
```
$ cd ~/edk2
＜省略＞
```

## 3.6 エラー処理をしよう (osbook_day03d)

"Main.c" 下記関数の呼び出し箇所において、"EFI_ERROR" の返り値を判定する処理を追加
 - EFI_FILE_PROTOCOL.Open()
 - EFI_FILE_PROTOCOL.Write()
 - EFI_FILE_PROTOCOL.GetInfo()
 - EFI_BOOT_SERVICES.OpenProtocol()
 - EFI_BOOT_SERVICES.LocateHandleBuffer()
 - EFI_BOOT_SERVICES.AllocatePages()
 - GetMemoryMap() - EFI_BOOT_SERVICES.GetMemoryMap()
 - OpenRootDir() - EFI_SIMPLE_FILE SYSTEM_PROTOCOL.OpenVolume()
 - OpenGOP() - LocateHandleBuffer(), OpenProtocol()

"Main.c - Halt()" を追加
"while (1) __asm__("hlt");" にてプログラムを停止する

# 第４章 ピクセル描画とmake入門

## 4.1 make入門 (osbook_day04a)

"kernel/Makefile" を作成

make コマンドで kernel.elf をビルドする
```
$ cd ~/workspace/osbook/kernel
$ source ~/osbook/devenv/buildenv.sh 
$ make
```

## 4.2 ピクセルを自在に描く (osbook_day04b)

"frame_buffer_config.h" へフレームバッファの構成情報を格納するデータ構造を定義
 - enum PixelFormat
 - struct FrameBufferConfig

"Main.c - UefiMain()" にてカーネルへ渡すフレームバッファ情報を作成

"main.cpp" にてフレームバッファに書き込む処理を追加
 - WritePixel() : １つの点を描画

## 4.3 C++の機能を使って書き直す (osbook_day04c)

"main.cpp - WritePixel()" はピクセルを描画するたびにピクセル形式をチェックして書き込むため実行効率が悪い。よって、描画を管理するベースクラスを作成して、ピクセル形式ごとに処理を分けた派生クラスを作成する。
"main.cpp"
 - class PixelWriter
 - class RGBResv8BitPerColorPixelWriter : public PixelWriter
 - class BGRResv8BitPerColorPixelWriter : public PixelWriter

コンストラクタの実行にはnew演算子が必要だが、通常のnewはヒープメモリが必要となる。
作成中のOSにはヒープメモリの管理が未実装であるため、配置newを新規定義して、グローバルRAM領域へインスタンスを配置する。
"main.cpp"
 - void* operator new(size_t size, void* buf)
 - void operator delete(void* obj) noexcept
 - char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
 - PixelWriter* pixel_writer;

ベースクラス(PixelWrite)のポインタを使って、"pixel_format"に合わせた派生クラスのインスタンスを作成する。そのインスタンスを使ってフレームバッファへ描画する。
"main.cpp - KernelMain()"

## 4.5 ローダを改良する (osbook_day04d)

カーネルを読み込む処理(Main.c - UefiMain)においてカーネル用のメモリを確保する処理にバグがあるのでこれを修正する。
"readelf" コマンドを使って "kernel.elf" のプログラムヘッダを見てみると、３つあるLOADセグメントのうち、ファイルサイズ(FileSiz)とメモリサイズ(MemSiz)が不一致のセグメントがある。
初期値がつかないグローバル変数はファイルにそのメモリ領域が格納されないため、現状のELFファイルサイズからカーネルのメモリ確保する処理では、実際に使用するメモリ領域に対して不足する。
```
$ cd ~/workspace/osbook/kernel
$ readelf -l kernel.elf
Elf file type is EXEC (Executable file)
Entry point 0x101020
There are 5 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
...
  LOAD           0x0000000000000000 0x0000000000100000 0x0000000000100000
                 0x00000000000001a8 0x00000000000001a8  R      0x1000
  LOAD           0x0000000000001000 0x0000000000101000 0x0000000000101000
                 0x00000000000001b9 0x00000000000001b9  R E    0x1000
  LOAD           0x0000000000002000 0x0000000000102000 0x0000000000102000
                 0x0000000000000000 0x0000000000000018  RW     0x1000
...
```

"elf.hpp" にてファイルヘッダ(Elf64_Ehdr)とプログラムヘッダ(Elf64_Phdr) を追加
 - typedef struct {} Elf64_Ehdr
 - typedef struct {} Elf64_Phdr

 "Main.c" にてELFファイルヘッダからLOADセグメント情報を読み出して、セグメントを正しく初期化する処理を追加。
 - UefiMain()
 - CalcLoadAddressRange()
 - CopyLoadSegments()

# 第５章 文字表示とコンソールクラス

## 5.1 文字を書いてみる (osbook_day05a)

8x16ピクセルのフォントデータをuint8の配列データとして定義する
 - "main.cpp - kFontA[]"

フォントデータを描画する関数を追加
 - "main.cpp - WriteAscii()"

## 5.2 分割コンパイル (osbook_day05b)

カーネル(main.cpp)を機能別に分割する
 - graphics.cpp - 画像処理に関するコード
 - font.cpp - フォントに関するコード
 - main.cpp - メイン関数を格納するコード

 "Makefile" にソース(.cpp)が持つ依存関係をファイル(.d)に出力するためのパターンを追加
 - .%.d: %.cpp
 - depends:

## 5.3 フォントを増やそう (osbook_day05c)

Makefileへフォントデータを作成するためのルールを追加

全ASCIIコードのフォントを埋め込んだテキスト(hankaku.txt) からバイナリファイルを作成する
```
hankaku.bin: hankaku.txt
	../tools/makefont.py -o $@ $<
```

バイナリファイル(hankaku.bin) からリンク可能なオブジェクトファイルを作成する。
```
hankaku.o: hankaku.bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@
```

"font.cpp" にて "hankaku.o" への参照を追加
 - "_binary_hankaku_bin_start, _binary_hankaku_bin_end, _binary_hankaku_bin_size"
 - "GetFont()" - 文字コードからフォントデータを返す
 - "WriteAscii()" - "GetFont()" を使ってASCIIコードからフォントを描画する

## 5.4 文字列描画と sprintf() (osbook_day05d)

"font.cpp" にて文字列描画関数を追加
 - WriteString()

"newlib_support.c" にてsprintfに必要な関数を追加
 - sbrk()

 ## 5.5 コンソールクラス (osbook_day05e)

 "console.hpp", "console.cpp" コンソールクラスを追加、画面に表示する文字列をバッファに記録して、文字列が画面からあふれるときにスクロールさせる。
  - NewLine() - 改行
  - PutString() - 文字列の描画と画面のスクロール

## 5.6 printk() (osbook_day05f)

"main.cpp" に書式指定文字列を表示する機能を追加
 - "Console* console" - グローバル領域に配置newで Console を作成
 - "printk" - 可変長引数を受け取って、vsprintf で書式指定文字列を処理する

# 第6章 マウス入力とPCI

## 6.1 マウスカーソル (osbook_day06a)
画面上にタスクバーとマウスカーソルを描画する

### "graphics.cpp"
 - DrawRectangle() - 長方形の枠を描画
 - FillRectangle() - 塗りつぶした長方形を描画

### "main.cpp"
 - mouse_cursor_shape - マウスカーソルのビットマップデータ
 - KernelMain - タスクバーを描画、マウスカーソルを描画
