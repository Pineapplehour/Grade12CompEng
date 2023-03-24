/*Shift Register Exercise
Runs a light back and forth on a bank of leds
Version: March 23, 2023
Made by Ethan Lai
*/

#define DS_PIN 13
#define LATCH_PIN 12
#define CLOCK_PIN 11
#define BAUD_RATE 9600
#define DELAY_LENGTH 10

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(DS_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
}

void loop() {
  //running light forward
  for(int i = 1; i <= 8; i++){
    lightOneLed(i);
    clearShiftRegister(i);
  }
  //running light backwards
  for(int i = 7; i > 1; i--){
    lightOneLed(i);
    clearShiftRegister(i);
  }
}


void pulsePin(int pin, int delayLength){
  //sends a pulse through a selected pin, pulse length set by "delaylatch"
  digitalWrite(pin, HIGH);
  delay(delayLength);
  digitalWrite(pin, LOW);
}

void lightOneLed(int led){
//Lights one led. Use in conjuction with clearRegister()
//Parameter: Position of LED, 1 being the first Flip Flop
  //toggle first filp flop 
  digitalWrite(DS_PIN, HIGH);
  pulsePin(CLOCK_PIN, DELAY_LENGTH);
  Serial.print(1);
  digitalWrite(DS_PIN, LOW);
  //shift to indicated flip flip 
  for(int i = 1; i < led; i++){
    pulsePin(CLOCK_PIN, DELAY_LENGTH);
    Serial.print(0);
  }
  //latch
  pulsePin(LATCH_PIN, DELAY_LENGTH);
  Serial.println(" LATCHED");
}

void clearShiftRegister(int lastLit){
//pushes all data off shift register, DOES NOT CLEAR LEDS
//Parameter: Position of latest loaded flip flop, 1 being the first Flip Flop
  for(int i = lastLit; i <= 8; i++){
    pulsePin(CLOCK_PIN, DELAY_LENGTH);
    Serial.print(0);
  }
}