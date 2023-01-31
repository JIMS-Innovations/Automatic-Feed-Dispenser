#ifndef __DISPENSER_H__
#define __DISPENSER_H__

/*Including required libraries*/
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <Servo.h>

/*Defintions*/
#define PUSH_BUTTON 2
#define SERVO 3
#define RED_LED 4
#define GREEN_LED 5
#define RELAY 6
#define LCD_ADDR 0x27
#define LCD_COL 16
#define LCD_ROW 2

#define OPEN 90 //Default = 80
#define CLOSE 155 //Default = 155
#define PUMP_TIME 2
#define DISPENSE_TIME 3
#define DISPENSE_SPEED 50


volatile bool btnState = false;

const int SetTime1[3] = {8, 0, 0};
const int SetTime2[3] = {14, 0, 0};



typedef struct{
  /*Vatiables*/
  int date[3];
  int time[3];

  /*Function pointers*/
  void (*DisplayTime)();
  void (*DisplayDate)();
  void (*Dispense)(int, int);
  void (*Pump)(int);

}Dispenser;




#endif//__DISPENSER_H__