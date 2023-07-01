#include <Servo.h>
Servo servo;

#define SensorPin A0            // pH meter Analog output to Arduino Analog Input 0
#define Offset 0.25            // deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLength 40    // times of collection
#define TRIGPIN 2
#define ECHOPIN 3

float duration, distance;

int pHArray[ArrayLength];   // Store the average value of the sensor feedback
int pHArrayIndex = 0;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

  servo.attach(9);
  servo.write(90);
  delay(2000);

  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);

  Serial.println("pH meter experiment!");    // Test the serial monitor
}

void loop() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;

  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLength) pHArrayIndex = 0;
    voltage = averageArray(pHArray, ArrayLength) * 5.0 / 1024;
    pHValue = 3.5 * voltage + Offset;
    samplingTime = millis();
  }

  if (millis() - printTime > printInterval) {
    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(pHValue, 2);
    digitalWrite(LED, digitalRead(LED) ^ 1);
    printTime = millis();
  }

  // Sending pH value to ESP32 via UART (TX/RX)
  Serial.print("pH value: ");
  Serial.println(pHValue, 2);
  delay(7000);

  // Measure distance using ultrasonic sensor
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);

  // Set the trigger pin HIGH for 20us to send pulse
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(20);

  // Return the trigger pin to LOW
  digitalWrite(TRIGPIN, LOW);

  // Measure the width of the incoming pulse
  duration = pulseIn(ECHOPIN, HIGH);

  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  // Divide by 1000 as we want millimeters
  distance = (duration / 2) * 0.343;

  // Print result to serial monitor
  Serial.print("distance: ");
  Serial.print(distance);
  Serial.println(" mm");

  if (distance < 500) {
    servo.write(135);
    delay(4000);
    servo.write(45);
    delay(4000);
    servo.write(90);
    delay(1000);
  }

  // Delay before repeating measurement
  delay(1000);
}

double averageArray(int* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;

  if (number <= 0) {
    Serial.println("Error: Invalid array size for averaging!");
    return 0;
  }

  if (number < 5) {   // less than 5, calculate direct statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0];
      max = arr[1];
    } else {
      min = arr[1];
      max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min;        // arr < min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max;    // arr > max
          max = arr[i];
        } else {
          amount += arr[i]; // min <= arr <= max
        }
      }
    }
    avg = (double)amount / (number - 2);
  }
  return avg;
}
