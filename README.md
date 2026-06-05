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

## Fixes from d1.ino → sketch.ino

The following critical mistakes in `d1.ino` were corrected in `sketch.ino`:

### 1. **No Button Input Handling** (Critical)
   - **d1.ino**: Button reading code was commented out (lines 438-442), so modes ran sequentially regardless of button press.
   - **sketch.ino**: Implemented proper button detection in `loop()` with `if (pressed(btnCPR, lastCPR))` checks.

### 2. **Invalid LCD Cursor Position**
   - **d1.ino**: `lcd.setCursor(0,3)` in mode2 (line 134) — a 16x2 LCD only has rows 0 and 1.
   - **sketch.ino**: Corrected to `lcd.setCursor(0, 0)` (line 216).

### 3. **Wrong LED Pin**
   - **d1.ino**: Green LED on `D1` (line 38), which is the UART TX pin causing serial conflicts.
   - **sketch.ino**: Moved to `D7` (line 34).

### 4. **No Debouncing or Edge Detection**
   - **d1.ino**: `pinMode(buttonRED, INPUT)` without `INPUT_PULLUP`; unreliable readings, no debounce logic.
   - **sketch.ino**: Uses `INPUT_PULLUP` and implements `pressed()` function (lines 62-71) with:
     - Falling edge detection (HIGH → LOW)
     - 30ms debounce delay

### 5. **Hard-Coded Sequential Mode Execution**
   - **d1.ino**: Loop runs mode1 → compression → shock → mode2 → breathing → mode3 in fixed sequence, then exits.
   - **sketch.ino**: Loop continuously checks buttons and runs only the selected mode.

### 6. **Broken Loop Structure**
   - **d1.ino**: Loop doesn't return to check buttons; it runs once and ends.
   - **sketch.ino**: Loop repeats, checking button states each iteration with edge detection.

### 7. **Incomplete/Malformed Code**
   - **d1.ino**: Line 442 has `digitalWrite (, HIGH);` (missing pin), lines 506-508 are incomplete conditionals.
   - **sketch.ino**: All syntax is clean and complete.

### 8. **Unused Variables**
   - **d1.ino**: `byte numButtonModes = 2;` defined but never used (line 30).
   - **sketch.ino**: Removed unused variables.

### 9. **No Mode Wrapper Functions**
   - **d1.ino**: Nested braces with no functional purpose (lines 448-454, 488-492, 498-502).
   - **sketch.ino**: Proper helper functions: `runCPR()`, `runRescueBreath()`, `runSelectedMode()`.

**Result**: `d1.ino` ignores button input entirely. `sketch.ino` correctly implements **3 independent button-driven modes**.

## Notes

- This README is written for physical wiring on a real Arduino UNO, with the Wokwi simulator wiring reflected in `diagram.json`.
