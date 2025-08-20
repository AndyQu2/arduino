#include <Wire.h>

int data[10];
int X, Y;

void setup()
{
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop()
{
  X = data[0] + data[1] + data[2] + data[3] + data[4];
  Y = data[5] + data[6] + data[7] + data[8] + data[9];

  Serial.print("X: ");
  Serial.print(X);
  Serial.print("Y: ");
  Serial.println(Y);
}

void receiveEvent() 
{
  for (int i = 0; i < 10; i ++)
  {
    data[i] = Wire.read();
  }
}
