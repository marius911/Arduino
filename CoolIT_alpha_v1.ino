#include "Adafruit_HTU21DF.h"
#include <Wire.h> 
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//Variabile
float hum;  //Stores humidity value 
float temp; //Stores temperature value
Adafruit_HTU21DF htu = Adafruit_HTU21DF(); // pentru senzor

//Setare display
#define I2C_ADDR 0x3F
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

//functie de afisare a temperaturii si umiditatii pe display
void temp_umid_pe_display()
{ lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" ");
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Hum:  ");
  lcd.print(hum);
  lcd.print(" %"); }

  //setare buzzer
 const int pin_de_buzzer = 12; //buzzer to arduino pin 12 
  void buzzer()
  {tone(pin_de_buzzer, 1000, 500);} // Send 1KHz sound signal... 


void setup()
{
  Serial.begin(9600);
 
  lcd.begin(16,2);

  //setare lcd
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);

  // Set buzzer - pin 12 as an output
  pinMode(pin_de_buzzer, OUTPUT); 
}

void loop()
{
  
  delay(2000);
  //Read data and store it to variables hum and temp
  hum = htu.readHumidity();
  temp = htu.readTemperature();
  
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius");

// print on lcd
temp_umid_pe_display();

//suna alarma
  if(temp > 25 || temp < 5)
  {buzzer();}


}
