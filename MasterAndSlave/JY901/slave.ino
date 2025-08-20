#include <Wire.h>

double data[9];
double roll, pitch, yaw = 0.0;

void setup() {
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  roll = data[1] + (data[2] / 100);
  if (!data[0]) roll = roll * -1;
  pitch = data[4] + (data[5] / 100);
  if (!data[3]) pitch = pitch * -1;
  yaw = data[7] + (data[8] / 100);
  if (!data[6]) yaw = yaw * -1;

  Serial.print("Angle:");
	Serial.print(roll);
	Serial.print(" ");
	Serial.print(pitch);
	Serial.print(" ");
	Serial.print(yaw);
	Serial.print("\n");
}

void receiveEvent() 
{
  for (int i = 0; i < 9; i ++)
  {
    data[i] = Wire.read();
  }
}
