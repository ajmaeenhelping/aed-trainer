# Mini AED Trainer (Arduino UNO)

This project is a simulated AED trainer using an Arduino UNO, a 16x2 LCD, 3 pushbuttons, 2 LEDs, and a buzzer.

## What it does

- `RED` button = Mode 1: CPR
- `BLUE` button = Mode 2: Rescue Breath
- `GREEN` button = Mode 3: Victim Saved

When a button is pressed, the selected mode starts immediately and runs its sequence on the LCD, LEDs, and buzzer.

## Files

- `sketch.ino` — current working sketch with direct mode selection for red/blue/green buttons
- `d1.ino` — original base code provided by the project owner
- `diagram.json` — Wokwi board layout and wiring description

## Arduino pin connections

### Buttons

All buttons use `INPUT_PULLUP`, so one side of each button goes to the Arduino pin and the other side goes to `GND`.

- `RED` button: `D13`
- `BLUE` button: `D12`
- `GREEN` button: `D10`

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
- Both LED cathodes = `GND` through resistors
- Buzzer positive = `D8`
- Buzzer negative = `GND`

> Note: `D1` is the Arduino TX pin, so the current sketch avoids using it for the green LED.

## Wiring summary

1. Place each pushbutton so it connects one pin to the Arduino input pin and the other to ground.
2. Use `INPUT_PULLUP` in `sketch.ino`, so the buttons read `HIGH` when released and `LOW` when pressed.
3. Connect the LCD with the standard 4-bit wiring shown above.
4. Connect LEDs through 220Ω resistors to `D2` and `D7`, with the other LED terminal to `GND`.
5. Connect the buzzer to `D8` and `GND`.

## How to use

1. Upload `sketch.ino` to the Arduino UNO.
2. Power the UNO.
3. Press:
   - `RED` for CPR mode
   - `BLUE` for Rescue Breath mode
   - `GREEN` for Victim Saved mode
4. Watch the LCD sequence and listen for the buzzer feedback.

## Notes

- If you want the original `ON/OFF` + menu style behavior, the earlier `d1.ino` base code is not complete and does not implement button-driven mode selection.
- This README is written for physical wiring on a real Arduino UNO, with the Wokwi simulator wiring reflected in `diagram.json`.
