#include <Wire.h>

#define JOYSTICK_X A9
#define JOYSTICK_Y A8

void setup()
{
  Serial.begin(9600);
  Wire.begin();
}

void sendMessageToSlave(int data)
{
  int count = 5;
  while(data > 255)
  {
    Wire.write(255);
    data = data - 255;
    count --;
  }

  Wire.write(data);
  count --;

  if (count != 0)
  {
    for (int i = 0; i < count; i ++)
    {
      Wire.write(0);
    }
  }
}

void loop()
{
  int x = analogRead(JOYSTICK_X);
  int y = analogRead(JOYSTICK_Y);

  Serial.print("X: ");
  Serial.print(x);
  Serial.print("Y: ");
  Serial.println(y);

  Wire.beginTransmission(4);
  sendMessageToSlave(x);
  sendMessageToSlave(y);
  Wire.endTransmission();
}
