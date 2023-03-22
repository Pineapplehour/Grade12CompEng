int DS_PIN = 2;
int LATCH_PIN = 3;
int CLOCK_PIN = 4;
int BAUD_RATE = 9600;
int DELAY_LENGTH = 50; 
int patternArray[8][8] = {
  {1,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0},
  {0,0,0,1,0,0,0,0},
  {0,0,0,0,1,0,0,0},
  {0,0,0,0,0,1,0,0},
  {0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,1}
};
void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(DS_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
}

void loop() {
  for(int r = 0; r < 8; r++){
    for(int c = 0; c < 8; c++){
      digitalWrite(DS_PIN, patternArray[r][c]);
      pulsePin(CLOCK_PIN, DELAY_LENGTH);
    }
    pulsePin(LATCH_PIN, DELAY_LENGTH);
  }
}

void pulsePin(int pin, int delayLength){
  digitalWrite(pin, HIGH);
  delay(delayLength);
  digitalWrite(pin, LOW);
}