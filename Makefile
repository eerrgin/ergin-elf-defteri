CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -O2 -Isrc
.PHONY: all test clean
all: build/elf-defteri build/test_elf
build:
	mkdir -p build
build/elf-defteri: src/elf_note.c src/elf_facts.c src/elf_facts.h | build
	$(CC) $(CFLAGS) -o $@ src/elf_note.c src/elf_facts.c
build/test_elf: tests/test_elf.c src/elf_facts.c src/elf_facts.h | build
	$(CC) $(CFLAGS) -o $@ tests/test_elf.c src/elf_facts.c
test: all
	./build/test_elf
	printf '\\177ELF' > build/notelf
	./build/elf-defteri build/notelf; test $$? -eq 3
	cp /bin/true build/sample 2>/dev/null || cp /usr/bin/true build/sample
	./build/elf-defteri build/sample | grep -E 'nx_yigin='
clean:
	rm -rf build
