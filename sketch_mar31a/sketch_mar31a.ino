#define DELAY_LENGTH 100
int i = 1;
void setup() {
  Serial.begin(9600);
  DDRA = B11111111;
}
void loop() {
  while(i < 256){
    PORTA = i; 
    i = i*2;
    Serial.println(i);
    delay(DELAY_LENGTH);
  }
   while(i > 1){
    PORTA = i; 
    i = i/2;
    Serial.println(i);
    delay(DELAY_LENGTH);
  }
}
