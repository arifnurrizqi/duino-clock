//************libraries**************//
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2 // define the pin used for the DHT11 sensor
#define DHTTYPE DHT22 // DHT 22 

//************************************//
LiquidCrystal_I2C lcd(0x27,16,2); // Display  I2C 16 x 2
RTC_DS1307 RTC;
DHT dht(DHTPIN, DHTTYPE);

//************Button*****************//
int P1=3; // Button SET MENU'
int P2=4; // Button +
int P3=5; // Button -

//************Variables**************//
int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg;
int menu =0;

unsigned long previousMillisCLock = 0;
int waktuTampilWaktu = 25; // 25 detik
int waktuTampilSuhu = 5; // 5 detik
int status = 0;  // 0 untuk waktu, 1 untuk suhu/kelembaban

void setup()
{
  lcd.init(); 
  lcd.backlight();
  lcd.clear();

  pinMode(P1,INPUT);
  pinMode(P2,INPUT);
  pinMode(P3,INPUT);

  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  dht.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Set the date and time at compile time
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  // RTC.adjust(DateTime(__DATE__, __TIME__)); //removing "//" to adjust the time
    // The default display shows the date and time
  int menu=0;
}
 
void loop()
{ 

// check if you press the SET button and increase the menu index
  if(digitalRead(P1))
  {
   menu=menu+1;
  }
// in which subroutine should we go?
  if (menu==0)
    {

     DisplayDateTime(); // void DisplayDateTime

    }
  if (menu==1)
    {
    DisplaySetHour();
    }
  if (menu==2)
    {
    DisplaySetMinute();
    }
  if (menu==3)
    {
    DisplaySetYear();
    }
  if (menu==4)
    {
    DisplaySetMonth();
    }
  if (menu==5)
    {
    DisplaySetDay();
    }
  if (menu==6)
    {
    StoreAgg(); 
    delay(500);
    menu=0;
    }
    delay(100);
}

void DisplayDateTime ()
{
  unsigned long currentMillis = millis();
  if (status == 0) {
    // Menampilkan waktu
    if (currentMillis - previousMillisCLock >= (waktuTampilWaktu*1000)) {
      previousMillisCLock = currentMillis;
      status = 1;  // Ganti status ke suhu/kelembaban
      lcd.clear();
    }
    // We show the current date and time
    DateTime now = RTC.now();

    lcd.setCursor(0, 1);
    lcd.print("Hour: ");
    if (now.hour()<=9)
    {
      lcd.print("0");
    }
    lcd.print(now.hour(), DEC);
    hourupg=now.hour();
    lcd.print(":");
    if (now.minute()<=9)
    {
      lcd.print("0");
    }
    lcd.print(now.minute(), DEC);
    minupg=now.minute();
    lcd.print(":");
    if (now.second()<=9)
    {
      lcd.print("0");
    }
    lcd.print(now.second(), DEC);

    lcd.setCursor(0, 0);
    lcd.print("Date: ");
    if (now.day()<=9)
    {
      lcd.print("0");
    }
    lcd.print(now.day(), DEC);
    dayupg=now.day();
    lcd.print("/");
    if (now.month()<=9)
    {
      lcd.print("0");
    }
    lcd.print(now.month(), DEC);
    monthupg=now.month();
    lcd.print("/");
    lcd.print(now.year(), DEC);
    yearupg=now.year();

  } else {

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    lcd.clear();  // Bersihkan layar sebelum menampilkan suhu/kelembaban
    lcd.setCursor(0,0);
    lcd.print("Humi: "); 
    lcd.print(h);
    lcd.print(" %      ");
    lcd.setCursor(0,1);
    lcd.print("Temp: "); 
    lcd.print(t);
    lcd.print(" C      ");
    delay(waktuTampilSuhu*1000);
    status = 0;  // Ganti status ke waktu
  }
}

void DisplaySetHour()
{
// time setting
  lcd.clear();
  DateTime now = RTC.now();
  if(digitalRead(P2)==HIGH)
  {
    if(hourupg==23)
    {
      hourupg=0;
    }
    else
    {
      hourupg=hourupg+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if(hourupg==0)
    {
      hourupg=23;
    }
    else
    {
      hourupg=hourupg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set time:");
  lcd.setCursor(0,1);
  lcd.print(hourupg,DEC);
  delay(200);
}

void DisplaySetMinute()
{
// Setting the minutes
  lcd.clear();
  if(digitalRead(P2)==HIGH)
  {
    if (minupg==59)
    {
      minupg=0;
    }
    else
    {
      minupg=minupg+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if (minupg==0)
    {
      minupg=59;
    }
    else
    {
      minupg=minupg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Minutes:");
  lcd.setCursor(0,1);
  lcd.print(minupg,DEC);
  delay(200);
}
  
void DisplaySetYear()
{
// setting the year
  lcd.clear();
  if(digitalRead(P2)==HIGH)
  {    
    yearupg=yearupg+1;
  }
   if(digitalRead(P3)==HIGH)
  {
    yearupg=yearupg-1;
  }
  lcd.setCursor(0,0);
  lcd.print("Set Year:");
  lcd.setCursor(0,1);
  lcd.print(yearupg,DEC);
  delay(200);
}

void DisplaySetMonth()
{
// Setting the month
  lcd.clear();
  if(digitalRead(P2)==HIGH)
  {
    if (monthupg==12)
    {
      monthupg=1;
    }
    else
    {
      monthupg=monthupg+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if (monthupg==1)
    {
      monthupg=12;
    }
    else
    {
      monthupg=monthupg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Month:");
  lcd.setCursor(0,1);
  lcd.print(monthupg,DEC);
  delay(200);
}

void DisplaySetDay()
{
// Setting the day
  lcd.clear();
  if(digitalRead(P2)==HIGH)
  {
    if (dayupg==31)
    {
      dayupg=1;
    }
    else
    {
      dayupg=dayupg+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if (dayupg==1)
    {
      dayupg=31;
    }
    else
    {
      dayupg=dayupg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Day:");
  lcd.setCursor(0,1);
  lcd.print(dayupg,DEC);
  delay(200);
}

void StoreAgg()
{
// Variable saving
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SAVING IN");
  lcd.setCursor(0,1);
  lcd.print("PROGRESS");
  RTC.adjust(DateTime(yearupg,monthupg,dayupg,hourupg,minupg,0));
  delay(200);
}
