// Mini AED Trainer story-driven sketch
// - RED starts CPR with 3 compression cycles and a shock decision window
// - BLUE starts Rescue Breath with a 3,2,1,T- countdown and 20 breaths
// - YELLOW starts Shock Only mode
// This builds on sketch.ino by moving from direct mode selection to a guided training flow.
#include <Wire.h>
#include <LiquidCrystal.h>

const int rs = 11;
const int en = 9;
const int d4 = 6;
const int d5 = 5;
const int d6 = 4;
const int d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int btnCPR = 13;    // RED button
const int btnBreath = 12; // BLUE button
const int btnShock = 10;  // YELLOW button - used for shock decision or shock-only mode

const int buzzer = 8;
const int ledRED = 2;
const int ledGREEN = 7;

bool lastCPR = HIGH;
bool lastBreath = HIGH;
bool lastShock = HIGH;
bool idleShown = false;

void setup() {
  lcd.begin(16, 2);

  pinMode(btnCPR, INPUT_PULLUP);
  pinMode(btnBreath, INPUT_PULLUP);
  pinMode(btnShock, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
  pinMode(ledRED, OUTPUT);
  pinMode(ledGREEN, OUTPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(ledRED, LOW);
  digitalWrite(ledGREEN, LOW);

  showIntro();
}

bool pressed(int pin, bool &lastState) {
  bool now = digitalRead(pin);
  bool justPressed = (lastState == HIGH && now == LOW);
  lastState = now;
  if (justPressed) {
    delay(30);
    return true;
  }
  return false;
}

void beepPulse(int durationMs) {
  digitalWrite(buzzer, HIGH);
  digitalWrite(ledRED, HIGH);
  delay(durationMs);
  digitalWrite(buzzer, LOW);
  digitalWrite(ledRED, LOW);
}

// Display the training intro screens and guide the user to choose a mode.
void showIntro() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Let's practise CPR");
  lcd.setCursor(0, 1);
  lcd.print("with Mini AED");
  delay(2500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place pads as guide");
  lcd.setCursor(0, 1);
  lcd.print("Select mode");
  delay(2500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R=CPR B=Breath");
  lcd.setCursor(0, 1);
  lcd.print("Y=Shock Only");
  delay(2500);

  idleShown = false;
}

void showIdle() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press R B or Y");
  lcd.setCursor(0, 1);
  lcd.print("Ready");
}

void showRecovery() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Patient Safe");
  lcd.setCursor(0, 1);
  lcd.print("Recovery Position");
  digitalWrite(ledGREEN, HIGH);
  delay(4000);
  digitalWrite(ledGREEN, LOW);
}

void compressCycle(int cycleNumber) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cycle ");
  lcd.print(cycleNumber);
  lcd.print(" of 3");
  lcd.setCursor(0, 1);
  lcd.print("30 Compressions");
  delay(2000);

  for (int i = 1; i <= 30; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Compress");
    lcd.setCursor(0, 1);
    lcd.print(i);
    lcd.print(" / 30");

    digitalWrite(ledRED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(ledRED, LOW);
    digitalWrite(buzzer, LOW);
    delay(300);
  }
}

// Ask the trainee to press the YELLOW button within 10 seconds for shock.
bool promptShock() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shock available");
  lcd.setCursor(0, 1);
  lcd.print("Press Y in 10s");

  unsigned long start = millis();
  while (millis() - start < 10000UL) {
    int remaining = 10 - (millis() - start) / 1000;
    if (remaining < 0) remaining = 0;
    lcd.setCursor(12, 1);
    if (remaining < 10) lcd.print('0');
    lcd.print(remaining);
    if (pressed(btnShock, lastShock)) {
      return true;
    }
    delay(50);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No shock given");
  lcd.setCursor(0, 1);
  lcd.print("Next cycle");
  delay(1500);
  return false;
}

void shockSequence() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shock Advised!");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I Clear!");
  lcd.setCursor(0, 1);
  lcd.print("You Clear!");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Everybody Clear");
  delay(1500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shock Applied");
  digitalWrite(buzzer, HIGH);
  digitalWrite(ledRED, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  digitalWrite(ledRED, LOW);
  delay(1500);
}

// Run the guided CPR flow: 3 cycles of 30 compressions, each followed by an optional shock.
void runCPRMode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No heartbeat");
  lcd.setCursor(0, 1);
  lcd.print("No breathing");
  delay(2500);

  for (int cycle = 1; cycle <= 3; cycle++) {
    compressCycle(cycle);
    bool shocked = promptShock();
    if (shocked) {
      shockSequence();
    }
  }

  showRecovery();
}

// Run the single-cycle rescue breath flow with countdown and 20 breath prompts.
void runRescueBreathMode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartbeat detected");
  lcd.setCursor(0, 1);
  lcd.print("No breathing");
  delay(2500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Rescue Breath");
  lcd.setCursor(0, 1);
  lcd.print("3,2,1,T-");
  delay(2500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Inhale & Blow");
  delay(2000);

  for (int i = 1; i <= 20; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Breath ");
    lcd.print(i);
    lcd.print("/20");
    lcd.setCursor(0, 1);
    lcd.print("2 min total");

    digitalWrite(buzzer, HIGH);
    digitalWrite(ledRED, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    digitalWrite(ledRED, LOW);
    delay(5500);
  }

  showRecovery();
}

// Run the shock-only training mode directly.
void runShockOnlyMode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shock Only Mode");
  delay(2000);
  shockSequence();
  showRecovery();
}

// Main button loop: idle state plus RED/BLUE/YELLOW mode selection.
void loop() {
  if (!idleShown) {
    showIdle();
    idleShown = true;
  }

  if (pressed(btnCPR, lastCPR)) {
    runCPRMode();
    idleShown = false;
  }

  if (pressed(btnBreath, lastBreath)) {
    runRescueBreathMode();
    idleShown = false;
  }

  if (pressed(btnShock, lastShock)) {
    runShockOnlyMode();
    idleShown = false;
  }
}
