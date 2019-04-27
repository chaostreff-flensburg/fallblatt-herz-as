//
// https://github.com/chaostreff-flensburg/faltblatt-herz-as
// https://eichhorn.dev
// Chaostreff Flensburg Alexander @Pfannkuchensack Eichhorn
//
int state1 = 0; // state of the output1; 0 --> OFF, 1 --> ON
int state2 = 0; // state of the output1; 0 --> OFF, 1 --> ON
int state3 = 0; // state of the output1; 0 --> OFF, 1 --> ON
int state4 = 0; // state of the output1; 0 --> OFF, 1 --> ON
unsigned long currentMillis = millis(); // derive actual timestamp
unsigned long startMillis = millis(); // derive actual timestamp
int pulsewidth_1 = 1;
// Mapping: E=leer
String names[] = { "E", "A", "2", "K", "3", "D", "4", "B", "5", "0", "6", "9", "7", "8", "8", "7", "9", "6", "0", "5", "B", "4", "D", "3", "K", "2", "A", "2", "A", "3", "K", "4", "D", "5", "B", "6", "0", "7", "9", "8", "8", "9", "7", "10", "6", "B", "5", "D", "4", "K", "3", "A", "2", "E", "E", "E", "E", "E", "E", "E", "E" };

#include "Wire.h"
#define DS1307_ADDRESS 0x68

byte bcdToDec(byte val)  {
  // Convert binary coded decimal to normal decimal numbers
  return ( (val / 16 * 10) + (val % 16) );
}

// Setze Flatblatt zurück damit das Mapping arbeiten kann
bool nullpunkt(int pin, int gpin, int state)
{
  for (int j = 0; j <= 64; j++)
  {
    for (int i = 0; i <= 4; i++) {
      if (digitalRead(gpin) == HIGH)
      {
        if (state == 1)
          state1 = 18;
        if (state == 2)
          state2 = 18;
        if (state == 3)
          state3 = 18;
        if (state == 4)
          state4 = 18;
        return true;
      }
      digitalWrite(pin, HIGH);
      delay(6);
      digitalWrite(pin, LOW);
      delay(6);
    }
  }
}

// Wechsel zu bestimmte Faltblatt auf bestimmten Pin
bool changeto(int pin, int state, String newstate)
{
  if(newstate == "1")
    newstate = "A";
  if(newstate == "0")
    newstate = "D";
  for (int j = 0; j <= 61; j++)
  {
    for (int i = 0; i <= 3; i++) {  
      digitalWrite(pin, HIGH);
      delay(6);
      digitalWrite(pin, LOW);
      delay(6);
    }
    if (state == 1)
      state1++;
    if (state == 2)
      state2++;
    if (state == 3)
      state3++;
    if (state == 4)
      state4++;

    // DEBUG
    /*Serial.print(newstate);
    Serial.print(" - ");
    if (state == 1)
      Serial.print(names[state1]);
    if (state == 2)
      Serial.print(names[state2]);
    if (state == 3)
      Serial.print(names[state3]);
    if (state == 4)
      Serial.print(names[state4]);
    Serial.print("\r\n");*/
    
    if (state == 1 && names[state1] == newstate)
      return true;
    if (state == 2 && names[state2] == newstate)
      return true;
    if (state == 3 && names[state3] == newstate)
      return true;
    if (state == 4 && names[state4] == newstate)
      return true;
    // RESET
    if (state == 1 && state1 >= 60)
      state1 = 0;
    if (state == 2 && state2 >= 60)
      state2 = 0;
    if (state == 3 && state3 >= 60)
      state3 = 0;
    if (state == 4 && state4 >= 60)
      state4 = 0;
   }
}

// Uhrzeit auslesen plus Anzeigen
void printDate() {

  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  //int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  //int monthDay = bcdToDec(Wire.read());
  //int month = bcdToDec(Wire.read());
  //int year = bcdToDec(Wire.read());

  //print the date EG   3/1/11 23:59:59
  //Serial.print(month);
  //Serial.print("/");
  //Serial.print(monthDay);
  //Serial.print("/");
  //Serial.print(year);
  //Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

  if(hour > 9)
  {
    changeto(5, 1, (String)(hour / 10U % 10));
    changeto(6, 2, (String)(hour / 1U % 10));
  }
  else
  {
    changeto(5, 1, "D");
    changeto(6, 2, (String)String(hour));  
  }
  if(minute > 9)
  {
    changeto(9, 3, (String)(minute / 10U % 10));
    changeto(10, 4, (String)(minute / 1U % 10));
  }
  else
  {
    changeto(9, 3, "D");
    changeto(10, 4, (String)String(minute));
  }
}

void setup() {
  // initialize the digital pin as an output.

  Wire.begin();
  // debug
  //Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);

  //////////////////////////////

  digitalWrite(5, LOW);    // turn the output off
  digitalWrite(6, LOW);    // turn the output off
  digitalWrite(9, LOW);    // turn the output off
  digitalWrite(10, LOW);    // turn the output off
  currentMillis = millis(); // derive actual timestamp
  startMillis = millis(); // derive actual timestamp

  nullpunkt(5,7,1);
  nullpunkt(6,8,2);
  nullpunkt(9, 12, 3);
  nullpunkt(10, 13, 4);
  /*for(int h = 0; h <= 4; h++)
  {
    //changeto(5, 1, "A");
    //changeto(6, 2, "A");
    changeto(9, 3, "A");
    changeto(10, 4, "A");
  }*/
}
/////////////////////////////

void loop() {
  // Debug ABBA
  //changeto(5, 1, "A");
  //changeto(6, 2, "B");
  //changeto(9, 3, "B");
  //changeto(10, 4, "A");
  //delay(2000);
  // Zeige alle 30 sekunden die Aktuelle ausgelesene Uhrzeit an
  printDate();
  delay(30000);
}
