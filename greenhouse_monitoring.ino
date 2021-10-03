#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>
#include <SandTimer.h>
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,20,4);
HCSR04 watersens(5,6);
SandTimer timersensor;
SandTimer backlightimer;
SandTimer pump1timer;
SandTimer pump2timer;
const int lowaterled=4;
const int venthum=7;
const int irsensor=8;
const int pump2=9;
const int heaterpin=10;
const int lightpin=11;
const int pump1=12;
const int timerpin=13;
int h=0;
int t=0;
int Soil1=0; //lower soil moisture sensor
int Soil2=0;
int settemp;
int daytemp=26;
int nightemp=23;
int timervalue;
int motiondetect;
int waterlevel;


void setup() {
  dht.begin();
  lcd.init();
  timersensor.start(2000);
  backlightimer.start(30000);
  pump1timer.start(10000);
  pump2timer.start(10000);
  pinMode(heaterpin, OUTPUT);
  pinMode(timerpin, INPUT);
  pinMode(lightpin, OUTPUT);
  pinMode(lowaterled, OUTPUT);
  pinMode(venthum, OUTPUT);
  pinMode(irsensor, INPUT);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  digitalWrite(pump1, HIGH);
  digitalWrite(pump2, HIGH);
 }

void loop() {
  sensorcheck();
  lowater();
  settempset();
  humidcontrol();
  runheat();
  runlights();
  lcdprint();
  lcdbacklight();
  watering();
 }

//===========Functions=============

void sensorcheck(){
  if (timersensor.finished()){
    h=dht.readHumidity();
    t=dht.readTemperature();
    timervalue=digitalRead(timerpin);
    Soil1=analogRead(6);
    Soil2=analogRead(7);
    Soil1=map(Soil1,560,270,01,99);
    Soil2=map(Soil2,545,270,01,99);
    motiondetect=digitalRead(irsensor);
    waterlevel=watersens.dist();
    waterlevel=map(waterlevel,3,57,99,01);
    timersensor.startOver();
  }
}

void lowater(){
  if (waterlevel<=3){
    digitalWrite(lowaterled, HIGH);
  }
  else{
    digitalWrite(lowaterled, LOW);
  }
}

void watering(){
  if ((waterlevel>=3)&&(Soil1<=24)){
    digitalWrite(pump1, LOW);
//    if (pump1timer.finished()){
//      digitalWrite(pump1, HIGH);
//      pump1timer.startOver();
//    }
  }
else{
  digitalWrite(pump1, HIGH);  
}
  
  if ((waterlevel>=3)&&(Soil2<=24)){
    digitalWrite(pump2, LOW);
//    if (pump2timer.finished()){
//      digitalWrite(pump2, HIGH);
//      pump2timer.startOver();
//    }
  }
else {
  digitalWrite(pump2, HIGH);
}
}

void settempset(){
  if (timervalue==HIGH){
  settemp=daytemp;
}
else{
  settemp=nightemp;
}
}

void lcdbacklight(){
    if (motiondetect==1){
    lcd.backlight();
  }
else if ((motiondetect==0)&&(backlightimer.finished())){
  lcd.noBacklight();
  backlightimer.startOver();
}
}

void humidcontrol(){
  if(h>=72){
  digitalWrite(venthum, HIGH);
}
else if (h<=65){
  digitalWrite(venthum, LOW);
}
}

void runheat(){
  if(t>=settemp){
  digitalWrite(heaterpin, HIGH);
}
else{
  digitalWrite(heaterpin, LOW);
}
}

void runlights(){
if (timervalue==LOW){
  digitalWrite(lightpin, HIGH);
}
else{
  digitalWrite(lightpin, LOW);
}
}

void lcdprint(){
  lcd.setCursor(0,0);
  lcd.print("T/H:");
  lcd.print(t);
  lcd.print("/");
  lcd.print(h);
  lcd.setCursor(10,0);
  lcd.print("WL:");
  lcd.print(waterlevel);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Z1:");
  lcd.print(Soil1);
  lcd.print("%");
  lcd.print(" ");
  lcd.print("Z2:");
  lcd.print(Soil2);
  lcd.print("%");
}
