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
int lastMode = 0; 
int lastRef= 0;
String tempRef = "";
long tempRefInMs = 0;
long lastDistanceInMs = 0;
boolean refSet = true;
boolean useCM = true; 
boolean matchFound = false; 
long refArray[5] = {200,400,800,1600,3200};

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
  mode name | last mode value
  measure   | 0
  hold      | 1
  reference | 2
  */
  if(lastMode == 0){
    measure();
  }
  else if(lastMode == 1){
    hold();
  }
  else if(lastMode == 2){
    referenceMode();
  }
}

long distanceInMs(){
  //returns a weighted average of the distance and last distance measured, in Ms. 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  lastDistanceInMs = (pulseIn(echoPin, HIGH)*3 + lastDistanceInMs*2)/5;
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
  if(readKeys() == 1 && lastMode != 2){
    lastMode++ ;
    while(readKeys() == 1){}
  }
  else if(readKeys() == 1 && lastMode == 2){
    lastMode = 0; 
    while(readKeys() == 1){}
  }
  Serial.print(" lastMode: ");
  Serial.print(lastMode);
}

void updateUnits(){
  if(readKeys() == 4){
    useCM = !useCM;
    while(readKeys() == 4){}
  }
  Serial.print(" Using CM: ");
  Serial.println(useCM);
}

void measure(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MEASURE");
  lcd.setCursor(0,1);
  long tempMeasurement = distanceInMs();
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
    if(refArray[i] + 50 > tempMeasurement && refArray[i] - 50 < tempMeasurement){
      lcd.print(" =R");
      lcd.print(i + 1);
      lcd.print("/5");
      matchFound = true; 
    }
  }
  Serial.print("Measured: ");
  Serial.println(tempMeasurement);
  delay(250);
}

void hold(){
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
    delay(250);
    lcd.clear();
  }
  
}

void updateRef(){
  if(readKeys() == 0 && lastRef != 4){
    lastRef++;
    while(readKeys() == 0){}
  }
  else if(readKeys() == 0 && lastRef == 4 ){
    lastRef = 0; 
    while(readKeys() == 0){}
  }
  if(readKeys() == 3 && lastRef != 0){
    lastRef--;
    while(readKeys() == 3){}
  }
  else if(readKeys() == 3 && lastRef == 0){
    lastRef = 4;
    while(readKeys() == 3){}
  }
  Serial.print(" last Ref: ");
  Serial.print(lastRef);
}

void referenceMode(){
  updateRef();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("REFERENCE: ");
  lcd.print(lastRef + 1);

  char key = keypad.getKey();
  if (key){
    tempRef = tempRef + key;
    if(useCM = true){
      tempRefInMs = tempRef.toInt()*58;
    }
    else if(useCM = false){
      tempRefInMs = tempRef.toInt()*148;
    }
    refSet = false;
  }
  
  if(readKeys() == 2){
    if(useCM == true){
      refArray[lastRef] = tempRef.toInt()*58; 
    }
    if(useCM == false){
      refArray[lastRef] = tempRef.toInt()*148; 
    }
    while(readKeys() == 2){}
    refSet = true;
    tempRef = "";
    tempRefInMs = 0;
  }

  if(refSet == true){
    lcd.setCursor(0, 1);
    if(useCM == true){
      lcd.print(refArray[lastRef]/58);
      lcd.print("cm");
    }
    else if(useCM == false){
      lcd.print(refArray[lastRef]/148);
      lcd.print("\"");
    }
  }

  if(refSet == false){
    lcd.setCursor(0, 1);
    if(useCM == true){
      lcd.print(tempRefInMs/58);
      lcd.print("cm");
    }
    else if(useCM == false){
      lcd.print(tempRefInMs/148);
      lcd.print("\"");
    }
  }
}


