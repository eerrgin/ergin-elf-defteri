#include "elf_facts.h"
#include <stdio.h>
#include <stdlib.h>
static const char *etype(uint16_t t) {
    switch (t) {
        case 2: return "EXEC";
        case 3: return "DYN";
        case 1: return "REL";
        default: return "DIGER";
    }
}
int main(int argc, char **argv) {
    FILE *f; unsigned char *buf; long sz; ElfFacts facts;
    if (argc != 2) { fprintf(stderr, "kullanim: %s <elf>\n", argv[0]); return 2; }
    f = fopen(argv[1], "rb");
    if (!f) { perror(argv[1]); return 1; }
    if (fseek(f, 0, SEEK_END) != 0) return 1;
    sz = ftell(f);
    if (sz < 0 || sz > 32 * 1024 * 1024) { fclose(f); return 1; }
    rewind(f);
    buf = malloc((size_t)sz);
    if (!buf || fread(buf, 1, (size_t)sz, f) != (size_t)sz) { fclose(f); free(buf); return 1; }
    fclose(f);
    if (elf_facts(buf, (size_t)sz, &facts) != 0) {
        fprintf(stderr, "ELF degil veya baslik tutarsiz\n");
        free(buf); return 3;
    }
    printf("sinif=%s endian=%s tur=%s makine=%u giris=0x%llx nx_yigin=%s\n",
           facts.elf64 ? "64" : "32", facts.little ? "little" : "big", etype(facts.type),
           facts.machine, (unsigned long long)facts.entry,
           !facts.saw_stack ? "bilinmiyor" : (facts.nx_stack ? "evet" : "hayir"));
    free(buf);
    return 0;
}
