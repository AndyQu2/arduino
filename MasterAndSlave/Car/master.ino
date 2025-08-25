// Master
#define joy1X A8
#define joy1Y A9
#define joy2X A7
#define joy2Y A6

int x1, x2, y1, y2 = 0;
const int max=1040;
const int min=0;
double yaw, lastYaw = 0.0;

int x1_mid = 520;
int x2_mid = 533;
int y1_mid = 497;
int y2_mid = 508;
int deadzone = 50;
int speed_x1, speed_x2, speed_y1 = 0;

void sendSpeedDataToSlave(int data)
{
  if (data > 0)
  {
    Serial1.write(1);
    int body = floor(data);

    Serial1.write(body);
  }
  if (data < 0)
  {
    Serial1.write(0);
    int body = ceil(data);
        
    Serial1.write((body * -1));
  }
  if (data == 0)
  {
    Serial1.write(0);
    Serial1.write(0);
  }
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}
 
void loop() {
  x1 = analogRead(joy1X);
  y1 = analogRead(joy1Y);
  x2 = analogRead(joy2X);
  y2 = analogRead(joy2Y);

  Serial.print("Joystick1 X: ");
  Serial.print(x1);
  Serial.print(" Y: ");
  Serial.println(y1);
  Serial.print("Joystick2 X: ");
  Serial.print(x2);
  Serial.print(" Y: ");
  Serial.println(y2);

  if (x1 <= x1_mid + 80 && x1 >= x1_mid - 80 && y1 <= y1_mid + 80 && y1 >= y1_mid - 80 && x2 <= x2_mid + 80 && x2 >= x2_mid - 80)
  {
    speed_x1 = 0;
    speed_x2 = 0;
    speed_y1 = 0;

    x1 = analogRead(joy1X);
    y1 = analogRead(joy1Y);
    x2 = analogRead(joy2X);
    y2 = analogRead(joy2Y);
  }

  speed_x1 = map(x1, min, 996, -200, 200);
  speed_x2 = map(x2, min, 1066, -200, 200);
  speed_y1 = map(y1, min, 1040, -200, 200);

  int speed_RF = speed_y1 - speed_x1 - speed_x2; 
  int speed_RR = speed_y1 + speed_x1 - speed_x2; 
  int speed_LF = speed_y1 + speed_x1 + speed_x2; 
  int speed_LR = speed_y1 - speed_x1 + speed_x2; 

  Serial.print(speed_RF);
  Serial.print(" ");
  Serial.print(speed_RR);
  Serial.print(" ");
  Serial.print(speed_LF);
  Serial.print(" ");
  Serial.println(speed_LR);

  speed_RF = constrain(speed_RF, -200, 200);
  speed_RR = constrain(speed_RR, -200, 200);
  speed_LF = constrain(speed_LF, -200, 200);
  speed_LR = constrain(speed_LR, -200, 200);

  // Send speed data to slave
  sendSpeedDataToSlave(speed_RF);
  sendSpeedDataToSlave(speed_RR);
  sendSpeedDataToSlave(speed_LF);
  sendSpeedDataToSlave(speed_LR);
  // Sending verify code
  Serial.print("Verify code: ");
  int verify = floor(abs(speed_LR + speed_RR) >> 2);
  Serial.println(verify);
  Serial1.write(verify);
}
