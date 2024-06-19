#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Address 0x27, 16 columns, 2 rows

const int IR_PIN = 6; // IR sensor pin
const int TIMELIMIT = 1 * 15 * 1000; // Time limit of 15 sec
unsigned long lapTimes[3]; // Array to store lap times
unsigned long bestLapTime = WINT_MAX; // Variable to store the best lap time, initialized with maximum value
unsigned long totalTime = 0; // Variable to store total time
int lapCount = 0; // Counter for laps
bool stopwatchRunning = false; // Flag to indicate if the stopwatch is running
unsigned long startTime = 0; // Variable to store start time
unsigned long lastTime = 0; // Variable to store previous lap time
bool firstTrigger = false; // Flag to indicate if the first trigger has occurred

void setup() {
  pinMode(IR_PIN, INPUT);
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on backlight
  lcd.print("IR Stopwatch");
  delay(2000);
  lcd.clear();
}

void loop() {
  int sensorValue = digitalRead(IR_PIN);
  static bool lastState = LOW; // Static variable to store the previous state of the sensor
  unsigned long lapTime = 0;

  // Detect rising edge
  if (sensorValue == HIGH && lastState == LOW) {
    if (!firstTrigger) {
      firstTrigger = true; // Set first trigger flag
    } else {
      if (!stopwatchRunning) {
        startTime = millis(); // Start the stopwatch
        lastTime = millis();
        stopwatchRunning = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Start");
      } else {
        if (lapCount < 3) {
          lapTime = millis() - lastTime; // Calculate lap time
          lapTimes[lapCount] = lapTime; // Record lap time
          lapCount++;
          if (lapTime < bestLapTime) {
            bestLapTime = lapTime; // Update best lap time if current lap is faster
          }
          lastTime = millis(); // Reset start time for the next lap
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Lap ");
          lcd.print(lapCount);
          lcd.print(": ");
          lcd.print(formatTime(lapTime));
        }
      }
    }
  }

  lastState = sensorValue; // Update last state

  // Display current time
  if (stopwatchRunning && lapCount < 3) {
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(formatTime(millis() - startTime));
    lcd.setCursor(0, 0);
    if(lapCount>0) {
      lcd.print("L Time: ");
      lcd.print(formatTime(lastTime));
    }
  }

  // Check if a lap time exceeds 9 minutes (540,000 milliseconds)
  if (lapTime > TIMELIMIT) {
    stopWatch(); // Stop execution
  }

  if (lapCount == 3) {
    for(unsigned long t : lapTimes)
      totalTime += t; // Calculate total time
    stopWatch(); // Stop execution after 3 laps
  }
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

// Function to stop the stopwatch
void stopWatch() {
  lcd.clear();

  // Display best lap time
  lcd.setCursor(0, 0);
  lcd.print("Best: ");
  lcd.print(formatTime(bestLapTime));

  // Display total lap time
  lcd.setCursor(0, 1);
  lcd.print("Total: ");
  lcd.print(formatTime(totalTime));

  while (true); // Stop execution
}
