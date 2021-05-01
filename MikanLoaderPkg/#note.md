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
