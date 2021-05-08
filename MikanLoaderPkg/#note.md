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

「EDK II Module Information (INF) File Specification」
 - 2.11 [Guids] Section (https://edk2-docs.gitbook.io/edk-ii-inf-specification/2_inf_overview/211_-guids-_section)

## 3.4 ブートローダからピクセルを描く (osbook_day03b)

"Main.c - OpenGOP()" にて GOP を取得する処理を追加

"Main.c - GetPixelFormatUnicode()" にて PixelFormat から文字列に変換する処理を追加

"Main.c - UefiMain()" にて画面描画処理を追加
 - GOP(Graphics Output Protocol) を取得して情報を出力
 - フレームバッファを白で塗りつぶす
