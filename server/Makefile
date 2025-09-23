mingw32 := i686-w64-mingw32-gcc
gcc32 := gcc

main_win:
	$(mingw32) main.c -o output.exe -lws2_32 -no-pie -O0 -Wno-format-truncation -fno-stack-protector

main_linux:
	gcc main.c -o output -no-pie -O0 -Wno-format-truncation -fno-stack-protector

clean:
	rm output*

run:
	./main.exe