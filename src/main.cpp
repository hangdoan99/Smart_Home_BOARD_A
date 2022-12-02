#include <Arduino.h>

const int pulseA = 12;
const int pulseB = 13;
const int pushSW = 2;
volatile int lastEncoded = 0;
volatile long encoderValue = 0;

int lux = A0;

IRAM_ATTR void handleRotary()
{
  // Never put any long instruction
  int MSB = digitalRead(pulseA); // MSB = most significant bit
  int LSB = digitalRead(pulseB); // LSB = least significant bit

  int encoded = (MSB << 1) | LSB;         // converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    encoderValue++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    encoderValue--;
  lastEncoded = encoded; // store this value for next time
  if (encoderValue > 1023)
  {
    encoderValue = 1023;
  }
  else if (encoderValue < 0)
  {
    encoderValue = 0;
  }
}

// IRAM_ATTR void buttonClicked()
// {
//   Serial.println("pushed");
// }

void setup()
{
  Serial.begin(115200);
  pinMode(pushSW, INPUT_PULLUP);
  pinMode(pulseA, INPUT_PULLUP);
  pinMode(pulseB, INPUT_PULLUP);
//   pinMode(RELAY, OUTPUT);
//   attachInterrupt(pushSW, buttonClicked, FALLING);
  attachInterrupt(pulseA, handleRotary, CHANGE);
  attachInterrupt(pulseB, handleRotary, CHANGE);

}

void loop()
{
  lux = analogRead(A0);
  delay(500);
  Serial.print("encoder value: "); Serial.println(encoderValue);
  Serial.print("lux value: "); Serial.println(lux);

  if (encoderValue > lux)
  {
    // digitalWrite(RELAY, HIGH);
    Serial.println("NeoPixel ON");
  }
  else
  {
    // digitalWrite(RELAY, LOW);
    Serial.println("NeoPixel OFF");
  }
}