#include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address (0x3F) if necessary

MPU6050 mpu;
int stepCount = 0;
int previousReading = 0;
int stepThreshold = 10000; // Adjust this value as needed

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    mpu.initialize();

    // Initialize the LCD display
    lcd.begin(16, 2);
    lcd.backlight(); // Turn on the backlight of the 16x2 LCD.

    // Print the initial message on the LCD display
    lcd.setCursor(0, 0);
    lcd.print("Step Count:");
}

void loop()
{
    // Read the current acceleration on the Y-axis
    int currentReading = mpu.getAccelerationY();

    // Check if the current reading is above the previous reading plus the step threshold
    if (currentReading > previousReading + stepThreshold)
    {
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
    else
    {
        // Update the previous reading without incrementing the step count
        previousReading = currentReading;
    }
}
