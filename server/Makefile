MINGW32 := i686-w64-mingw32-gcc
GCC32 := gcc -m32

bls_version = bls
defcon_version = defcon

# OS VERSION
ifeq ($(OS),Windows_NT)
	BLS_EXEC = $(bls_version).exe
	DEFCON_EXEC = $(defcon_version).exe
	COMPILE_CMD = $(MINGW32)

	# Windows needs WinSock2
	OS_CFLAGS = -lws2_32
else
	BLS_EXEC = $(bls_version)
	DEFCON_EXEC = $(defcon_version)
	COMPILE_CMD = $(GCC32)

	# Blank for Linux
	OS_CFLAGS =
endif

# SOURCES
BLS_SOURCES = $(bls_version).c $(bls_version).h
DEFCON_SOURCES = $(defcon_version).c $(defcon_version).h
CFLAGS = -no-pie -O0 -Wno-format-truncation -fno-stack-protector

# make bls
$(BLS_EXEC): $(BLS_SOURCES)
	$(COMPILE_CMD) $(BLS_SOURCES) -o ./output/$(BLS_EXEC) $(CFLAGS) $(OS_CFLAGS)

# make defcon
$(DEFCON_EXEC): $(DEFCON_SOURCES)
	$(COMPILE_CMD) $(DEFCON_SOURCES) -o ./output/$(DEFCON_EXEC) $(CFLAGS) $(OS_CFLAGS)

# make all
all: bls defcon

# make clean
clean:
	rm ./output/* || echo 'No Files to Delete. Continuing...'

# make run
run:
	./output/$(defcon_version)
