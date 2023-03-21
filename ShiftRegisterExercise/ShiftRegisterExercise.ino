int DS_PIN = 2;
int LATCH_PIN = 3;
int CLOCK_PIN = 4;
int BAUD_RATE = 9600;
int delayLength = 250; 
int patternArray[]= {1,2,4,8,16,32,64,128};
void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(DS_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
}

void loop() {
  for(int r = 0; r < sizeof(patternArray)/sizeof(int); r++){
  shiftOut(DS_PIN, CLOCK_PIN, MSBFIRST, patternArray[r]);
  pulsePin(LATCH_PIN, delayLength);
  Serial.println(patternArray[r]);
  }
}

void pulsePin(int pin, int delayLength){
  digitalWrite(pin, HIGH);
  delay(delayLength);
  digitalWrite(pin, LOW);
}