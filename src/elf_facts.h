#ifndef ERGIN_ELF_FACTS_H
#define ERGIN_ELF_FACTS_H
#include <stddef.h>
#include <stdint.h>
typedef struct {
    int elf64;
    int little;
    uint16_t type;
    uint16_t machine;
    uint64_t entry;
    int nx_stack;
    int saw_stack;
} ElfFacts;
int elf_facts(const unsigned char *b, size_t n, ElfFacts *out);
#endif
