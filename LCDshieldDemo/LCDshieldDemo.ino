/* LCDshieldDemo
 * This program demonstrates how to display messages on 16x2 LCD shield. 
 * Library originally added 18 Apr 2008
 * by David A. Mellis
 * library modified 5 Jul 2009
 * by Limor Fried (http://www.ladyada.net)
 * example added 9 Jul 2009
 * by Tom Igoe
 * modified 22 Nov 2010
 * by Tom Igoe
 * Version: 14/04/2014
 * Adaptation: Mr.G
 */
// -------------------------------------
#include <LiquidCrystal.h>                // include the library code
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);      // select the pins used on the LCD panel
int counter = 0;                          // variable to count number of repetitions

// -------------------------------------
void setup() {
    lcd.begin(16, 2);                     // start the LCD with 2 rows and 16 columns
    lcd.print("Hello World!");            // print a generic message to the LCD
    delay(500);
}

// -------------------------------------
void loop() {
    lcd.setCursor(0, 1);                  // set the cursor to column 0, line 1 (second row, since counting begins from 0):
    lcd.print(counter);                   // print the counter to the LCD 
    counter=counter + 1;                  // increase the counter
    delay(500);
}
