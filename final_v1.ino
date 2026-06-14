// ============================================================

//  Mini AED Trainer - DIRECT MODE SELECT version

//  Red = CPR, Blue = Rescue Breath, Yellow = Shock Only, Red & Blue = CPR Mode 2

//

//  ---- What was fixed vs the original d1.ino ----

//  * Buttons now directly select the three modes.

//  * pinMode INPUT -> INPUT_PULLUP so reads are reliable with no resistors

//    (wire each button between its pin and GND; pressed reads LOW).

//  * Removed unused on/off/menu logic from the previous sketch.

//  * mode2 lcd.setCursor(0,3) -> (0,0): a 16x2 LCD has only rows 0 and 1.

//

//  ---- Button mapping (3 physical buttons) ----

//    RED   (pin 13) = Mode 1 CPR

//    BLUE  (pin 12) = Mode 2 Rescue Breath

//    RED && BLUE (pin 12 && 13) = Mode 3 CPR 2

//    YELLOW (pin 10) = Shock Only

//

//  NOTE: each button starts its mode immediately, then returns to idle.

// ============================================================

 

#include <Wire.h>

#include <LiquidCrystal.h>    //or #include <LiquidCrystal_I2C.h>

 

const int rs = 11, en = 9, d4 = 6, d5 = 5, d6 = 4, d7 = 3;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

 

// ---------- Buttons (active LOW with INPUT_PULLUP) ----------

const int btnCPR    = 13;   // RED

const int btnBreath = 12;   // BLUE

const int btnShock  = 10;   // GREEN

int btnShockstate = 0;

 

// ---------- Outputs ----------

int buzzer   = 8;

int ledRED   = 2;

int ledGREEN = 7;  

 

// Remembered button states for edge detection.

bool lastCPR    = HIGH;

bool lastBreath = HIGH;

bool lastCPR2  = HIGH;

bool lastShock = HIGH;

 

bool idleShown = false;

 

void setup() {

  lcd.begin(16, 2);

 

  pinMode(btnCPR,    INPUT_PULLUP);

  pinMode(btnBreath, INPUT_PULLUP);

  pinMode(btnShock,  INPUT);

 

  pinMode(buzzer,   OUTPUT);

  pinMode(ledRED,   OUTPUT);

  pinMode(ledGREEN, OUTPUT);

 

  digitalWrite(ledRED,   LOW);

  digitalWrite(ledGREEN, LOW);

  digitalWrite(buzzer,   LOW);

 

  showIdle();

}

 

// Returns true ONCE at the moment a button is pressed (debounced falling edge).

bool pressed(int pin, bool &lastState) {

  bool now = digitalRead(pin);            // LOW = pressed

  bool justPressed = (lastState == HIGH && now == LOW);

  lastState = now;

  if (justPressed) {

    delay(30);                            // debounce

    return true;

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

  lcd.print("R & B = CPR2");

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

  else                lcd.print("CPR 2");

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

//Tak nak bagi shock secara automatik. Nak by tekan button

void shock(){

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

 

 

 //Tak nak bagi shock secara automatik. Nak by tekan button Yellow

 

  if (pressed(btnShock, lastShock)) {  //Shock button trial 3

  showSelectedMode(3);

  runSelectedMode(3);

  idleShown = false;

 

  shock();

  }

 

  else{

 

  delay (10000);

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

  compression(); //First cycle

  advice();

  shock();

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

 

  compression();  //Second cycle

  advice();

  shock();

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

 

  compression();  //Third cycle

  advice();

  shock();

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

  delay(200);

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

// MODE 3 = CPR with 130 compressions & 1 shock per cycle

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

 

void runCPR2(){

  mode3();

  compression2(); //First cycle

  advice();

  shock();

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

 

  compression2();  //Second cycle

  advice();

  shock();

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

 

  compression2();  //Third cycle

  advice();

  delay(1000);

  safe();

}

 

// Run whichever mode is currently selected.

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

 

  if (pressed(btnCPR, lastCPR)) {

    showSelectedMode(1);

    runSelectedMode(1);

    idleShown = false;

  }

 

  if (pressed(btnBreath, lastBreath)) {

    showSelectedMode(2);

    runSelectedMode(2);

    idleShown = false;

  }

//Nak buat mode ke-4 iaitu 130:1

  if ((pressed(btnBreath, lastBreath)) && (pressed(btnCPR, lastCPR))) {

    showSelectedMode(4);

    runSelectedMode(4);

    idleShown = false;

  }

  }

 

 