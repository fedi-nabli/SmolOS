LD=ld86
INC = -I./src
FLAGS=-0 -I./src -ansi -c 

OBJECTS= ./build/kernel.o ./build/disk/disk.asm.o ./build/display/display.asm.o ./build/display/display.o ./build/memory/memory.o ./build/memory/heap/heap.o ./build/memory/heap/kheap.o ./build/string/string.o

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

./build/display/display.asm.o: ./src/display/display.asm
	nasm -f as86 ./src/display/display.asm -o ./build/display/display.asm.o

./build/display/display.o: ./src/display/display.c ./src/display/display.h
	bcc $(FLAGS) $(INC) -I./src/display ./src/display/display.c -o ./build/display/display.o

./build/memory/memory.o: ./src/memory/memory.c ./src/memory/memory.h
	bcc $(FLAGS) $(INC) -I./src/memory ./src/memory/memory.c -o ./build/memory/memory.o

./build/memory/heap/heap.o: ./src/memory/heap/heap.c ./src/memory/heap/heap.h
	bcc $(FLAGS) $(INC) -I./src/memory/heap ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o

./build/memory/heap/kheap.o: ./src/memory/heap/heap.c ./src/memory/heap/kheap.h
	bcc $(FLAGS) $(INC) -I./src/memory/heap ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o

./build/string/string.o: ./src/string/string.c ./src/string/string.h
	bcc $(FLAGS) $(INC) -I./src/string ./src/string/string.c -o ./build/string/string.o

./bin/kernel.bin: ${OBJECTS} 
	$(LD) -d -M ${OBJECTS} -L/usr/lib/bcc/ -lc -o ./bin/kernel.bin

clean:
	rm -rf ./bin/boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin
	rm -rf ${OBJECTS}