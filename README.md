# ergin-elf-defteri

Ergin Kaplan. Savunmaci, salt okunur ELF baslik okuyucusu. Istismar, kabuk kodu veya uzak tarama yoktur. Yalnizca verilen dosyanin basligini yerelde okur.

## Ne bakilir

- Sihirli sayi, 32/64 bit, endian.
- `e_type` (REL / EXEC / DYN) ve `e_machine`.
- Giris adresi.
- `PT_GNU_STACK` program basliginda calistirma izni (`PF_X`) var mi. Yoksa yigin NX olarak isaretlenmis sayilir.

Bu, bir guvenlik arayuzunun altinda duran kontroldur: rozet degil, baslik alani.

## Sinir

RELRO, PIE yukleme adresi, sembol tablosu ve dinamik baglayici buraya dahil degil. Bilinmeyen bir ikili icin "guvenli" demez. Sadece olctugu alani soyler.

Arastirma notu: [NOT.md](NOT.md)

```bash
make test
```
