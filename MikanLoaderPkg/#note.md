# ��Q�� EDK �U����ƃ������}�b�v

## 2.2 EDK �U�Ńn���[���[���h (osbook_day02a)
�\�[�X�R�[�h������
```
~/workspace/osbook/MikanLoaderPkg/
  MikanLoader.dec
  MikanLoader.dsc
  Loader.inf
  Main.c
```

## �r���h�Ώۂւ̃V���{���b�N�����N���쐬
```
$ cd ~/edk2
$ ln -s ~/workspace/osbook/MikanLoaderPkg ./
```

## edk ���Z�b�g�A�b�v�A�r���h
```
$ cd ~/edk2
$ source edksetup.sh
$ code ./Conf/target.txt  # �\2.1 ���Q�Ƃ��ď���������
$ build
```

## QEMU �Ŏ��s
```
$ ~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi
```

## 2.5 �������}�b�v�̎擾 (osbook_day02b)

UEFI ���烁�����}�b�v���擾���āA�t�@�C���ɕۑ�����v���O�������쐬����

[2] UEFI Specification Version 2.8 (Errata B) (released June 2020)

> EFI_BOOT_SERVICES.GetMemoryMap() : Returns the current memory map.

> EFI_BOOT_SERVICES.OpenProtocol() : Queries a handle to determine if it supports a specified protocol. 
If the protocol is supported by the handle, it opens the protocol on behalf of the calling agent. 
This is an extended version of the EFI boot service EFI_BOOT_SERVICES.HandleProtocol(). 

> EFI_FILE_PROTOCOL.Open() : Opens a new file relative to the source file�fs location.

Main.c �֒ǉ�

- MemoryMap : 'EFI_BOOT_SERVICES.GetMemoryMap()' ����擾��������i�[����f�[�^�\��
- GetMemoryMap() : UEFI ���烁�����}�b�v���擾
- GetMemoryTypeUnicode() : EFI_MEMORY_TYPE �^����v�񕶂��擾
- SaveMemoryMap() : �������}�b�v��CSV�t�@�C���ɕϊ����ĕۑ�����
- OpenRootDir() : UEFI ���烋�[�g�f�B���N�g���ւ̃n���h�����擾

Loader.inf �֒ǉ�

```
[Protocols]
  gEfiLoadedImageProtocolGuid
  gEfiLoadFileProtocolGuid
  gEfiSimpleFileSystemProtocolGuid
```

## 2.7 �������}�b�v�̊m�F

Main.c ���r���h
```
$ cd ~/edk2
$ source edksetup.sh
$ build
```

QEMU �� Loader.efi ���N��
```
$ ~/osbook/devenv/run_qemu.sh  Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi
```

QEMU ���N������ƃt�@�C���V�X�e���C���[�W�� disk.img �Ƃ��č쐬�����̂ł�����}�E���g���Ē��g���m�F�ł���B
```
$ mkdir -p mnt
$ sudo mount -o loop disk.img mnt
$ ls mnt
```

�v���O�����ɂ���č쐬���ꂽ�������}�b�v��CSV�t�@�C���̒��g���m�F
```
$ cat mnt/memmap
```

�f�B�X�N�C���[�W���A���}�E���g
```
$ sudo umount mnt
```
