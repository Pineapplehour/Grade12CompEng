/*Shift Register Exercise
Runs a light back and forth on a bank of leds
Version: March 23, 2023
Made by Ethan Lai
*/

#define DS_PIN 13
#define LATCH_PIN 12
#define CLOCK_PIN 11
int BAUD_RATE = 9600;
int DELAY_LENGTH = 100; 
int patternArray[14][8] = {
  {1,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0},
  {0,0,0,1,0,0,0,0},
  {0,0,0,0,1,0,0,0},
  {0,0,0,0,0,1,0,0},
  {0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,1,0},
  {0,0,0,0,0,1,0,0},
  {0,0,0,0,1,0,0,0},
  {0,0,0,1,0,0,0,0},
  {0,0,1,0,0,0,0,0},
  {0,1,0,0,0,0,0,0},
};
void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(DS_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
}

void loop() {
  runLightForwards();
  runLightBackwards();
}


void pulsePin(int pin, int delayLength){
  //sends a pulse through a selected pin, pulse length set by "delaylatch"
  digitalWrite(pin, HIGH);
  delay(delayLength);
  digitalWrite(pin, LOW);
}

void displayPatternArray(){
//displays the pattern set in the pattern array
//traverse pattern array 
  for(int r = 0; r < 14; r++){
    for(int c = 0; c < 8; c++){
      //shift out one row of the array into the shift register 
      digitalWrite(DS_PIN, patternArray[r][c]);
      pulsePin(CLOCK_PIN, DELAY_LENGTH);
      Serial.print(patternArray[r][c]);
    }
    //latch shift register  
    pulsePin(LATCH_PIN, DELAY_LENGTH);
    Serial.println(" Light Set");
  }
}

void runLightForwards() {
//moves lit LED forwards
  digitalWrite(DS_PIN, HIGH);
  pulsePin(CLOCK_PIN, DELAY_LENGTH);
  digitalWrite(DS_PIN, LOW);
  pulsePin(LATCH_PIN, DELAY_LENGTH);
  for(int i = 1; i <= 8; i++){
    pulsePin(LATCH_PIN, DELAY_LENGTH);
    pulsePin(CLOCK_PIN, DELAY_LENGTH);
  }
}

void runLightBackwards(){
//moves lit LED backwards
  for(int i = 7; i >= 0; i--){
    digitalWrite(DS_PIN, HIGH);
    pulsePin(CLOCK_PIN, DELAY_LENGTH);
    digitalWrite(DS_PIN, LOW);
    Serial.print(1);
    for (int j = i; j > 0; j--){
      pulsePin(CLOCK_PIN, DELAY_LENGTH);
      Serial.print(0);
    }
    pulsePin(LATCH_PIN, DELAY_LENGTH);
    Serial.println(" LATCHED");
    for (int p = 7-i; p > 0; p--){
      pulsePin(CLOCK_PIN, DELAY_LENGTH);
      Serial.print(0);
    }
  }
}
  