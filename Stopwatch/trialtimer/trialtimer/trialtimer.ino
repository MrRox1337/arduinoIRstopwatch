#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Address 0x27, 16 columns, 2 rows

const int IR_PIN = 6; // IR sensor pin
const int TIMELIMIT = 1 * 15 * 1000; // Time limit of 15 sec
const int LAPS = 3;
unsigned long bestLapTime = TIMELIMIT; // Variable to store the best lap time, initialized with maximum value
unsigned long lapTimes[LAPS]; // Array to store lap times
unsigned long currentLapTime = 0; // Variable to store the current lap time
unsigned long totalTime = 0; // Variable to store total time
int lapCount = 0; // Counter for laps
bool stopwatchRunning = false; // Flag to indicate if the stopwatch is running
bool zeroTrigger = false;

void setup() {
  pinMode(IR_PIN, INPUT);

  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on backlight

  lcd.print("IR Stopwatch");
  delay(1000);
  lcd.clear();
}

void loop() {
  int sensorValue = !digitalRead(IR_PIN);
  static bool lastState = HIGH;

  // Start game on first rising edge
  if (!zeroTrigger && sensorValue) {
    zeroTrigger = true;
  } else {
    if(!stopwatchRunning){
      // If stopwatch is not running, start and initialize timers
      currentLapTime = millis();
      totalLapTime = millis();
      stopwatchRunning = true;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Start");
    } else { // If stopwatch is running then increase lap at every trigger
      record();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lap ");
      lcd.print(lapCount);
      lcd.print(": ");
      lcd.print(formatTime(lapTimes[lapCount-1]));
    }
  }
}

void record() {
  lapTimes[lapCount] = millis() - currentLapTime;
  if(lapTimes[lapCount] < bestLapTime) {
    bestLapTime = lapTimes[lapCount];
  }
  currentLapTime = millis(); // Reset current lap timer
  lapCount++;
}

// Function to format time in milliseconds to MM:ss.mmm
String formatTime(unsigned long milliseconds) {
  unsigned long minutes = milliseconds / 60000;
  unsigned long seconds = (milliseconds % 60000) / 1000;
  unsigned long hundredths = (milliseconds % 1000) / 10;

  String timeString = String(minutes);
  timeString += ":";
  if (seconds < 10) {
    timeString += "0";
  }
  timeString += String(seconds);
  timeString += ".";
  if (hundredths < 10) {
    timeString += "0";
  }
  timeString += String(hundredths);

  return timeString;
}