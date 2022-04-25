#include <LiquidCrystal_I2C.h>

#include <AltSoftSerial.h>

#include <Vector.h>

#include <Adafruit_Fingerprint.h>

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

//Oggetti
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

AltSoftSerial bluetooth;

LiquidCrystal_I2C lcd(0x27, 16, 2);

Vector<String> permissions;
Vector<int> blackList;
String message; 
const int Open = 12;
const int Closed = 13;
bool Status;
uint8_t id;
String identity;
int ButtonState = HIGH;
const int BUTTON_PIN = 5;
bool done;
int buzzer = 7;


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode (buzzer, OUTPUT) ;
  Serial.begin(9600); //set baud rate
  mySerial.begin(57600);
  bluetooth.begin(9600); //set baud rate 
  pinMode(Open, OUTPUT);
  pinMode(Closed, OUTPUT);
  Status = true;
  message ="";
  identity = "";
  id = 0;
  done = false;
  permissions.Clear();
  lcd.begin(); //Init with pin default ESP8266 or ARDUINO
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Benvenuto!");
  lcd.setCursor(0, 1);
  lcd.print("");
  finger.emptyDatabase();
  getFingerprintEnrollMaster();
  
}

void loop() {

    ButtonState = digitalRead(BUTTON_PIN);
    getFingerprintIDez();
    delay(50);
    lcd.backlight();
    
    while(bluetooth.available()){
          message += char(bluetooth.read());
    }
    if (!bluetooth.available()) {
      if (message != "") { 
      Serial.println(message);
      if(message == "O"){
           Status = true;
           digitalWrite(buzzer, HIGH);
           delay(20);
           digitalWrite(buzzer, LOW) ;
           message = "";
        }else if(message == "C"){
           Status = false;
           digitalWrite(buzzer, HIGH);
           delay(20);
           digitalWrite(buzzer, LOW) ;
           message ="";
        }else if(message == "S"){
           if(Status == true){
             bluetooth.print(0);
           }else if(ButtonState == LOW && Status == false){
             bluetooth.print(1);
           }else if(ButtonState == HIGH && Status == false){
             bluetooth.print(2);
           }
           delay(20);
           message ="";
        }else if(message == "R"){
          done = false;
          while(done != true){
           while(bluetooth.available()){
              identity += char(bluetooth.read());
           }
           delay(5000);
           if(identity != ""){
               Serial.println(identity);
               permissions.PushBack(identity);
               Serial.println(permissions.Find(identity));
               id = permissions.Find(identity)+1;
               Serial.println(id);
               getFingerprintEnroll();
               blackList.PushBack(id);
               done = true;
           }
          }
          identity = "";
          message ="";
        }else if(permissions.Find(message)!= -1){
              Serial.println("found!");
              int id = permissions.Find(message)+1;
              int check = blackList.Find(id);
              if(check == -1){
                 blackList.PushBack(id);
              }else{
                blackList.Erase(check);
              }
              delay(20);
              message = ""; 
           }   
      }
 }
 
 if(Status == true){
    digitalWrite(Open, HIGH);
    digitalWrite(Closed, LOW);
 }else if(Status == false){
    digitalWrite(Closed, HIGH);
    digitalWrite(Open, LOW);
 }

 ButtonState = HIGH;
 identity = "";
 message= "";
}
