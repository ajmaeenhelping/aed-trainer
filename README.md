# Mini AED Trainer (Arduino UNO) - Versi 2

Projek ini ialah alat latihan AED (Automated External Defibrillator) yang dibina menggunakan Arduino UNO, LCD 16x2, 3 butang tekan, 2 LED, dan satu buzzer.

## Fail

- `final_v1.ino` — alat latihan mod terus (direct-mode) yang pertama. Ia mempunyai mod CPR, Rescue Breath, Shock Only, dan pengendalian awal dua butang, tetapi butang shock kuning serta mod gabungan dua butang masih tidak stabil.
- `final_v2.ino` — alat latihan yang telah dikemas kini dengan pengendalian butang yang stabil, mod Shock Only yang khusus, dan pengesanan dua butang Merah+Biru yang tepat untuk mod CPR 130:1.

## Apa yang berubah dalam `final_v2.ino`

- Butang shock kuning kini menggunakan `INPUT_PULLUP`, jadi ia tidak terapung (floating) dan tidak akan terpicu dengan sendiri.
- Shock hanya diberikan apabila pengguna menekan butang kuning semasa arahan shock dipaparkan.
- Fungsi `runShock()` ditambah supaya Mod 3 menjadi laluan Shock Only yang sebenar.
- Tekan Merah + Biru serentak untuk masuk ke mod CPR 130:1 dengan boleh dipercayai.
- Panggilan shock automatik selepas `advice()` telah dibuang.
- Logik dua butang dikendalikan dengan selamat sebelum semakan butang tunggal.

**Peta mod:**
- `MERAH` = CPR biasa.
- `BIRU` = Rescue Breath (bantuan pernafasan).
- `KUNING` = Shock Only.
- `MERAH + BIRU` = CPR 130:1.

## Sambungan pin Arduino

### Butang

Semua butang menggunakan `INPUT_PULLUP`, jadi satu hujung setiap butang disambung ke pin Arduino dan hujung yang satu lagi disambung ke `GND`.

- Butang `MERAH` (RED): `D13`
- Butang `BIRU` (BLUE): `D12`
- Butang `KUNING` (YELLOW): `D10`

### LCD (16x2)

- `RS` = `D11`
- `E` = `D9`
- `D4` = `D6`
- `D5` = `D5`
- `D6` = `D4`
- `D7` = `D3`
- `VSS` = `GND`
- `VDD` = `5V`
- `V0` = `GND` (kontras ditetapkan rendah)
- `RW` = `GND`
- `A` = `5V` (lampu latar / backlight)
- `K` = `GND`

### Output

- Anod LED Merah = `D2`
- Anod LED Hijau = `D7`
- Katod LED = `GND` melalui perintang (resistor)
- Positif buzzer = `D8`
- Negatif buzzer = `GND`

> Nota: `D1` ialah pin TX Arduino, jadi reka bentuk ini mengelak menggunakannya untuk GPIO.

## Ringkasan pendawaian (wiring)

1. Letakkan setiap butang tekan supaya ia menyambungkan satu pin ke pin input Arduino dan satu lagi ke `GND`.
2. Gunakan `INPUT_PULLUP` dalam sketch. Butang membaca `HIGH` apabila dilepaskan dan `LOW` apabila ditekan.
3. Sambungkan LCD dalam mod 4-bit menggunakan peta pin di atas.
4. Sambungkan LED melalui perintang 220Ω ke `D2` dan `D7`, dengan terminal satu lagi ke `GND`.
5. Sambungkan buzzer ke `D8` dan `GND`.

## Cara guna `final_v2.ino`

1. Muat naik `final_v2.ino` ke Arduino UNO.
2. Hidupkan papan UNO.
3. Tekan:
   - `MERAH` untuk mod CPR
   - `BIRU` untuk mod Rescue Breath
   - `KUNING` untuk mod Shock Only
   - `MERAH + BIRU` serentak untuk mod CPR 130:1
4. Semasa CPR, tekan `KUNING` apabila diminta untuk memberi shock.

## Perbandingan mod: `final_v1.ino` lawan `final_v2.ino`

- `final_v1.ino`
  - Butang shock kuning menggunakan `INPUT` dan boleh terapung, menyebabkan pengaktifan palsu.
  - Shock masih dipanggil secara automatik selepas mesej nasihat (advice).
  - Pengesanan dua butang Merah+Biru untuk CPR 130:1 tidak boleh dipercayai.

- `final_v2.ino`
  - Butang shock kuning menggunakan `INPUT_PULLUP` dan stabil.
  - Shock hanya berlaku apabila kuning ditekan semasa arahan shock.
  - Terdapat mod `runShock()` khusus untuk Shock Only.
  - Pengesanan Merah+Biru untuk CPR 130:1 boleh dipercayai.

## Nota

- Sketch ini bergantung pada pendawaian `INPUT_PULLUP`: pin butang → butang → GND.
- Jangan gunakan pin TX/RX Arduino untuk LED atau butang.
