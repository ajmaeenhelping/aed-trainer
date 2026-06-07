# Mini AED Trainer (Arduino UNO)

This project is a simulated AED trainer using an Arduino UNO, a 16x2 LCD, 3 pushbuttons, 2 LEDs, and a buzzer.

## What it does

- `RED` button = Mode 1: CPR
- `BLUE` button = Mode 2: Rescue Breath
- `GREEN` button = Mode 3: Victim Saved

When a button is pressed, the selected mode starts immediately and runs its sequence on the LCD, LEDs, and buzzer.

## Files

- `mini_aed_trainer.ino` — new story-driven CPR trainer with CPR, Rescue Breath, and Shock Only modes
- `sketch.ino` — current working sketch with direct mode selection for red/blue/green buttons
- `d1.ino` — original base code provided by the project owner
- `diagram.json` — Wokwi board layout and wiring description

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
- Both LED cathodes = `GND` through resistors
- Buzzer positive = `D8`
- Buzzer negative = `GND`

> Note: `D1` is the Arduino TX pin, so the current sketch avoids using it for the green LED.

## Wiring summary

1. Place each pushbutton so it connects one pin to the Arduino input pin and the other to ground.
2. Use `INPUT_PULLUP` in the sketch, so the buttons read `HIGH` when released and `LOW` when pressed.
3. Connect the LCD with the standard 4-bit wiring shown above.
4. Connect LEDs through 220Ω resistors to `D2` and `D7`, with the other LED terminal to `GND`.
5. Connect the buzzer to `D8` and `GND`.

## How to use

1. Upload `mini_aed_trainer.ino` to the Arduino UNO.
2. Power the UNO.
3. Press:
   - `RED` for CPR mode
   - `BLUE` for Rescue Breath mode
   - `YELLOW` for Shock Only mode
4. Watch the LCD sequence and listen for the buzzer feedback.

## New story-driven CPR mode

The new `mini_aed_trainer.ino` file adds a guided training flow:

- Intro screen: "Let’s practise CPR with Mini AED Trainer. Place the pads as in the guide. Select Mode. Press the button. R=CPR B=Breath Y=Shock Only"
- `RED` starts 3 cycles of CPR with 30 compressions each.
- After each cycle, the trainer shows a 10-second window to press `YELLOW` for shock.
- If shock is not pressed within 10 seconds, the next CPR cycle continues.
- After 3 cycles, the patient proceeds to Recovery Position.
- `BLUE` starts one Rescue Breath cycle with a 3,2,1,T- preparation flow and 20 breaths over 2 minutes.
- After the Rescue Breath cycle, the patient proceeds to Recovery Position.

## Fixes from d1.ino → sketch.ino

The following critical mistakes in `d1.ino` were corrected in `sketch.ino`:

### 1. **No Button Input Handling** (Critical)
   - **d1.ino**: Button reading code was commented out (lines 438-442), so modes ran sequentially regardless of button press.
   - **sketch.ino**: Implemented proper button detection in `loop()` with `if (pressed(btnCPR, lastCPR))` checks.
   
   **How it should work:**
   ```cpp
   // Continuously check buttons in loop
   void loop() {
     if (pressed(btnCPR, lastCPR)) {        // Only triggers once per button press
       showSelectedMode(1);
       runSelectedMode(1);
     }
     if (pressed(btnBreath, lastBreath)) {  // Each button is independent
       showSelectedMode(2);
       runSelectedMode(2);
     }
   }
   ```

### 2. **Invalid LCD Cursor Position**
   - **d1.ino**: `lcd.setCursor(0,3)` in mode2 (line 134) — a 16x2 LCD only has rows 0 and 1.
   - **sketch.ino**: Corrected to `lcd.setCursor(0, 0)` (line 216).
   
   **Why**: A 16x2 LCD has only 2 rows (0 and 1). Row 3 doesn't exist and causes undefined behavior.

### 3. **Wrong LED Pin**
   - **d1.ino**: Green LED on `D1` (line 38), which is the UART TX pin causing serial conflicts.
   - **sketch.ino**: Moved to `D7` (line 34).
   
   **Why**: D0/D1 are hardware serial pins (RX/TX). Using D1 for GPIO breaks serial communication needed for debugging/uploads.

### 4. **No Debouncing or Edge Detection** (Critical)
   - **d1.ino**: `pinMode(buttonRED, INPUT)` without `INPUT_PULLUP`; unreliable readings, no debounce logic.
   - **sketch.ino**: Uses `INPUT_PULLUP` and implements `pressed()` function (lines 62-71).
   
   **Why buttons fail in d1:**
   - `INPUT` mode floats; button voltage is undefined when released → random reads.
   - No debounce → single button press triggers multiple times due to electrical bounce (~5-20ms).
   - No edge detection → mode runs multiple times per press.
   
   **How sketch.ino fixes it:**
   ```cpp
   // INPUT_PULLUP: pulls pin HIGH internally (5V), LOW when button pressed (connects to GND)
   pinMode(btnCPR, INPUT_PULLUP);
   
   // Edge detection: only triggers once when transitioning HIGH → LOW
   bool pressed(int pin, bool &lastState) {
     bool now = digitalRead(pin);                // LOW = pressed with INPUT_PULLUP
     bool justPressed = (lastState == HIGH && now == LOW);  // Falling edge
     lastState = now;
     if (justPressed) {
       delay(30);  // Debounce: ignore electrical noise for 30ms
       return true;
     }
     return false;
   }
   ```
   
   **Wiring requirement with INPUT_PULLUP:**
   ```
   Button pin (D13) ----[Button]---- GND
   (Arduino pulls to 5V when not pressed)
   ```

### 5. **Hard-Coded Sequential Mode Execution**
   - **d1.ino**: Loop runs mode1 → compression → shock → mode2 → breathing → mode3 in fixed sequence, then exits.
   - **sketch.ino**: Loop continuously checks buttons and runs only the selected mode.
   
   **d1.ino execution (wrong):**
   ```cpp
   mode1();           // Always runs first
   compression();     // Always runs
   shock();           // Always runs
   mode2();           // Always runs
   breathing();       // Always runs
   mode3();           // Always runs
   // Loop exits, never returns to check buttons
   ```
   
   **sketch.ino execution (correct):**
   ```cpp
   // Press RED button → only mode1 runs
   // Press BLUE button → only mode2 runs
   // Press GREEN button → only mode3 runs
   // Returns to idle, ready for next button press
   ```

### 6. **Broken Loop Structure**
   - **d1.ino**: Loop doesn't return to check buttons; it runs once and ends.
   - **sketch.ino**: Loop repeats continuously, checking button states each iteration.
   
   **Why**: Arduino's `loop()` should run forever. In d1, it executes all code once then doesn't restart.

### 7. **Incomplete/Malformed Code**
   - **d1.ino**: Line 442 has `digitalWrite (, HIGH);` (missing pin), lines 506-508 are incomplete conditionals.
   - **sketch.ino**: All syntax is clean and complete.

### 8. **Unused Variables**
   - **d1.ino**: `byte numButtonModes = 2;` defined but never used (line 30).
   - **sketch.ino**: Removed unused variables.

### 9. **No Mode Wrapper Functions**
   - **d1.ino**: Nested braces with no functional purpose (lines 448-454, 488-492, 498-502).
   - **sketch.ino**: Proper helper functions: `runCPR()`, `runRescueBreath()`, `runSelectedMode()` for cleaner, reusable code.

**Result**: `d1.ino` ignores button input entirely. `sketch.ino` correctly implements **3 independent button-driven modes** with proper debouncing and edge detection.

## Fixes from sketch.ino → mini_aed_trainer.ino

The new `mini_aed_trainer.ino` sketch builds on `sketch.ino` and adds a story-based CPR trainer experience:

- `sketch.ino` used direct button selection for CPR, Rescue Breath, and Victim Saved.
- `mini_aed_trainer.ino` adds an intro flow, recovery position screen, and a dedicated `YELLOW` shock-only mode.
- `RED` now runs 3 guided CPR cycles of 30 compressions each, with a 10-second `YELLOW` shock prompt after every cycle.
- `BLUE` now runs one Rescue Breath cycle using the `3,2,1,T-` prep flow and 20 breaths.
- `YELLOW` now triggers an explicit shock-only sequence, instead of `GREEN` for Victim Saved.
- Keeps the reliable `INPUT_PULLUP` button reading and debounced falling-edge detection from `sketch.ino`.
- Introduces `promptShock()` to pause for a decision window instead of always applying shock immediately.
- Uses clearer helper functions for story screens, compress cycles, and mode transitions.

## Notes

- This README is written for physical wiring on a real Arduino UNO, with the Wokwi simulator wiring reflected in `diagram.json`.
