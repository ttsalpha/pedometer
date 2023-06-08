#include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions for the switches
#define SW1_PIN 2
#define SW2_PIN 3

LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address (0x3F) if necessary

MPU6050 mpu;
int stepCount = 0;
int previousReading = 0;
int stepThreshold = 10000; // Adjust this value as needed
bool isCounting = true;    // Flag to indicate if step counting is active or stopped
int prevSW1State = HIGH;
int currentSW1State = HIGH;
bool switch1Pressed = false;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  mpu.initialize();
  
  // Initialize the LCD display
  lcd.begin(16, 2);
  lcd.backlight();   // Turn on the backlight of the 16x2 LCD.
  
  // Print the initial message on the LCD display
  lcd.setCursor(0, 0);
  lcd.print("Step Count:");
  lcd.setCursor(0, 1);
  lcd.print("0");

  // Set the switch pins as inputs
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
}

void loop() {
  // Read the current acceleration on the Y-axis
  int currentReading = mpu.getAccelerationY();

  // Read the state of switch 1
  currentSW1State = digitalRead(SW1_PIN);

  // Check if the switch 1 is pressed to toggle step counting
  if (currentSW1State == LOW && prevSW1State == HIGH) {
    delay(50); // Debounce delay

    // Toggle the switch1Pressed flag
    switch1Pressed = !switch1Pressed;

    // If counting is resumed, update the previous reading to the current reading
    if (switch1Pressed) {
      previousReading = currentReading;
    }
  }

  // Update the previous switch state
  prevSW1State = currentSW1State;

  // Check if the switch 2 is pressed to reset the step count
  if (digitalRead(SW2_PIN) == LOW) {
    delay(50); // Debounce delay

    // Reset the step count to 0
    stepCount = 0;

    // If counting is stopped, update the LCD display with the reset step count
    if (!isCounting) {
      lcd.setCursor(0, 1);
      lcd.print(stepCount);
    }
  }

  // Control the built-in LED based on the system's activity
  if (isCounting) {
    // Blink the built-in LED at a frequency of 1Hz (0.5s on, 0.5s off)
    digitalWrite(13, (millis() % 1000 < 500) ? HIGH : LOW);
  } else {
    // Turn off the built-in LED to indicate system stoppage
    digitalWrite(13, LOW);
  }

  // Only count steps if the step counting is active
  if (isCounting) {
    // Check if the current reading is above the previous reading plus the step threshold
    if (currentReading > previousReading + stepThreshold) {
      // Increment the step count
      stepCount++;
      
      // Print the step count to the Serial monitor
      Serial.print("Step Count: ");
      Serial.println(stepCount);

      // Update the previous reading with the current reading
      previousReading = currentReading;

      // Update the LCD display with the step count
      lcd.setCursor(0, 1);
      lcd.print(stepCount);

      // Wait for a short period to avoid counting multiple steps within a short time
      delay(500);
    }
    else {
      // Update the previous reading without incrementing the step count
      previousReading = currentReading;
    }
  }
}
