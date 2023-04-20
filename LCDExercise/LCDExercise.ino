/*
Test LCD display
*/
//--------------------------------------------------
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  
String name = "Ethan Lai";
void setup() {
  lcd.begin(16,2);
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Ethan Lai");
  for(int i = 0; i < 8; i++){
    delay(100);
    lcd.scrollDisplayRight();
  }
  for(int i = 0; i < 8; i++){
    delay(100);
    lcd.scrollDisplayLeft();
  }
}

void leftRightJustify(){
  lcd.setCursor(0,0);
  lcd.print("Ethan Lai");
  delay(1000);
  lcd.clear();
  lcd.setCursor(16-name.length(), 0);
  lcd.print("Ethan Lai");
  delay(1000);
  lcd.clear();
}
