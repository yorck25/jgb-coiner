#include <LiquidCrystal_I2C.h>
unsigned long lastwriteLCD = 0;
//LCD Inizialisieren
LiquidCrystal_I2C lcd(0x27, 16, 2);

void LcdSetup(){
    //initialize lcd screen
  lcd.init();
  // turn on the backlight
  lcd.backlight();
}

void LcdInAuszahlung(int s, int i) {
  lcd.clear();
  lcd.print("Auszahlung: ");
  lcd.setCursor(0, 1);
  lcd.print(i);
  lcd.print("/");
  lcd.print(s);
  if (s == i) {
    lcd.clear();
    lcd.print("Fertig");
    lcd.setCursor(0, 1);
    lcd.print(s);
    lcd.print(" Coins");
  }
}

void LcdAbbruchAuszahlung(int s, int i) {
  lcd.clear();
  lcd.print("Error: ");
  lcd.print(i);
  lcd.print(" von ");
  lcd.print(s);
  lcd.setCursor(0, 1);
  lcd.print("Es fehlen: ");
  lcd.print(s - i);
}

void LcdEncoder(int a) {
  if (millis() - lastwriteLCD > 200) {
    lastwriteLCD = millis();
    lcd.clear();
    lcd.print("Coins: ");
    lcd.print(a);
  }
}

void LcdStartbild() {
  if (millis() >= Interaktions_time + 10000 && Interaktions_time_out == true) {
    lcd.clear();
    lcd.print("JGB Coiner");
    lcd.setCursor(0, 1);
    lcd.print("Bereit");
    Interaktions_time_out = false;
    anzahl = 0;
  }
}