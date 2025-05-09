#include <Encoder.h>

//Encoder config
const int CLK = 5;             // Definition der Pins. CLK an D6, DT an D5.
const int DT = 4;
const int SW = 3;              // Der Switch wird mit Pin D2 Verbunden. ACHTUNG : Verwenden Sie einen interrupt-Pin!
long alteAnzahl = -999;        // Definition der "alten" Position (Diese fiktive alte Position wird benötigt, damit die aktuelle Position später im seriellen Monitor nur dann angezeigt wird, wenn wir den Rotary Head bewegen)
unsigned long Click_time = 0;  //Zum Speichern der zeit für den letzten Click
int Click_delay_time = 1000;   //Wie lange soll der Knopf nach dem Clicken aus sein

int anzahl = 0;                // Int für den encoder

Encoder meinEncoder(DT, CLK);  // An dieser Stelle wird ein neues Encoder Projekt erstellt. Dabei wird die Verbindung über die zuvor definierten Varibalen (DT und CLK) hergestellt.

void EncoderSetup(){
  //Encoder Click
  pinMode(SW, INPUT_PULLUP);
}

void EncoderLesen(){
    //Anzahl der Coins die augezahlt werden über Encoder abfragen
  int32_t neueAnzahl = meinEncoder.read();
  bool Encoder_Click = digitalRead(SW);
  if (neueAnzahl <= -3) {
    if (anzahl > 0) {
      anzahl -= 1;
      //Serial.println(anzahl);
      LcdEncoder(anzahl);
    }
    meinEncoder.write(neueAnzahl + 4);
    Interaktion();

  } else if (neueAnzahl >= 3) {
    anzahl += 1;
    //Serial.println(anzahl);
    LcdEncoder(anzahl);
    Interaktion();
    meinEncoder.write(neueAnzahl - 4);
  }
    //Per Encoder Click in die auszahlung
  if (Encoder_Click == LOW && millis() >= Click_time + Click_delay_time && EncoderClickAble == true) {
    Click_time = millis();
    EncoderClickAble = false;
    LcdInAuszahlung(anzahl, 0);
    CoinsAuszahlen(anzahl);
    anzahl = 0;
  }
}