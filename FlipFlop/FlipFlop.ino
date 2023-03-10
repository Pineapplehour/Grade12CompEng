int Q = 2;
int QBAR = 3;
int CLOCK_BUTTON = 5;
int DIP_SWITCH_2 = 6;
int DIP_SWITCH_1 = 7;
int BAUD_RATE = 9600;
int lastButtonState = 0;
int lastDip1State = 0;
int lastDip2State = 0;
int q;
int qBar;
void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(Q, OUTPUT);
  pinMode(QBAR, OUTPUT);
  pinMode(CLOCK_BUTTON, INPUT);
  pinMode(DIP_SWITCH_2, INPUT);
  pinMode(DIP_SWITCH_1, INPUT);
  tFlipFlop();
}

void tFlipFlop(){
  while(true){
    delay(250);
    if(digitalRead(CLOCK_BUTTON) == 0 && digitalRead(DIP_SWITCH_1) == 1 && lastDip1State != digitalRead(DIP_SWITCH_1)){
      q = !q;
      qBar = !q; 
      Serial.print("Q: ");
      Serial.print(q);
      Serial.print(", QBAR: ");
      Serial.print(qBar);
      digitalWrite(Q,q);
      digitalWrite(QBAR, qBar);
      Serial.print(", State Switched, ");
    }
    
    lastButtonState = digitalRead(CLOCK_BUTTON);
    Serial.print("Clock button: ");
    Serial.print(lastButtonState);
    lastDip1State = digitalRead(DIP_SWITCH_1);
    Serial.print(", Dip switch: ");
    Serial.println(lastDip1State);
  }
}
