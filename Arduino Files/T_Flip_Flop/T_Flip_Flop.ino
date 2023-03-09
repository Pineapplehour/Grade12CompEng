int Q = 2;
int QBAR = 3;
int PUSH_BUTTON = 5;
int BAUD_RATE = 9600;
int qState = 0;
int qBarState = 1;
void setup() 
{
  Serial.begin(BAUD_RATE);
  pinMode(Q, OUTPUT);
  pinMode(QBAR, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT);
}

void loop() 
{
  if(digitalRead(PUSH_BUTTON) == 1)
  {
    q = !q;
    qBar = !q;
    digitalWrite(Q, qState);
    digitalWrite(QBAR, qBarState);
    Serial.write("State Switched");
  }
}
