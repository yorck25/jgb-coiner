
#include <Wire.h>

//Anschlüsse Sensoren
int SensorSignalSchacht = 9;
int SensorSignalKontrolle = 2;

int CoinIst = 0;                 //Int fürs zählen
volatile int CoinKontrolle = 0;  // Int für die überwachung der richtigen Auszahlung

//Anschlüsse H Brücke
int MotorDrehzahl = 6;
int in1 = 8;
int in2 = 7;
int MotorGeschwindigkeit = 255;
int MotorGeschwindigkeitSlow = 100;

int nummer;  // Für die Auszahlung über Serial Monitor
String str;

bool EncoderClickAble = true;  //Kann der click ausgeführt werden

//Sensoren config
unsigned long Sensor_time = 0;  //Zum Speichern der zeit für den letzten gemessenen Coin
int Sensor_delay_time = 5;    //Wie lange soll der Sensor nach erkennen deaktiviert werden
byte Sensor_State = 0;
byte Sensor_Event = 0;

unsigned long Sensor_time_Kontrolle = 0;  //Zum Speichern der zeit für den letzten gemessenen Coin
int Sensor_delay_time_Kontrolle = 100;    //Wie lange soll der Sensor nach erkennen deaktiviert werden

//Interaktions tracking für timeout
unsigned long Interaktions_time = 0;
bool Interaktions_time_out = true;



void setup() {

  //Motor
  pinMode(MotorDrehzahl, OUTPUT);  //PWM
  pinMode(in1, OUTPUT);            // +
  pinMode(in2, OUTPUT);            // -

  LcdSetup();
  EncoderSetup();
  Serial.begin(9600);

  //attachInterrupt(0, KontrollSensor, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:



  /*if (Serial.available() > 0) {
    // Eingabe lesen:
    nummer = Serial.parseInt();

    // Eingabe ausgeben:
    //Serial.println(nummer);
    str = Serial.readStringUntil('\n');
    CoinsAuszahlen(nummer);
  }*/

  //Serial.println(digitalRead(SensorSignalSchacht));

  LcdStartbild();  //Lcd Standart schrifft

  EncoderLesen();


}


void MotorDrehen(int MotorSpeed) {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(MotorDrehzahl, MotorSpeed);
}

void CoinsAuszahlen(int CoinSoll) {
  MotorDrehen(MotorGeschwindigkeit);
  Serial.println("In Coin Auszahl void");
  CoinIst = 0;
  CoinKontrolle = 0;
  Sensor_time = millis();
  Sensor_time_Kontrolle = millis();


  while (CoinIst != CoinSoll) {

    if (millis() >= Sensor_time + 5000) {
      //Serial.println("Coins sind leer");
      //Serial.print("Es sind nur: ");
      //Serial.print(CoinIst);
      //Serial.print(" von: ");
      //Serial.println(CoinSoll);
      //Serial.print("Es fehlen: ");
      //Serial.println(CoinSoll - CoinIst);
      LcdAbbruchAuszahlung(CoinSoll, CoinIst);
      break;
    }

    if (CoinIst == CoinSoll - 1) {
      MotorDrehen(MotorGeschwindigkeitSlow);
    }
    /*if (digitalRead(SensorSignalSchacht) == LOW && millis() >= Sensor_time + Sensor_delay_time) {
      Sensor_time = millis();
      CoinIst = CoinIst + 1;
      LcdInAuszahlung(CoinSoll, CoinIst);
      Serial.print(CoinIst);
      Serial.print(" ");
      Serial.println(Sensor_time);
      Serial.print(CoinKontrolle);
      Serial.print(" ");
      Serial.print(Sensor_time_Kontrolle);
      Serial.println(" Kontrolle");
    }*/

    switch (Sensor_State) {
    case 0: 
      if (!digitalRead(SensorSignalKontrolle)) {
        Sensor_State = 1;
        Sensor_time = millis();
        Serial.print("LOW: ");
        Serial.println(Sensor_time);
      }
      break;
    case 1: 
      if (digitalRead(SensorSignalKontrolle)) {
        Sensor_State = 0;
        if (millis() - Sensor_time > Sensor_delay_time) {
          Sensor_Event = 1;
          Serial.print("High");
          Serial.println(Sensor_time);
        }
      }
      break;
  }

  //Auswertung Sensor
  if (Sensor_Event == 1) {
    Sensor_Event = 0; //Ereignis zurücksetzten
    CoinIst = CoinIst + 1;
      LcdInAuszahlung(CoinSoll, CoinIst);
      //Serial.print(CoinIst);
      //Serial.print(" ");
      //Serial.println(Sensor_time);
  }
  }
  Serial.println("Auszahlung beendet");
  EncoderClickAble = true;
  Interaktion();

  //Motor Stoppen
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  //Auszahlung überprüfen
  Serial.print(CoinIst);
  Serial.println(" = Coin ist");
  Serial.print(CoinKontrolle);
  Serial.println(" = Coin Kontrolle");

  if (CoinSoll == CoinIst && CoinIst == CoinKontrolle)
    Serial.println("Passt");
  else
    Serial.println("Passt nicht " + CoinKontrolle);
}


void Interaktion() {
  Interaktions_time = millis();
  Interaktions_time_out = true;
}
void KontrollSensor() {
  if (millis() >= Sensor_time_Kontrolle + Sensor_delay_time_Kontrolle) {
    CoinKontrolle = CoinKontrolle + 1;
    Sensor_time_Kontrolle = millis();
  }
}
