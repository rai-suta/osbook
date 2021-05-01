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
