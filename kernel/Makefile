TARGET = kernel.elf
OBJS = main.o graphics.o font.o
DEPENDS = $(join $(dir $(OBJS)),$(addprefix .,$(notdir $(OBJS:.o=.d))))

CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
            -fno-exceptions -fno-rtti -std=c++17
LDFLAGS  += --entry KernelMain -z norelro --image-base 0x100000 --static

SHELL = /bin/bash

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o

.PHONY: buildenv
buildenv:
	$(eval CPPFLAGS := $(shell source ~/osbook/devenv/buildenv.sh && echo $${CPPFLAGS}) $(CPPFLAGS))
	$(eval LDFLAGS := $(shell source ~/osbook/devenv/buildenv.sh && echo $${LDFLAGS}) $(LDFLAGS))

kernel.elf: $(OBJS) Makefile buildenv
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS)

%.o: %.cpp Makefile buildenv
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $<

.%.d: %.cpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.cpp=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@

.PHONY: depends
depends:
	$(MAKE) $(DEPENDS)

-include $(DEPENDS)
