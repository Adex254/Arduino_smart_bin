#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Pins
const int trigPin = 5;
const int echoPin = 6;
const int servoPin = 9;
const int buzzerPin = 8;
const int ldrPin = A0; // Optional: Night mode sensing

// Configuration
const int openDistance = 20; // Open bin if object is within 20cm
const int openAngle = 100;   // Adjust based on your lid hinge
const int closeAngle = 0;

// Variables
long duration;
int distance;
int usageCount = 0;
bool isOpen = false;

Servo lidServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  lidServo.attach(servoPin);
  lidServo.write(closeAngle);
  
  lcd.init();
  lcd.backlight();
  
  // Startup sequence
  lcd.setCursor(0,0);
  lcd.print("SMART BIN V1.0");
  beep(200);
  delay(1000);
  lcd.clear();
}

void loop() {
  // 1. Measure Distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // 2. Logic for opening/closing
  if (distance > 0 && distance < openDistance) {
    if (!isOpen) {
      openBin();
    }
    delay(3000); // Keep open for 3 seconds
  } else {
    if (isOpen) {
      closeBin();
    }
  }

  // 3. Update Display
  lcd.setCursor(0, 0);
  lcd.print("DIST: ");
  lcd.print(distance);
  lcd.print("cm   ");
  
  lcd.setCursor(0, 1);
  lcd.print("USES: ");
  lcd.print(usageCount);
  
  delay(100); 
}

void openBin() {
  lcd.setCursor(10, 1);
  lcd.print("OPEN ");
  beep(100);
  lidServo.write(openAngle);
  isOpen = true;
  usageCount++;
}

void closeBin() {
  lcd.setCursor(10, 1);
  lcd.print("CLOSE");
  lidServo.write(closeAngle);
  delay(500); // Wait for servo to move
  isOpen = false;
  beep(50);
  delay(50);
  beep(50);
}

void beep(int ms) {
  digitalWrite(buzzerPin, HIGH);
  delay(ms);
  digitalWrite(buzzerPin, LOW);
}
