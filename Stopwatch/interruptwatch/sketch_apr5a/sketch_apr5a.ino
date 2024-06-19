#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int maxLap = 3;  // maximum num of laps
unsigned long fastestLap = 99999;
int minLapTime = 2000;  // minimum lap time
int lap;
unsigned long start, lapStart, lapTime, now, total;
String line1, line2;

void setup() {
  lcd.init();       // Initialize LCD display
  lcd.backlight();  // Turn on backlight (optional)
  lcd.clear();
  lcd.print("Ramadan RoboRace");
  Serial.begin(9600);
  Serial.println("Ramadan RoboRace");
  delay(2000);
  Serial.println("Ready!");
  line1 = "Ready!";
  attachInterrupt(digitalPinToInterrupt(2), IRdetect, FALLING);  // run IRdetect() when IR is interrupted
  lapStart = millis();
}

void loop() {
  // No need for anything here, as all the work is done inside the interrupt routine.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  delay(50);
}

void IRdetect() {
  now = millis();              // record time right now
  if (lap <= maxLap) {         // only do the rest up to maximum number of laps
    lapTime = now - lapStart;  // calculate lap time

    if (lapTime > minLapTime) {  // only do this if lapTime is more than minimum lap time to prevent false lap
      if (lap > 0) {             // only do this after the car passed sensor for the first time
        if (lapTime < fastestLap) {
          fastestLap = lapTime;
        }
        Serial.print("Lap ");
        Serial.print(lap);
        Serial.print(": ");
        Serial.println(lapTime);
        line1 = "Lap: " + (String)lap;
        line2 = lapTime;
        total += lapTime;  // calculate total time
      } else {
        Serial.println("Start");
        line1 = "Starting...";
      }
      lapStart = now;
      lap++;
      if (lap > maxLap) {  // do this after the last lap
        Serial.print("Stopped... Total time:");
        line1 = "Fastest: " + (String)fastestLap;
        line2 = "Total: " + (String)total;
        Serial.println(total);
      }
    }
  }
}
