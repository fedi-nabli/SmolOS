LD=ld86

FLAGS=-0 -I./src -ansi -c 

OBJECTS= ./build/kernel.o ./build/disk/disk.asm.o

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/kernel.o: ./src/kernel.c ./src/kernel.h
	bcc $(FLAGS) ./src/kernel.c -o ./build/kernel.o

./build/disk/disk.asm.o: ./src/disk/disk.asm
	nasm -f as86 ./src/disk/disk.asm -o ./build/disk/disk.asm.o

./bin/kernel.bin: ${OBJECTS} 
	$(LD) -d -M ${OBJECTS} -L/usr/lib/bcc/ -lc -o ./bin/kernel.bin

clean:
	rm -rf ./bin/boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin
	rm -rf ${OBJECTS}