#assumes an installation of the cross compiler at /opt/cross
#automated by 

CCPREFIX=~/opt/cross/bin/
ASM=~/opt/cross/bin/i686-elf-as
CXX:=~/opt/cross/bin/i686-elf-g++
CC:=i686-elf-gcc
ASL=iasl
BUILD := .build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
INCLUDE:= -Iinclude -IFEL/include
CXXFLAGS= --std=c++17 -ffreestanding -O3 -Wall -Wextra -fno-exceptions -fno-rtti -fshort-enums -Wno-attributes -w

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.cpp.o)
OBJECTS := $(OBJECTS:%.s=$(OBJ_DIR)/%.s.o)


all: test

dir:
	mkdir -p $(BUILD)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(APP_DIR)

kernel: dir
	@mkdir -p $(@D)
	$(ASM) assets/*.s -o $(OBJ_DIR)/asm1.o
	#nasm assets/*.asm -f elf -o $(OBJ_DIR)/asm2.o
	$(CXX) -nostdlib $(CXXFLAGS) $(INCLUDE) -c src/*.cpp
	$(CXX) -nostdlib $(CXXFLAGS) $(INCLUDE) -c src/lib/*.cpp
	mv ./*.o $(OBJ_DIR)
	~/opt/cross/bin/i686-elf-ld -T assets/linker.ld $(OBJ_DIR)/*.o -o $(APP_DIR)/kernel.bin


clean:
	rm build -rf

image: kernel
	grub2-file --is-x86-multiboot $(APP_DIR)/kernel.bin
	mkdir -p $(APP_DIR)/isodir/boot/grub
	cp $(APP_DIR)/kernel.bin $(APP_DIR)/isodir/boot/kernel.bin
	cp assets/grub.cfg $(APP_DIR)/isodir/boot/grub/grub.cfg
	grub2-mkrescue -o $(APP_DIR)/clinl.iso $(APP_DIR)/isodir

test: image
	qemu-system-x86_64 -hda $(APP_DIR)/clinl.iso

debug_run:
	qemu-system-i386 --cpu host -S -s -d guest_errors -cdrom $(APP_DIR)/clinl.iso &> $(BUILD)/klog &
	sleep 3
	gdb -w --eval-command="target remote localhost:1234" $(APP_DIR)/kernel.bin

