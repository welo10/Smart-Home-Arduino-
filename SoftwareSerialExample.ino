#include <Key.h>
#include <Keypad.h>
#include <dht.h>// temp library
#include <LiquidCrystal.h>
//#include<String>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(0,1 ); 
const int rs = 9, en = 8, d4 =3, d5 = 4, d6 = 5, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int rain=A0;
int temp=A1;
int gas=A2;
int ldr=A3;
int buzzer=13;  
int pir=2;
int mofset=10;
int led=A4;
int brightness=0;
char password[]="1234"; 
const byte ROWS = 4; //four rows
const byte COLS = 4;
//four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte colPins[ROWS] = { 28,29,27,26}; //connect to the row pinouts of the keypad
byte rowPins[COLS] = {25,24,23,22}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

dht DHT ;
 unsigned long long timee;
 unsigned long long disp=10000;
void setup() {

lcd.begin(16, 2);
Serial.begin(9600);
pinMode (temp,INPUT);
pinMode (buzzer,OUTPUT);
pinMode (rain,INPUT);
pinMode (gas,INPUT);
pinMode (ldr,INPUT);
pinMode (pir,INPUT);
pinMode(mofset,OUTPUT);
pinMode(led,OUTPUT);
digitalWrite(mofset,HIGH);
}
void buzz()
{
 tone(buzzer,1000);
 delay(1000);
 noTone(buzzer); 
 delay(1000);
}
void loop() {
  timee=millis();
  String pass="";
   char Key = customKeypad.getKey();
    if(Key){lcd.clear();
      pass+=Key;
    lcd.print("*");
 while (true){
  Serial.println(Key);
Key = customKeypad.getKey();

 if(Key){
 if(Key=='*')break;
  pass+=Key;
  lcd.print("*");
 }
  }
  lcd.clear();
    if(pass==password)
 { lcd.print("Password");
 lcd.setCursor(1,1);
 lcd.print("Accepted");
  delay(2000);}
  else
  {
   lcd.print("Password "); 
    lcd.setCursor(1,1);
 lcd.print("Denied");
   delay(2000);
  }
    }
  
  

  
///////////// temp
   DHT.read11(temp);
    if(timee>disp)
    {disp+=10000;
    lcd.clear();
     Serial.print("Current humidity = ");
    Serial.print((int)(DHT.humidity));
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print((int)(DHT.temperature)); 
    Serial.println("C");
    Serial.println("Gas Reading: ");Serial.print(analogRead(gas));
    Serial.println("");
  lcd.print("temperature =");
  lcd.print(DHT.temperature);
    lcd.print("C");
  lcd.setCursor(1,1);
  lcd.print("humidity = "); 
   lcd.print(DHT.humidity);
    }
 //////////////// gas
  if(analogRead(gas)>500){   lcd.clear();
    Serial.println("There Is Gas Detected");
    lcd.print("There Is Gas");
  lcd.setCursor(1,1);
  lcd.print("Detected");
buzz();}
///////////////////RAIN
  if(analogRead(rain)<400){
     lcd.clear();
    Serial.println("There Is Rain Outside");
    lcd.print("There Is Rain");
  lcd.setCursor(1,1);
  lcd.print("   Outside");
buzz();
 lcd.clear();
}
  
   //////////////ldr
if(analogRead(ldr)<150)
  { Serial.println("It's Dark Do you want to turn on the lights?(y/n)");
  while(Serial.available() == 0){}
    char ch=Serial.read();
    if(ch=='y') brightness=255;
    else
     brightness=0;
 }

   //////////////
  /////////////////fan
  
                 

///////////////// bluetooth

if (Serial.available())
   {char customKey = Serial.read();
if ((char)(customKey)=='f')
 {lcd.clear(); 
  digitalWrite(mofset,LOW);
 lcd.print("Turn Off The Fan");
 }
   else{
    brightness=0;
    // Serial.println(brightness);

      brightness=(brightness*10)+(customKey-48);
 while (true){
customKey = Serial.read();
 if(customKey){
 if(customKey=='*')break;
  brightness=(brightness*10)+(customKey-48);

 }
 }
   if (brightness==0)
{lcd.clear(); 
  lcd.print("Turn Off The Lights");
   Serial.println("Turn Off The Lights");}

   else
  {lcd.clear(); 
    lcd.print("Turn On The Lights");
   Serial.println("Turn On The Lights");}
   }
   }
   
   /////////////
   analogWrite(led,brightness);
}
