// ============================================================
//  Mini AED Trainer - FINAL V2
//  Red = CPR, Blue = Rescue Breath, Yellow = Shock Only
//  Red + Blue = CPR 130:1 mode
//
//  Fixes in V2:
//  * Yellow shock button now uses INPUT_PULLUP and only fires when pressed.
//  * Shock is not delivered automatically after advice.
//  * Separate Shock Only mode implemented with runShock().
//  * Dual Red+Blue button press now enters CPR 130:1 mode reliably.
//
//  Nota Bahasa Melayu:
//  * Butang kuning hanya berfungsi untuk shock apabila ditekan semasa prompt.
//  * Tekan R + B serentak untuk masuk ke mod CPR 130:1.
//
//  Peta mod:
//  * Tekan MERAH (R) = Mod CPR biasa.
//  * Tekan BIRU (B) = Mod Rescue Breath.
//  * Tekan KUNING (Y) = Mod Shock Only.
//  * Tekan MERAH + BIRU serentak = Mod CPR 130:1.
// ============================================================

#include <Wire.h>
#include <LiquidCrystal.h>

const int rs = 11, en = 9, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// ---------- Buttons (active LOW with INPUT_PULLUP) ----------
const int btnCPR    = 13;   // RED
const int btnBreath = 12;   // BLUE
const int btnShock  = 10;   // YELLOW

// ---------- Outputs ----------
const int buzzer   = 8;
const int ledRED   = 2;
const int ledGREEN = 7;

// Remembered button states for edge detection.
// (INPUT_PULLUP: HIGH bila lepas, LOW bila ditekan)
bool lastCPR    = HIGH;
bool lastBreath = HIGH;
bool lastShock  = HIGH;

bool idleShown = false;

void setup() {
  lcd.begin(16, 2);

  pinMode(btnCPR,    INPUT_PULLUP);
  pinMode(btnBreath, INPUT_PULLUP);
  pinMode(btnShock,  INPUT_PULLUP);

  pinMode(buzzer,   OUTPUT);
  pinMode(ledRED,   OUTPUT);
  pinMode(ledGREEN, OUTPUT);

  digitalWrite(ledRED,   LOW);
  digitalWrite(ledGREEN, LOW);
  digitalWrite(buzzer,   LOW);

  showIdle();
}

// Returns true ONCE at the moment a button is pressed (debounced falling edge).
// Fungsi ini digunakan untuk kes tunggal (satu butang sahaja).
bool pressed(int pin, bool &lastState) {
  bool now = digitalRead(pin);
  bool justPressed = (lastState == HIGH && now == LOW);
  lastState = now;
  if (justPressed) {
    delay(30);
    if (digitalRead(pin) == LOW) {
      return true;
    }
  }
  return false;
}

// Detect pressing both RED and BLUE together.
// Jangan kemaskini state lastCPR/lastBreath jika hanya satu butang ditekan.
bool pressedBoth() {
  bool nowCPR = digitalRead(btnCPR);
  bool nowBreath = digitalRead(btnBreath);
  bool bothPressed = (nowCPR == LOW && nowBreath == LOW);
  bool justPressed = (bothPressed && (lastCPR == HIGH || lastBreath == HIGH));
  if (justPressed) {
    // Debounce kedua-dua butang bersama-sama
    delay(30);
    if (digitalRead(btnCPR) == LOW && digitalRead(btnBreath) == LOW) {
      lastCPR = LOW;
      lastBreath = LOW;
      return true;
    }
  }
  return false;
}

// ---------- Screens ----------
void showOff() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mini AED");
  lcd.setCursor(0, 1);
  lcd.print("Powered OFF");
  digitalWrite(ledRED,   LOW);
  digitalWrite(ledGREEN, LOW);
  digitalWrite(buzzer,   LOW);
}

void showIdle() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Let's practise");
  lcd.setCursor(0,1);
  lcd.print("CPR with");
  delay(5000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mini AED Trainer");
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print("Let us start!");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Attach the pads");
  lcd.setCursor(0, 1);
  lcd.print("Select the Mode");
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press the button");
  lcd.setCursor(0, 1);
  lcd.print("R=CPR B=Breath");
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R + B = CPR2");
  lcd.setCursor(0, 1);
  lcd.print("Y=Shock Only");
  delay(5000);
}

void showSelectedMode(byte mode) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode ");
  lcd.print(mode);
  lcd.print(":");
  lcd.setCursor(0, 1);

  if (mode == 1)      lcd.print("CPR");
  else if (mode == 2) lcd.print("Rescue Breath");
  else if (mode == 3) lcd.print("Shock");
  else if (mode == 4) lcd.print("CPR 2");
  else                lcd.print("Unknown");
  delay(3000);
}

// ============================================================
// MODE 1 = CPR (compression & shock)
// ============================================================
void mode1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartbeat");
  lcd.setCursor(0, 1);
  lcd.print("Detected?");
  delay(3000);
  lcd.setCursor(12, 1);
  lcd.print("No!");
  delay(2000);

  digitalWrite(ledRED,   HIGH);
  digitalWrite(ledGREEN, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Proceed to CPR");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Ready Position");
  delay(3000);
}

void compression() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Compression");
  lcd.setCursor(0, 1);
  lcd.print("Start : ");
  delay(2000);

  for (int i = 0; i < 30; i++) {
    lcd.setCursor(9, 1);
    lcd.print(i + 1);
    digitalWrite(ledRED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(ledRED,   LOW);
    digitalWrite(ledGREEN, LOW);
    digitalWrite(buzzer,   LOW);
    delay(300);
  }
}

void shock() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shock Applied!");
  digitalWrite(buzzer,   HIGH);
  digitalWrite(ledRED,   HIGH);
  digitalWrite(ledGREEN, LOW);
  delay(1500);
  digitalWrite(buzzer,   LOW);
  digitalWrite(ledRED,   LOW);
  digitalWrite(ledGREEN, LOW);
  delay(3000);
}

void advice() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shock Adviced!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I Clear!");
  lcd.setCursor(0, 1);
  lcd.print("You Clear!");
  delay(2500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Everybody   ");
  lcd.setCursor(0, 1);
  lcd.print("    Clear!    ");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press Yellow");
  lcd.setCursor(0, 1);
  lcd.print("Button for Shock");
  delay(2500);

  if (pressed(btnShock, lastShock)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Shock Delivered");
    delay(1000);
    shock();
  } else {
    delay(10000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No shock given");
    lcd.setCursor(0, 1);
    lcd.print("Next cycle");
    delay(2000);
  }
}

void safe() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartbeat");
  lcd.setCursor(0, 1);
  lcd.print("Detected?");
  delay(3000);
  lcd.setCursor(12, 1);
  lcd.print("Yes!");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stop Compression");
  delay(2500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Do Recovery");
  lcd.setCursor(0, 1);
  lcd.print("Position");
  digitalWrite(ledRED,   LOW);
  digitalWrite(ledGREEN, HIGH);
  delay(5000);
}

void runCPR() {
  mode1();
  compression();
  advice();
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartbeat");
  lcd.setCursor(0, 1);
  lcd.print("Detected?");
  delay(1000);
  lcd.setCursor(12, 1);
  lcd.print("No!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Continue CPR");
  delay(2000);

  compression();
  advice();
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartbeat");
  lcd.setCursor(0, 1);
  lcd.print("Detected?");
  delay(1000);
  lcd.setCursor(12, 1);
  lcd.print("No!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Continue CPR");
  delay(2000);

  compression();
  advice();
  delay(1000);
  safe();
}

// ============================================================
// MODE 2 = Rescue Breathing
// ============================================================
void mode2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartbeat");
  lcd.setCursor(0, 1);
  lcd.print("Detected?");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Yes, heartbeat");
  lcd.setCursor(0, 1);
  lcd.print("detected but...");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No breathing");
  lcd.setCursor(0, 1);
  lcd.print("detected!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Proceed to");
  lcd.setCursor(0, 1);
  lcd.print("Rescue Breath");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready? 3,2,1,T-");
  delay(3000);
  lcd.setCursor(0, 1);
  lcd.print("Inhale & Blow");
  delay(3000);
}

void breathing() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Rescue Breath");
  lcd.setCursor(0, 1);
  lcd.print("Start : ");
  delay(1000);
  digitalWrite(buzzer,  HIGH);
  digitalWrite(ledRED,  HIGH);
  delay(250);
  digitalWrite(buzzer,  LOW);
  digitalWrite(ledRED,  LOW);
  delay(250);

  delay(500);

  for (int i = 0; i < 20; i++) {
    lcd.setCursor(9, 1);
    lcd.print(i + 1);
    delay(6000);
    digitalWrite(ledRED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(ledRED,   LOW);
    digitalWrite(ledGREEN, LOW);
    digitalWrite(buzzer,   LOW);
    delay(300);
  }
}

void runRescueBreath() {
  mode2();
  breathing();
  safe();
}

// ============================================================
// MODE 4 = CPR with 130 compressions & 1 shock per cycle
// ============================================================
void mode3() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartbeat");
  lcd.setCursor(0, 1);
  lcd.print("Detected?");
  delay(3000);
  lcd.setCursor(12, 1);
  lcd.print("No!");
  delay(2000);

  digitalWrite(ledRED,   HIGH);
  digitalWrite(ledGREEN, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Proceed to CPR2");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Ready Position");
  delay(3000);
}

void compression2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Compression");
  lcd.setCursor(0, 1);
  lcd.print("Start : ");
  delay(2000);

  for (int i = 0; i < 130; i++) {
    lcd.setCursor(9, 1);
    lcd.print(i + 1);
    digitalWrite(ledRED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(ledRED,   LOW);
    digitalWrite(ledGREEN, LOW);
    digitalWrite(buzzer,   LOW);
    delay(300);
  }
}

void runCPR2() {
  mode3();
  compression2();
  advice();
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartbeat");
  lcd.setCursor(0, 1);
  lcd.print("Detected?");
  delay(1000);
  lcd.setCursor(12, 1);
  lcd.print("No!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Continue CPR");
  delay(2000);

  compression2();
  advice();
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartbeat");
  lcd.setCursor(0, 1);
  lcd.print("Detected?");
  delay(1000);
  lcd.setCursor(12, 1);
  lcd.print("No!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Continue CPR");
  delay(2000);

  compression2();
  advice();
  delay(1000);
  safe();
}

void runShock() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shock Only");
  lcd.setCursor(0, 1);
  lcd.print("Stand Clear!");
  delay(2000);
  shock();
}

void runSelectedMode(byte mode) {
  if (mode == 1)      runCPR();
  else if (mode == 2) runRescueBreath();
  else if (mode == 3) runShock();
  else                runCPR2();

  digitalWrite(ledRED,   LOW);
  digitalWrite(ledGREEN, LOW);
}

// ============================================================
// MAIN LOOP - reads buttons and reacts
// ============================================================
void loop() {
  if (!idleShown) {
    showIdle();
    idleShown = true;
  }

  // Semak dahulu kombinasi R+B sebelum semak butang tunggal.
  if (pressedBoth()) {
    showSelectedMode(4);
    runSelectedMode(4);
    idleShown = false;
  } else if (pressed(btnCPR, lastCPR)) {
    showSelectedMode(1);
    runSelectedMode(1);
    idleShown = false;
  } else if (pressed(btnBreath, lastBreath)) {
    showSelectedMode(2);
    runSelectedMode(2);
    idleShown = false;
  } else if (pressed(btnShock, lastShock)) {
    showSelectedMode(3);
    runSelectedMode(3);
    idleShown = false;
  }
}
