#include "DCF77.h"  // inkludiert die Bibliotheken


#define DCF_PIN 2                     // definiert den Pin zum DCF77-Gerät
#define DCF_INTERRUPT 0               // als 0 definiert, wird verwendet um das Zeitsignal der Funkuhr zu empfangen
#define BLAU pixels.Color(0, 0, 255)  // definiert die Farbe der Pixel

time_t time;                                      // die aktuelle Zeit
DCF77 DCF = DCF77(DCF_PIN, DCF_INTERRUPT, true);  

#include <Adafruit_NeoPixel.h>  //inkludiert die Bib um mit LEDs zu kommunizieren
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6                                                    // An pin 6 sind die LEDs angeschlossen
#define NUMPIXELS 64                                             // Anzahl der angeschlossenen LEDs
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  // Neo_ -> Farbordnungsmodus und Datenrate

#define DELAYVAL 500  // gibt Verzögerungswert für die Verspätung der Aktionen im Code

int stunde = 0;  // als 0 deklariert
int h = 11;
int m = 40;
int s = 0;

void setup() {
  Serial.begin(9600);  //Serial Monitor aktivieren
  DCF.Start();
  Serial.println("Waiting for DCF77 time ... ");
  Serial.println("It will take at least 2 minutes before a first time update.");
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  pixels.begin();
  setTime(h, m, s, 1, 1, 23);
}


void setBlue(int start, int end) { // gibt an wie du die einzelnen Wörter mit einer schreibweise aufleuchten lassen kannst. Beispiel Startwert ist 17 und Endwert ist 20 -> es leuchtet das Wort DREI 
  for (int i = start; i <= end; i++) {
    pixels.setPixelColor(i, BLAU);
  }
  pixels.show();
}

void digitalClockDisplay() { // Was soll sozusagen alles auf der Uhr zu sehen sein
  Serial.println(hour());
  Serial.println(minute());  //serieller Monitor zu Kontrollzwecken
  Serial.println(second());
  printHour();
  printMinute();
  printFrame();
}

void printMinute() {
  int aktuelleMinute = minute();
  if ((3 <= aktuelleMinute && aktuelleMinute <= 7) || (13 <= aktuelleMinute && aktuelleMinute <= 17) || (23 <= aktuelleMinute && aktuelleMinute <= 27) || (33 <= aktuelleMinute && aktuelleMinute <= 37) || (43 <= aktuelleMinute && aktuelleMinute <= 47) || (53 <= aktuelleMinute && aktuelleMinute <= 57)) {
    setBlue(60, 63);
    //FÜNF leuchtet
  }
  if ((8 <= aktuelleMinute && aktuelleMinute <= 12) || (13 <= aktuelleMinute && aktuelleMinute <= 17) || (18 <= aktuelleMinute && aktuelleMinute <= 22) || (38 <= aktuelleMinute && aktuelleMinute <= 42) || (43 <= aktuelleMinute && aktuelleMinute <= 47) || (48 <= aktuelleMinute && aktuelleMinute <= 52)) {
    setBlue(56, 59);
    //ZEHN leuchtet
  }
}

void editHour() { // ist die Funktion damit beispielsweise bei 7:30 nicht halb 7 angezeigt wird sondern halb 8
  int aktuelleMinute = minute();
  if (0 <= aktuelleMinute && aktuelleMinute <= 17) {
    stunde = hour();
  } else {
    stunde = hour() + 1;
  }
}

void printHour() { // Beispiel: wenn 18 Uhr ist soll die Uhr 6 anzeigen. 
  editHour();
  switch (stunde) {
    case 12:
    case 0:
    case 24:  // schreibe zwölf
      setBlue(24, 28);
      break;
    case 13:
    case 1:
      setBlue(36, 39);
      break;
    case 14:
    case 2:
      setBlue(17, 18);
      setBlue(27, 28);
      break;
    case 15:
    case 3:
      setBlue(17, 20);
      break;
    case 16:
    case 4:
      setBlue(40, 43);
      break;
    case 17:
    case 5:
      setBlue(0, 0);
      setBlue(8, 8);
      setBlue(16, 16);
      setBlue(24, 24);
      break;
    case 18:
    case 6:
      setBlue(32, 36);
      break;
    case 19:
    case 7:
      setBlue(21, 23);
      setBlue(29, 31);
      break;
    case 20:
    case 8:
      setBlue(3, 6);
      break;
    case 21:
    case 9:
      setBlue(8, 11);
      break;
    case 22:
    case 10:
      setBlue(11, 14);
      break;
    case 23:
    case 11:
      setBlue(0, 2);
      break;
  }
}

void printFrame() {// sagt aus wann VOR, NACH oder HALB aufleuchten soll
  int aktuelleMinute = minute();

  if ((18 <= aktuelleMinute && aktuelleMinute <= 27) || (43 <= aktuelleMinute && aktuelleMinute <= 57)) {
    setBlue(52, 54);
    //VOR leuchtet 
  }
  if ((3 <= aktuelleMinute && aktuelleMinute <= 17) || (33 <= aktuelleMinute && aktuelleMinute <= 42)) {
    setBlue(48, 51);
    //NACH leuchtet 
  }
  if (18 <= aktuelleMinute && aktuelleMinute <= 42) {
    setBlue(44, 47);
    //HALB leuchtet
  }
}
//dcffunktioniertnicht();{
/*if (s < 60){
  s = s + 1;
}
if (s == 60){
  m = m + 1;
  s = 0;
}
if (m < 60){
  m = m + 1;
}
if (m == 60){
  h = h + 1;
  s = 0;
}
if (h == 24){
  h = 0;
}
setTime(h, m, s, 1, 1, 23);
}
*/





void loop() {
  pixels.clear();
  /* time_t DCFtime = DCF.getTime();  // Check if new DCF77 time is available
   if (DCFtime != 0) {
    Serial.println("Time is updated");
     setTime(DCFtime);
   }*/
  //dcffunktioniertnicht();
  digitalClockDisplay();
  delay(1000);
}
