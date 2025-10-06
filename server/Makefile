mingw32 := i686-w64-mingw32-gcc
gcc32 := gcc -m32

bls_version = bls
defcon_version = defcon

windows:
	$(mingw32) $(bls_version).c -o ./output/$(bls_version).exe -lws2_32 -no-pie -O0 -Wno-format-truncation -fno-stack-protector
	$(mingw32) $(defcon_version).c -o ./output/$(defcon_version).exe -lws2_32 -no-pie -O0 -Wno-format-truncation -fno-stack-protector

linux:
	$(gcc32) $(bls_version).c -o ./output/$(bls_version) -no-pie -O0 -Wno-format-truncation -fno-stack-protector
	$(gcc32) $(defcon_version).c -o ./output/$(defcon_version) -no-pie -O0 -Wno-format-truncation -fno-stack-protector

all: linux windows

clean:
	rm ./output/*

run:
	./$(defcon_version)
