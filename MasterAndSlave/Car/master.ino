// Master

#define JOYSTICK_X A9
#define JOYSTICK_Y A8
#define JOYSTICK1_X A7
#define JOYSTICK1_Y A6

int joystickX, joystickY = 0;
int joystick1X, joystick1Y = 0;
const int max=977;
const int min=0;
double roll, pitch, yaw = 0.0;

int x1_mid = 521;
int x2_mid = 533;
int y1_mid = 496;
int deadzone = 30;

void Setdeadzone(int a, int b, byte c){
  if (abs(a - b) < deadzone){
    a = b;
  }else{
    a = analogRead(c);
  }
}

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
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}
 
void loop() {
  joystickX = analogRead(JOYSTICK_X);
  joystickY = analogRead(JOYSTICK_Y);
  joystick1X = analogRead(JOYSTICK1_X);
  joystick1Y = analogRead(JOYSTICK1_Y);

  Setdeadzone(joystickX, x1_mid, JOYSTICK_X);
  Setdeadzone(joystick1X, x2_mid, JOYSTICK1_X);
  Setdeadzone(joystickY, y1_mid, JOYSTICK_Y);

  Serial.print("Joystick1 X: ");
  Serial.print(joystickX);
  Serial.print(" Y: ");
  Serial.println(joystickY);
  Serial.print("Joystick2 X: ");
  Serial.print(joystick1X);
  Serial.print(" Y: ");
  Serial.println(joystick1Y);

  int speed_y = map(joystickY, min, max, -200, 200);
  int speed_x = map(joystickX, min, max, -200, 200);
  int speed_x1= map(joystick1X, min, max, -200, 200);

  int speed_rf = speed_y - speed_x - speed_x1;
  int speed_rr = speed_y + speed_x - speed_x1;
  int speed_lf = speed_y + speed_x + speed_x1;
  int speed_lr = speed_y - speed_x + speed_x1;

  // Send speed data to slave
  sendSpeedDataToSlave(constrain(speed_rf, -200, 200));
  sendSpeedDataToSlave(constrain(speed_rf, -200, 200));
  sendSpeedDataToSlave(constrain(speed_rf, -200, 200));
  sendSpeedDataToSlave(constrain(speed_rf, -200, 200));

  Serial.print(speed_rf);
  Serial.print(" ");
  Serial.print(speed_rr);
  Serial.print(" ");
  Serial.print(speed_lf);
  Serial.print(" ");
  Serial.println(speed_lr);

  // Receive JY901 data from slave
  double received[9];
  if (Serial1.available()) {
    for (int i = 0; i < 9; i ++) {
      received[i] = Serial1.read();
    }
  }
  
  roll = received[1] + (received[2] / 100);
  if (!received[0]) roll = roll * -1;
  pitch = received[4] + (received[5] / 100);
  if (!received[3]) pitch = pitch * -1;
  yaw = received[7] + (received[8] / 100);
  if (!received[6]) yaw = yaw * -1;
  
  Serial.print("JY901 data: ");
  Serial.print(roll);
  Serial.print(" ");
  Serial.print(pitch);
  Serial.print(" ");
  Serial.print(yaw);
  Serial.println();
}
