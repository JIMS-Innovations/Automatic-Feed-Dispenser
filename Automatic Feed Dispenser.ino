/*
@file: Automatic Feed Dispenser.ino
@author: Jesutofunmi Kupoluyi
@email: jimsufficiency@gmail.com
@brief: This is a firmware for an automatic feed
dispenser.
@revision: 2
@date: 27 January, 2023
*/

/*Including required libraries*/
#include "dispenser.h"

/*Constructors*/
LiquidCrystal_I2C LCD(LCD_ADDR, LCD_COL, LCD_ROW);
RTClib Clock;

/*Objects*/
Servo DispenserLid;

/*Calling dispenser struct*/
Dispenser *FeedDispenser;

void setup() {

  /*Configuring pin modes*/
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RELAY, OUTPUT);

  /*Starting I2C*/
  Wire.begin();

  /*Attaching dispenser servo PWM pin to */
  DispenserLid.attach(SERVO);

  /*Starting LCD*/
  LCD.begin(LCD_COL, LCD_ROW);

  /*Assigning functions to struct*/
  FeedDispenser->DisplayTime = DisplayTime;
  FeedDispenser->DisplayDate = DisplayDate;
  FeedDispenser->Dispense = Dispense;
  FeedDispenser->Pump = Pump;

  /*Closing dispenser lid*/
  // LidClose();
  LCD.backlight();

  /*LCD Intro*/
  intro();
  /*Attach interrupt*/
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON), onButtonPress, FALLING);

}

void loop() {
  /*Get current date and time*/
  DateTime now = Clock.now();
  // FeedDispenser->time[0] = now.hour();
  // FeedDispenser->time[1] = now.minute();
  // FeedDispenser->time[2] = now.second();
  // FeedDispenser->date[0] = now.year();
  // FeedDispenser->date[1] = now.month();
  // FeedDispenser->date[2] = now.day();


  if (btnState == true) {
    /*dispense feed*/
    Dispense(DISPENSE_TIME * 1000, DISPENSE_SPEED);

    delay(100);

    /*pump water*/
    Pump(PUMP_TIME * 1000);

    delay(100);

    btnState = false;

  } else if ((now.hour() == SetTime1[0]) && (now.minute() == SetTime1[1]) && (now.second() == SetTime1[2])) {

    /*dispense feed*/
    Dispense(DISPENSE_TIME * 1000, DISPENSE_SPEED);

    delay(100);

    /*pump water*/
    Pump(PUMP_TIME * 1000);

    delay(100);

  } else if ((now.hour() == SetTime2[0]) && (now.minute() == SetTime2[1]) && (now.second() == SetTime2[2])) {

    /*dispense feed*/
    Dispense(DISPENSE_TIME * 1000, DISPENSE_SPEED);

    delay(100);

    /*pump water*/
    Pump(PUMP_TIME * 1000);

    delay(100);

  } else {
    LCD.clear();
    delay(10);
    FeedDispenser->DisplayTime();
    FeedDispenser->DisplayDate();
    delay(50);
  }
}

/*Function definitions*/

void DisplayTime(void) {
  DateTime now = Clock.now();
  LCD.print("Time: ");
  LCD.print(now.hour());
  LCD.print(":");
  LCD.print(now.minute());
  LCD.print(":");
  LCD.print(now.second());
}

void DisplayDate(void) {
  DateTime now = Clock.now();
  LCD.setCursor(0, 1);
  LCD.print("Date: ");
  LCD.print(now.year());
  LCD.print(":");
  LCD.print(now.month());
  LCD.print(":");
  LCD.print(now.day());
}

void LidOpen(void) {

  DispenserLid.write(OPEN);
}

void LidClose(void) {

  DispenserLid.write(CLOSE);
}

void Dispense(int time, int speed) {
  LCD.clear();
  delay(100);
  LCD.setCursor(0, 0);
  delay(10);
  LCD.print(" Dispensing!!!  ");

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  delay(100);


  for (int i = CLOSE; i > OPEN; i--) {
    DispenserLid.write(i);
    delay(speed / 4);
  }

  delay(time / 6);

  for (int i = OPEN; i < CLOSE; i++) {
    DispenserLid.write(i);
    delay(speed / 4);
  }

  delay(time / 6);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  delay(100);
}

void Pump(int time) {
  LCD.clear();
  delay(100);
  LCD.setCursor(0, 0);
  delay(10);
  LCD.print("   Pumping!!!  ");

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  delay(100);

  digitalWrite(RELAY, HIGH);

  delay(time);

  digitalWrite(RELAY, LOW);

  delay(time / 2);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  delay(100);
}

void onButtonPress(void) {
  /*debounce delay*/
  for (long long i = 0; i < 16000000; i++)
    ;

  if (digitalRead(PUSH_BUTTON) == 0)
    btnState = true;
}

void intro() {
  LCD.setCursor(0, 0);
  delay(100);
  LCD.print("   AUTOMATIC  ");
  LCD.setCursor(0, 1);
  LCD.print(" FEED DISPENSER ");
  delay(2000);
  LCD.clear();
  LCD.setCursor(0, 0);
  delay(100);
  LCD.print("D.Time 1: 8.00AM");
  LCD.setCursor(0, 1);
  LCD.print("D.Time 2: 2.00PM");
  delay(2000);
}
