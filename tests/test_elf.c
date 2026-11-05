#include "elf_facts.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
static int fails;
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "fail %d %s\n", __LINE__, #c); fails++; } } while (0)
static void w16(unsigned char *p, uint16_t v) { p[0]=v&255; p[1]=v>>8; }
static void w32(unsigned char *p, uint32_t v) { p[0]=v; p[1]=v>>8; p[2]=v>>16; p[3]=v>>24; }
static void w64(unsigned char *p, uint64_t v) { w32(p,(uint32_t)v); w32(p+4,(uint32_t)(v>>32)); }
int main(void) {
    unsigned char b[128];
    ElfFacts f;
    memset(b, 0, sizeof b);
    b[0]=0x7f; b[1]='E'; b[2]='L'; b[3]='F'; b[4]=2; b[5]=1;
    w16(b+16, 3);
    w16(b+18, 62);
    w64(b+24, 0x401000);
    w64(b+32, 64);
    w16(b+54, 56);
    w16(b+56, 1);
    w32(b+64, 0x6474e551);
    w32(b+68, 6);
    CHECK(elf_facts(b, sizeof b, &f) == 0);
    CHECK(f.elf64 == 1 && f.little == 1);
    CHECK(f.type == 3 && f.machine == 62);
    CHECK(f.entry == 0x401000);
    CHECK(f.saw_stack == 1 && f.nx_stack == 1);
    w32(b+68, 7);
    CHECK(elf_facts(b, sizeof b, &f) == 0);
    CHECK(f.nx_stack == 0);
    b[0]=0;
    CHECK(elf_facts(b, sizeof b, &f) != 0);
    CHECK(elf_facts(b, 8, &f) != 0);
    if (fails) return 1;
    puts("ok");
    return 0;
}
