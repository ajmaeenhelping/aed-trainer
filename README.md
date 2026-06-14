# Mini AED Trainer (Arduino UNO) - Version 2

This project is a simulated AED trainer using an Arduino UNO, a 16x2 LCD, 3 pushbuttons, 2 LEDs, and a buzzer.

## Files

- `final_v1.ino` — first direct-mode trainer. It has CPR, Rescue Breath, Shock Only, and early dual-button handling, but the Yellow shock button and combined-button mode are not robust.
- `final_v2.ino` — updated trainer with stable button handling, dedicated Shock Only mode, and proper Red+Blue dual-button detection for CPR 130:1.
- `diagram.json` — Wokwi board layout for the Arduino, LCD, buttons, LEDs, and buzzer.

## What changed in `final_v2.ino`

- Yellow shock button now uses `INPUT_PULLUP`, so it does not float and trigger on its own.
- Shock is delivered only when the user presses the Yellow button during the shock prompt.
- `runShock()` was added so Mode 3 is a true Shock Only path.
- Red + Blue pressed together reliably enters the CPR 130:1 mode.
- Automatic shock calls after `advice()` were removed.
- Dual-button logic is handled safely before individual button checks.

**Ringkas dalam Bahasa Melayu:**
- Butang kuning hanya aktif untuk shock apabila ditekan semasa arahan shock.
- Tekan R + B serentak untuk masuk mod CPR 130:1.
- Mod 3 kini mod Shock Only yang berasingan.

**Peta mod (Bahasa Melayu):**
- `MERAH` = CPR biasa.
- `BIRU` = Rescue Breath.
- `KUNING` = Shock Only.
- `MERAH + BIRU` = CPR 130:1.

## Arduino pin connections

### Buttons

All buttons use `INPUT_PULLUP`, so one side of each button goes to the Arduino pin and the other side goes to `GND`.

- `RED` button: `D13`
- `BLUE` button: `D12`
- `YELLOW` button: `D10`

### LCD (16x2)

- `RS` = `D11`
- `E` = `D9`
- `D4` = `D6`
- `D5` = `D5`
- `D6` = `D4`
- `D7` = `D3`
- `VSS` = `GND`
- `VDD` = `5V`
- `V0` = `GND` (contrast fixed low)
- `RW` = `GND`
- `A` = `5V` (backlight)
- `K` = `GND`

### Outputs

- Red LED anode = `D2`
- Green LED anode = `D7`
- LED cathodes = `GND` through resistors
- Buzzer positive = `D8`
- Buzzer negative = `GND`

> Note: `D1` is the Arduino TX pin, so the design avoids using it for GPIO.

## Wiring summary

1. Place each pushbutton so it connects one pin to the Arduino input pin and the other to `GND`.
2. Use `INPUT_PULLUP` in the sketch. Buttons read `HIGH` when released and `LOW` when pressed.
3. Connect the LCD in 4-bit mode using the pin mapping above.
4. Connect LEDs through 220Ω resistors to `D2` and `D7`, with the other terminal to `GND`.
5. Connect the buzzer to `D8` and `GND`.

## How to use `final_v2.ino`

1. Upload `final_v2.ino` to the Arduino UNO.
2. Power the UNO.
3. Press:
   - `RED` for CPR mode
   - `BLUE` for Rescue Breath mode
   - `YELLOW` for Shock Only mode
   - `RED + BLUE` together for the CPR 130:1 mode
4. During CPR, press `YELLOW` when prompted to apply shock.

**Cara guna (Bahasa Melayu):**
1. Muat naik `final_v2.ino` ke Arduino UNO.
2. Hidupkan papan UNO.
3. Tekan:
   - `MERAH` untuk mod CPR
   - `BIRU` untuk mod Rescue Breath
   - `KUNING` untuk mod Shock Only
   - `MERAH + BIRU` serentak untuk mod CPR 130:1
4. Semasa CPR, tekan `KUNING` apabila diminta untuk memberi shock.

## Mode comparison: `final_v1.ino` vs `final_v2.ino`

- `final_v1.ino`
  - Yellow shock button uses `INPUT` and can float, causing false activation.
  - Shock is still called automatically after the advice prompt.
  - Red+Blue dual-button detection for CPR 130:1 is not reliable.

- `final_v2.ino`
  - Yellow shock button uses `INPUT_PULLUP` and is stable.
  - Shock only happens when Yellow is pressed during the prompt.
  - Dedicated `runShock()` mode for Shock Only.
  - Reliable Red+Blue detection for CPR 130:1.

**Perbandingan dalam Bahasa Melayu:**
- `final_v1.ino`
  - Butang kuning menggunakan `INPUT` dan boleh memberi bacaan rawak.
  - Shock masih berlaku secara automatik selepas mesej nasihat.
  - Pengesanan Red+Blue untuk mod CPR 130:1 tidak konsisten.

- `final_v2.ino`
  - Butang kuning menggunakan `INPUT_PULLUP` dan lebih stabil.
  - Shock hanya berlaku apabila `KUNING` ditekan semasa arahan shock.
  - Terdapat mod `runShock()` khusus untuk Shock Only.
  - Pengesanan Red+Blue untuk CPR 130:1 lebih dipercayai.

## Diagram coordinates

The `diagram.json` layout uses these part positions:

- Arduino UNO: `top: 0`, `left: 0`
- LCD 16x2: `top: -150`, `left: 200`
- Blue button: `top: 100`, `left: 250`
- Yellow button: `top: 100`, `left: 400`
- Red button: `top: 100`, `left: 550`
- Red LED: `top: 250`, `left: 250`
- Green LED: `top: 250`, `left: 400`
- Buzzer: `top: 250`, `left: 550`

## Notes

- The sketch depends on `INPUT_PULLUP` wiring: button pin → button → GND.
- Use the `diagram.json` file in Wokwi to inspect the board layout.
- Do not use the Arduino TX/RX pins for LEDs or buttons.
