#include <Wire.h>
#include <JY901.h>

void setup() {
  Serial.begin(9600);
  JY901.startIIC();
  Wire.begin();
}

void sendMessageToSlave(double data)
{
  if (data > 0)
  {
    Wire.write(1);
    int body = floor(data);
    int remain = (data - body) * 100;
    Wire.write(body);
    Wire.write(remain);
  }
  if (data < 0)
  {
    Wire.write(0);
    int body = ceil(data);
    int remain = (data - body) * -100;
    
    Wire.write((body * -1));
    Wire.write(remain);
  }
}

void loop() {
  Serial.print("Angle:");
	Serial.print(JY901.getRoll());
	Serial.print(" ");
	Serial.print(JY901.getPitch());
	Serial.print(" ");
	Serial.print(JY901.getYaw());
	Serial.print("\n");

  double roll = JY901.getRoll();
  double pitch = JY901.getPitch();
  double yaw = JY901.getYaw();

  Wire.beginTransmission(4);
  sendMessageToSlave(roll);
  sendMessageToSlave(pitch);
  sendMessageToSlave(yaw);
  Wire.endTransmission();
}
