# Not — yigin calistirilabilirligi neden bir alan

Tarih: 2026-11-05. Yazan: Ergin Kaplan. Denek: kendi urettigi baslik ve sistemdeki yerel ikili. Ag yok.

## Soru

Bir dosyanin korumali gorunup gorunmedigine, pencere metnine bakmadan nereden karar verilir?

## Yontem

ELF program basliklari dosyanin icinde, yukleme tarifesidir. `PT_GNU_STACK` (0x6474e551) yiginin izinlerini tasir. `p_flags` icinde `PF_X` (1) varsa yigin calistirilabilirdir. Yoksa cekirdek, yigin sayfalarini calistirmadan esler. Buna NX denir.

Test, elle kurulmus 64-bit little-endian bir baslik kullanir. Boylece iddia, canli sistemin tesadufi ciktisina bagli kalmaz. Ikinci adimda ayni baslikta bayrak 6'dan 7'ye cekilir ve okuyucunun nx hayir demesi beklenir.

## Ne ise yarar

Eski bir ikili veya kendi derledigim bir deneme, yigini yanlislikla calistirilabilir biraktiysa bu arac onu tek satirda soyler. Karar, antivirus skoruna degil, bayragin kendisine dayanir.

## Ne ise yaramaz

Baslik yalan soyleyebilir; yuklenen surec sonradan `mprotect` ile izni degistirebilir. Bu okuyucu sureci baslatmaz, o yuzden calisma anini gormez. Bunu kasitli olarak yapmiyorum: calistirma, inceleme sinirinin disina cikar.
