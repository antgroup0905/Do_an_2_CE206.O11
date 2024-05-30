#include <OneWire.h>
#include <DallasTemperature.h>
#include <pHSensor.h>
#include <TurbiditySensor.h>

#define pHPin A0        // pH meter Analog output to Arduino Analog Input 0
#define turbidityPin A1 // Turbidity meter Analog output to Arduino Analog Input 1
#define Offset 0.25     // deviation compensate
#define LED 13
#define samplingInterval 20 // Sampling interval in milliseconds
#define ArrayLength 40      // times of collection
#define ONE_WIRE_BUS 3

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temperatureSensor(&oneWire);
TurbiditySensor myTurbiditySensor(turbidityPin);
pHSensor mypHSensor(pHPin, samplingInterval, ArrayLength, Offset);

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  temperatureSensor.begin();
  //  Serial.println("pH meter experiment!");	// Test the serial monitor
}

void loop()
{
  // Run the function of pH sensor
  // float pHValue = mypHSensor.getpHValue();
  // delay(100);

  // Run the function of Turbidity sensor
  // float ntu = myTurbiditySensor.getTurbidityValue();
  // delay(100);

  // Run the function of Temperature sensor
  temperatureSensor.requestTemperatures();
  float temperature = temperatureSensor.getTempCByIndex(0);
  delay(100);

  // Sending value to ESP32 via UART (TX/RX)
  // Serial.print("pH value: ");
  // Serial.println(pHValue, 2);

  // Serial.print("Turbidity value: ");
  // Serial.println(ntu, 2);

  Serial.print("Temperature value: ");
  Serial.println(temperature, 2);

  delay(5000);
}
