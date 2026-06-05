
#include <Wire.h>

#include <LiquidCrystal.h>    //or #include <LiquidCrystal_I2C.h>

const int rs = 11, en = 9, d4 = 6, d5 = 5, d6 = 4, d7 = 3;

LiquidCrystal  lcd(rs, en, d4, d5, d6, d7);

//const int buttonPins [] = {10, 12, 13}; //10=G, 12=B, 13=R

 

int buttonRED = 13;

int buttonGREEN = 10;

int buttonBLUE = 12;

 

unsigned long currentMillis;

unsigned long buttonMillis;

int buttonTimer = 350;

byte buttonMode;

byte numButtonModes = 2;

 

int buzzer = 8;

int ledRED = 2; // Red LED for every movement

int ledGREEN = 1; // Green LED for every movement

int buttonState = 0, i = 0;

 

void setup() {

lcd.begin(16,2);

//lcd.backlight();

pinMode(buttonRED, INPUT);

pinMode(buttonGREEN, INPUT);

pinMode(buttonBLUE, INPUT);

pinMode(buzzer, OUTPUT);

pinMode(ledRED, OUTPUT);

pinMode(ledGREEN, OUTPUT);

digitalWrite(ledRED, LOW);

digitalWrite(ledGREEN, LOW);

}

 

void mode1(){

//Ready for Mode 1 = CPR (compression & shock)

lcd.clear();

lcd.setCursor(0,0);

lcd.print("Heartbeat");

lcd.setCursor(0,1);

lcd.print("Detected?");

lcd.setCursor(12,1);

delay(3000);

lcd.print("No!");

delay(2000);

 

digitalWrite (ledRED, HIGH);

digitalWrite (ledGREEN, LOW);

lcd.clear();

lcd.setCursor(0,0);

lcd.print("Proceed to CPR");

lcd.setCursor(0,1);

delay(2000);

lcd.print("Ready Position");

delay(3000);

}

 

void mode2(){

  //Ready for Mode 2 = rescue breathing

lcd.clear();

lcd.setCursor(0,0);

lcd.print("Heartbeat");

lcd.setCursor(0,1);

lcd.print("Detected?");

delay(2000);

lcd.clear();

lcd.setCursor(0,3);

lcd.print("Yes, but...");

delay(3000);

lcd.clear();

lcd.setCursor(0,0);

lcd.print("No breathing");

lcd.setCursor(0,1);

lcd.print("detected");

delay (2000);

lcd.clear();

lcd.setCursor(0,0);

lcd.print("Continue to");

lcd.setCursor(0,1);

delay(200);

lcd.print("Rescue Breath");

delay(2000);

lcd.clear();

lcd.setCursor(0,0);

lcd.print("Ready? 3,2,1,T-");

delay(3500);

lcd.setCursor(0,1);

lcd.print("Inhale & Blow");

delay(2000);

}

 

void compression(){

  // make timer for the compression

  // 30 times & appear countdown on screen

  // make buzzer sound for each beat

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("Compression");

  lcd.setCursor(0,1);

  lcd.print("Start : ");

  delay(2000);

 

 for (int i = 0; i<30; i++){

  lcd.setCursor(9,1);

  lcd.print(i+1);

  digitalWrite (ledRED, HIGH);

  digitalWrite(buzzer, HIGH);

  delay(300);

  digitalWrite (ledRED, LOW);

  digitalWrite (ledGREEN, LOW);

  digitalWrite(buzzer, LOW);

  delay(300);

  }

}

 

void shock(){

  // buzzer sounds twice along with LED

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("Shock Adviced!");

  delay(2000);

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("I Clear!");

  lcd.setCursor(0,1);

  lcd.print("You Clear!");

  delay(2500);

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("   Everybody   ");

  lcd.setCursor(0,1);

  lcd.print("    Clear!    ");

  delay(3000);

 

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("Shock Applied!");

  digitalWrite(buzzer, HIGH);

  digitalWrite (ledRED, HIGH);

  digitalWrite (ledGREEN, LOW);

  delay(1500);

  digitalWrite(buzzer, LOW);

  digitalWrite (ledRED, LOW);

  digitalWrite (ledGREEN, LOW);

  delay(3000);

}

 

void breathing(){

 // make timer for the rescue breathing

  // 20 times for 2 minutes & appear countdown on screen

  // make buzzer sound for each beat

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("Rescue Breath");

  lcd.setCursor(0,1);

  lcd.print("Start : ");

  digitalWrite(buzzer, HIGH);

  digitalWrite(ledRED, HIGH);

  delay(250);

  digitalWrite(buzzer, LOW);

  digitalWrite(ledRED, LOW);

  delay(250);

 

delay(500);

 

 for (int i = 0; i<20; i++){

  lcd.setCursor(9,1);

  lcd.print(i+1);

  delay(6000);

  digitalWrite(ledRED, HIGH);

  digitalWrite(buzzer, HIGH);

  delay(300);

  digitalWrite(ledRED, LOW);

  digitalWrite(ledGREEN, LOW);

  digitalWrite(buzzer, LOW);

  delay(300);

  }}

 

void mode3(){

 

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("Heartbeat");

  lcd.setCursor(0,1);

  lcd.print("Detected?");

  lcd.setCursor(12,1);

  delay(3000);

  lcd.print("Yes!");

  delay(2000);

 

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("Stop Compression");

  delay(2500);

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("Do Recovery");

  lcd.setCursor(0,1);

  lcd.print("Position");

  digitalWrite (ledRED, LOW);

  digitalWrite (ledGREEN, HIGH);

  delay(5000);

}

 

void loop() {

 

lcd.clear();

lcd.setCursor(0,0);

lcd.print("Practise CPR");

lcd.setCursor(0,1);

lcd.print("with Mini AED");

delay(5000);

 

lcd.clear();

lcd.setCursor(0,0);

lcd.print("Let us start!");

delay (2000);

 

//int buttonState = digitalRead(buttonPins);

//if (buttonState == HIGH)

//digitalWrite (, HIGH);

 

mode1();

{

  compression();

    {shock();}

}

 

lcd.clear();

lcd.setCursor(0,0);

lcd.print("Heartbeat");

lcd.setCursor(0,1);

lcd.print("Detected?");

lcd.setCursor(12,1);

delay(1000);

lcd.print("No!");

delay(2000);

lcd.clear();

lcd.setCursor(0,0);

lcd.print("Continue CPR");

delay(2000);

 

compression();

{

  shock();

}

 

mode2();

{

  breathing();

}

 

//if (digitalRead(buttonGREEN) == HIGH && HIGH);

//digitalRead(buttonGREEN) == HIGH;{

mode3();

 

delay(4000);

 

digitalWrite (ledRED, LOW);

digitalWrite (ledGREEN, LOW);

delay(3000);

 

}

 

 