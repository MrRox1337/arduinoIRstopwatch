#include <LiquidCrystal_I2C.h>

int maxLap = 3; // Maximum number of laps
int minLapTime = 2000; // Minimum lap time in milliseconds (to prevent false laps)
int lap;
unsigned long start, lapStart, lapTime, now, total;  // Initialize total to 0

LiquidCrystal_I2C lcd(0x27, 16, 2); // Replace with your LCD I2C address and pin connections

void setup() {
  Serial.begin(9600);
  Serial.println("Ready!");
  attachInterrupt(digitalPinToInterrupt(2), IRdetect, FALLING); 
  lapStart = millis();
  
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Update the LCD with lap number, lap time (if applicable), and total time
  lcd.clear();
  lcd.print("Lap: ");
  lcd.print(lap);
  lcd.setCursor(0, 1);

  // Calculate and display lap time (if a lap has been completed)
  if (lap > 0) {
    lapTime = now - lapStart;
    int seconds = lapTime / 1000;
    int milliseconds = lapTime % 1000;
    lcd.print("Time: ");
    lcd.print(seconds);
    lcd.print(":");
    lcd.print(milliseconds/100); // Display milliseconds with two digits
  } else {
    lcd.print("Time: --:--");
  }

  lcd.setCursor(0, 2);
  lcd.print("Total: ");
  lcd.print(total / 1000); // Display total time in seconds (adjust as needed)

  // You can also add delay here to control the update rate
  delay(100);
}

void IRdetect() {
  now = millis(); // Record current time
  if (lap < maxLap) {  // Only process up to the maximum number of laps (strictly less than)
    lapTime = now - lapStart; // Calculate lap time
    if (lapTime > minLapTime) { // Only register lap if time exceeds minimum
      lapStart = now;
      lap++;  // Increment lap only after successful lap detection
      if (lap == maxLap) {  // After the last lap
        Serial.print("Stop. Total time: ");
        Serial.println(total);
      }
    }
  }
}