MINGW32 := i686-w64-mingw32-gcc
GCC32 := gcc -m32

bls_version = bls
defcon_version = defcon
bronze_version = bronze

VERSION ?= defcon

# OS VERSION
ifeq ($(OS),Windows_NT)
	BLS_EXEC = $(bls_version).exe
	DEFCON_EXEC = $(defcon_version).exe
	BRONZE_EXEC = $(bronze_version).exe
	COMPILE_CMD = $(MINGW32)

    # Windows needs WinSock2
	OS_CFLAGS = -lws2_32
else
	BLS_EXEC = $(bls_version)
	DEFCON_EXEC = $(defcon_version)
	BRONZE_EXEC = $(bronze_version)
	COMPILE_CMD = $(GCC32)

    # Blank for Linux
	OS_CFLAGS =
endif

ifeq ($(VERSION),bls)
    RUN_EXEC_VAR = $(BLS_EXEC)
else
    ifeq ($(VERSION),bronze)
        RUN_EXEC_VAR = $(BRONZE_EXEC)
    
    else
        RUN_EXEC_VAR = $(DEFCON_EXEC)
    endif

endif

# SOURCES
BLS_SOURCES = $(bls_version).c $(bls_version).h
DEFCON_SOURCES = $(defcon_version).c $(defcon_version).h
BRONZE_SOURCES = $(bronze_version).c
CFLAGS = -no-pie -O0 -w -fno-stack-protector

# PHONY TARGETS
.PHONY: all clean run bls defcon service bronze

# make bls
$(BLS_EXEC): $(BLS_SOURCES)
	$(COMPILE_CMD) $(BLS_SOURCES) -o ./output/$(BLS_EXEC) $(CFLAGS) $(OS_CFLAGS)

# make defcon
$(DEFCON_EXEC): $(DEFCON_SOURCES)
	$(COMPILE_CMD) $(DEFCON_SOURCES) -o ./output/$(DEFCON_EXEC) $(CFLAGS) $(OS_CFLAGS)

# make bronze
$(BRONZE_EXEC): $(BRONZE_SOURCES)
	$(COMPILE_CMD) $(BRONZE_SOURCES) -o ./output/$(BRONZE_EXEC) $(CFLAGS) $(OS_CFLAGS)

service:
	$(COMPILE_CMD) service.c -o ./output/service

# make all
all: bls defcon service

# make clean
clean:
	@echo 'Cleaning (ignore errors)...'
	rm ./output/* || true
	rm log.txt || true

# make run VERSION=<version>
run: $(VERSION)
	@echo "--- Running D&D Version: $(VERSION) (./output/$(RUN_EXEC_VAR)) ---"
	./output/$(RUN_EXEC_VAR)
