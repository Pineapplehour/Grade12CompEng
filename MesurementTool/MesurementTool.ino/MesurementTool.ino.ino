/*
Mesurement Tool 
Summary: Runs ultrasonic sensor tool 
By: Ethan Lai
V: April 13th
*/
#include <LiquidCrystal.h>
#include <Keypad.h>
#define powerPin 41
#define trigPin 39
#define echoPin 37
#define groundPin 35
//LCD 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
//keypad 
const byte ROWS = 4;                   
const byte COLS = 4;                   
char key;                              
char keys[ROWS][COLS] = {              
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}      
  };
byte rowPins[ROWS] = {14, 15, 16, 17}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {18, 19, 20, 21}; // connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 
//other
int mode = 0; 
int referenceIndex= 0;
const int CM_CONSTANT = 58;
const int INCH_CONSTANT = 148;
String temporaryReference = "";
long temporaryReferenceInMs = 0;
long lastDistanceInMs = 0;
boolean refSet = true;
boolean useCM = true; 
boolean matchFound = false; 
long referenceArray[5] = {200,400,800,1600,3200};

void setup() {
 Serial.begin(9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(powerPin, OUTPUT);
 pinMode(groundPin, OUTPUT);
 digitalWrite(powerPin, HIGH);
 digitalWrite(groundPin, LOW);
 lcd.begin(16,2);
}
//--------------------------------------------------
void loop() {
  updateMode();
  updateUnits();
  if(mode == 0){ //mesure mode 
    measure();
    delay(250);
  }
  else if(mode == 1){ //hold mode
    hold();
    delay(250);
  }
  else if(mode == 2){ //reference mode
    referenceMode();
    delay(100);
  }
  
  Serial.println();
}
//--------------------------------------------------
long distanceInMs(){
  //returns a weighted average of the distance and last distance measured, in milliseconds
  lastDistanceInMs = 0;
  for(int i = 0; i < 10; i++){
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    lastDistanceInMs = lastDistanceInMs + pulseIn(echoPin, HIGH);
    delay(1);
  }
  lastDistanceInMs = lastDistanceInMs/10;
  if(lastDistanceInMs/CM_CONSTANT > 400){
    return(-1);
  }
  else if (lastDistanceInMs/CM_CONSTANT < 2) {
    return(-2);
  }
  else{}
  return (lastDistanceInMs);
}
//--------------------------------------------------
int readKeys(){ 
  //returns which lcd key has been pressed
  int ADCvalue = analogRead(0);        
  if (ADCvalue > 1000) return 5;       
  else if (ADCvalue < 50)   return 0;  //right
  else if (ADCvalue < 195)  return 1;  //up
  else if (ADCvalue < 380)  return 2;  //down
  else if (ADCvalue < 555)  return 3;  //left
  else if (ADCvalue < 790)  return 4;  //select
  else return 5;                        
}
//--------------------------------------------------
void updateMode(){
  //when up key is pressed, the mode cycles. 
  if(readKeys() == 1 && mode != 2){
    mode++ ;
    while(readKeys() == 1){}
  }
  else if(readKeys() == 1 && mode == 2){
    mode = 0; 
    while(readKeys() == 1){}
  }
  Serial.print(" Mode: ");
  Serial.print(mode);
}
//--------------------------------------------------
void updateUnits(){
  //when select key is pressed, the units cycle
  if(readKeys() == 4){
    useCM = !useCM;
    while(readKeys() == 4){}
  }
  Serial.print(" Using CM: ");
  Serial.print(useCM);
}
//--------------------------------------------------
void measure(){
  //meaure mode 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MEASURE");
  lcd.setCursor(0,1);
  long tempMeasurement = distanceInMs();
  if(tempMeasurement == -1){
    lcd.print("Out of Range");
  }
  else if(tempMeasurement == -2){
    lcd.print("Too close");
  }
  else{
    if(useCM == true){
      lcd.print(tempMeasurement/CM_CONSTANT);
      lcd.print("cm");
    }
    else if(useCM == false){
      lcd.print(tempMeasurement/INCH_CONSTANT );
      lcd.print("\"");    
    }
    matchFound = false; 
    for(int i = 0; i < 4 && !matchFound; i++){
      if(referenceArray[i] + 50 > tempMeasurement && referenceArray[i] - 50 < tempMeasurement){
        lcd.print(" =R");
        lcd.print(i + 1);
        lcd.print("/5");
        matchFound = true; 
      }
    }
  }
  Serial.print(" Measured: ");
  Serial.print(tempMeasurement);
}
//--------------------------------------------------
void hold(){
  //hold mode
  long held = distanceInMs();
  while(readKeys() != 1){
    updateUnits();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("HELD");
    lcd.setCursor(0,1);
    if(useCM == true){
      lcd.print(held/CM_CONSTANT);
      lcd.print("cm");
    }
    else if(useCM == false){
      lcd.print(held/INCH_CONSTANT );
      lcd.print("\"");
    }
    Serial.println();
  }
  updateMode();
}
//--------------------------------------------------
void updateRef(){
  //cycles through references using left and right lcd keys 
  if(readKeys() == 0 && referenceIndex != 4){
    referenceIndex++;
    while(readKeys() == 0){}
  }
  else if(readKeys() == 0 && referenceIndex == 4 ){
    referenceIndex = 0; 
    while(readKeys() == 0){}
  }
  if(readKeys() == 3 && referenceIndex != 0){
    referenceIndex--;
    while(readKeys() == 3){}
  }
  else if(readKeys() == 3 && referenceIndex == 0){
    referenceIndex = 4;
    while(readKeys() == 3){}
  }
  Serial.print(" Reference Index: ");
  Serial.print(referenceIndex);
}
//--------------------------------------------------
void referenceMode(){
  //reference mode
  updateRef();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("REFERENCE: ");
  lcd.print(referenceIndex + 1);
  char key = keypad.getKey();
  if (key){
    temporaryReference = temporaryReference + key;
    if(useCM == true){
      temporaryReferenceInMs = temporaryReference.toInt()*CM_CONSTANT;
    }
    else if(useCM == false){
      temporaryReferenceInMs = temporaryReference.toInt()*INCH_CONSTANT ;
    }
    refSet = false;
  }
  if(refSet == true){
    lcd.setCursor(0, 1);
    if(useCM == true){
      lcd.print(referenceArray[referenceIndex]/CM_CONSTANT);
      lcd.print("cm");
    }
    else if(useCM == false){
      lcd.print(referenceArray[referenceIndex]/INCH_CONSTANT );
      lcd.print("\"");
    }
  }
  if(refSet == false){
    lcd.setCursor(0, 1);
    if(useCM == true){
      lcd.print(temporaryReferenceInMs/CM_CONSTANT);
      lcd.print("cm");
    }
    else if(useCM == false){
      lcd.print(temporaryReferenceInMs/INCH_CONSTANT );
      lcd.print("\"");
    }
  }
  if(readKeys() == 2){
    if(temporaryReference.toInt() > 400 && useCM == true){
      lcd.setCursor(0, 1);
      lcd.print("Out of range");
      delay(500);
    }
    else if (temporaryReference.toInt() > 157 && useCM == false) {
      lcd.setCursor(0, 1);
      lcd.print("Out of range");
      delay(500);
    }
    else if(useCM == true){
      referenceArray[referenceIndex] = temporaryReference.toInt()*CM_CONSTANT; 
      lcd.setCursor(0, 1);
      lcd.print("Saved");
      delay(500);
    }
    else if(useCM == false){
      referenceArray[referenceIndex] = temporaryReference.toInt()*INCH_CONSTANT ; 
      lcd.setCursor(0, 1);
      lcd.print("Saved");
      delay(500);
    }
    while(readKeys() == 2){}
    refSet = true;
    temporaryReference = "";
    temporaryReferenceInMs = 0;
  }
}


