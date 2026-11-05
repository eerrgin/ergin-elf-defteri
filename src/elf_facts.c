#include "elf_facts.h"
#include <string.h>
#include <stdint.h>
#define PT_GNU_STACK 0x6474e551u
#define PF_X 1u
static uint16_t ru16(const unsigned char *p, int le) {
    return le ? (uint16_t)(p[0] | (p[1] << 8)) : (uint16_t)((p[0] << 8) | p[1]);
}
static uint32_t ru32(const unsigned char *p, int le) {
    return le ? (uint32_t)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24))
              : (uint32_t)((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
}
static uint64_t ru64(const unsigned char *p, int le) {
    if (!le) {
        uint64_t v = 0; int i;
        for (i = 0; i < 8; i++) v = (v << 8) | p[i];
        return v;
    }
    return (uint64_t)ru32(p, 1) | ((uint64_t)ru32(p + 4, 1) << 32);
}
int elf_facts(const unsigned char *b, size_t n, ElfFacts *out) {
    int elf64, le;
    uint64_t phoff;
    uint16_t phentsize, phnum, i;
    memset(out, 0, sizeof *out);
    out->nx_stack = -1;
    if (n < 64) return -1;
    if (b[0] != 0x7f || b[1] != 'E' || b[2] != 'L' || b[3] != 'F') return -1;
    if (b[4] != 1 && b[4] != 2) return -1;
    if (b[5] != 1 && b[5] != 2) return -1;
    elf64 = b[4] == 2;
    le = b[5] == 1;
    out->elf64 = elf64;
    out->little = le;
    out->type = ru16(b + 16, le);
    out->machine = ru16(b + 18, le);
    if (elf64) {
        out->entry = ru64(b + 24, le);
        phoff = ru64(b + 32, le);
        phentsize = ru16(b + 54, le);
        phnum = ru16(b + 56, le);
    } else {
        out->entry = ru32(b + 24, le);
        phoff = ru32(b + 28, le);
        phentsize = ru16(b + 42, le);
        phnum = ru16(b + 44, le);
    }
    if (phnum > 0 && (phoff > n || phentsize < 8 || phnum > 128 || phentsize > 1024)) return -1;
    if (phnum > 0 && phoff + (size_t)phentsize * phnum > n) return -1;
    for (i = 0; i < phnum; i++) {
        const unsigned char *ph = b + phoff + (size_t)i * phentsize;
        uint32_t ptype = ru32(ph, le);
        uint32_t flags;
        if (ptype != PT_GNU_STACK) continue;
        out->saw_stack = 1;
        flags = elf64 ? ru32(ph + 4, le) : ru32(ph + 24, le);
        out->nx_stack = (flags & PF_X) ? 0 : 1;
    }
    return 0;
}
