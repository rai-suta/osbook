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
