/*
Mesurement Tool 
By: Ethan Lai
V: April 13th
*/

#include <LiquidCrystal.h>
#include <Keypad.h>
#define powerPin 41
#define trigPin 39
#define echoPin 37
#define groundPin 35
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
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
int mode = 0; 
int referenceIndex= 0;
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

void loop() {
  updateMode();
  updateUnits();
  /*
  mode name | mode value
  measure   | 0
  hold      | 1
  reference | 2
  */
  if(mode == 0){ 
    measure();
  }
  else if(mode == 1){
    hold();
  }
  else if(mode == 2){
    referenceMode();
  }
  Serial.println();
  delay(250);
}

long distanceInMs(){
  //returns a weighted average of the distance and last distance measured, in Ms. 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  lastDistanceInMs = (pulseIn(echoPin, HIGH)*3 + lastDistanceInMs*2)/5;
  if(lastDistanceInMs/58 > 400){
    return(-1);
  }
  else if (lastDistanceInMs/58 < 2) {
    return(-2);
  }
  else{}
  return (lastDistanceInMs);
}

int readKeys(){ 
  int ADCvalue = analogRead(0);        
  if (ADCvalue > 1000) return 5;       
  else if (ADCvalue < 50)   return 0;  //right
  else if (ADCvalue < 195)  return 1;  //up
  else if (ADCvalue < 380)  return 2;  //down
  else if (ADCvalue < 555)  return 3;  //left
  else if (ADCvalue < 790)  return 4;  //select
  else return 5;                        
}

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

void updateUnits(){
  if(readKeys() == 4){
    useCM = !useCM;
    while(readKeys() == 4){}
  }
  Serial.print(" Using CM: ");
  Serial.print(useCM);
}

void measure(){
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
      lcd.print(tempMeasurement/58);
      lcd.print("cm");
    }
    else if(useCM == false){
      lcd.print(tempMeasurement/148);
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

void hold(){
  lcd.clear();
  long held = distanceInMs();
  while(readKeys() != 1){
    updateUnits();
    lcd.setCursor(0,0);
    lcd.print("HELD");
    lcd.setCursor(0,1);
    if(useCM == true){
      lcd.print(held/58);
      lcd.print("cm");
    }
    else if(useCM == false){
      lcd.print(held/148);
      lcd.print("\"");
    }
  }
  
}

void updateRef(){
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

void referenceMode(){
  updateRef();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("REFERENCE: ");
  lcd.print(referenceIndex + 1);

  char key = keypad.getKey();
  if (key){
    temporaryReference = temporaryReference + key;
    if(useCM == true){
      temporaryReferenceInMs = temporaryReference.toInt()*58;
    }
    else if(useCM == false){
      temporaryReferenceInMs = temporaryReference.toInt()*148;
    }
    refSet = false;
  }
  
  

  if(refSet == true){
    lcd.setCursor(0, 1);
    if(useCM == true){
      lcd.print(referenceArray[referenceIndex]/58);
      lcd.print("cm");
    }
    else if(useCM == false){
      lcd.print(referenceArray[referenceIndex]/148);
      lcd.print("\"");
    }
  }

  if(refSet == false){
    lcd.setCursor(0, 1);
    if(useCM == true){
      lcd.print(temporaryReferenceInMs/58);
      lcd.print("cm");
    }
    else if(useCM == false){
      lcd.print(temporaryReferenceInMs/148);
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
      referenceArray[referenceIndex] = temporaryReference.toInt()*58; 
    }
    else if(useCM == false){
      referenceArray[referenceIndex] = temporaryReference.toInt()*148; 
    }
    while(readKeys() == 2){}
    refSet = true;
    temporaryReference = "";
    temporaryReferenceInMs = 0;
  }
}


