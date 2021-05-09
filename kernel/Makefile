TARGET = kernel.elf
OBJS = main.o

CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
            -fno-exceptions -fno-rtti -std=c++17
LDFLAGS  += --entry KernelMain -z norelro --image-base 0x100000 --static

SHELL = /bin/bash

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o

kernel.elf: $(OBJS) Makefile buildenv
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS)

%.o: %.cpp Makefile buildenv
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $<

buildenv:
	$(eval CPPFLAGS := $(shell source ~/osbook/devenv/buildenv.sh && echo $${CPPFLAGS}) $(CPPFLAGS))
	$(eval LDFLAGS := $(shell source ~/osbook/devenv/buildenv.sh && echo $${LDFLAGS}) $(LDFLAGS))
