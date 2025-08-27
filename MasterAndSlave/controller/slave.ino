#define JOYSTICK1_X A2
#define JOYSTICK1_Y A3
#define JOYSTICK2_X A4
#define JOYSTICK2_Y A5
#define TRIGGER1 A6
#define TRIGGER2 A7
#define TRIGGER3 A8
#define TRIGGER4 A9

const int buttonPin[] = {37, 36, 35, 34, 33, 32, 31, 30};
const int switchPin[] = {22, 23, 24, 25, 26, 27, 28, 29};
int switchValue[8] = {0};
int buttonValue[8] = {0};
int trigger1, trigger2, trigger3, trigger4 = 0;

const int max=910;
const int min=0;

void sendSpeedDataToSlave(int data)
{
  if (data > 0)
  {
    Serial3.write(1);
    int body = floor(data);

    Serial3.write(body);
  }
  if (data < 0)
  {
    Serial3.write(0);
    int body = ceil(data);
        
    Serial3.write((body * -1));
  }
  if (data == 0)
  {
    Serial3.write(0);
    Serial3.write(0);
  }
}

void sendBigDataToSlave(int data)
{
  int count = 5;
  while(data > 255)
  {
    Serial3.write(255);
    data = data - 255;
    count --;
  }

  Serial3.write(data);
  count --;

  if (count != 0)
  {
    for (int i = 0; i < count; i ++)
    {
      Serial3.write(0);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial3.begin(9600);

  for (int i = 0; i < 8; i ++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
  for (int i = 0; i < 8; i ++) {
    pinMode(switchPin[i], INPUT_PULLUP);
  }
}

void loop() {
  int x1 = analogRead(JOYSTICK1_X);
  int y1 = analogRead(JOYSTICK1_Y);
  int x2 = analogRead(JOYSTICK2_X);
  int y2 = analogRead(JOYSTICK2_Y);

  int speed_x1 = map(x1, min, 996, -200, 200);
  int speed_x2 = map(x2, min, 1066, -200, 200);
  int speed_y1 = map(y1, min, 1040, -200, 200);

  int speed_RF = speed_y1 - speed_x1 - speed_x2; 
  int speed_RR = speed_y1 + speed_x1 - speed_x2; 
  int speed_LF = speed_y1 + speed_x1 + speed_x2; 
  int speed_LR = speed_y1 - speed_x1 + speed_x2; 

  Serial.print(" Speed: ");
  Serial.print(speed_RF);
  Serial.print(" ");
  Serial.print(speed_RR);
  Serial.print(" ");
  Serial.print(speed_LF);
  Serial.print(" ");
  Serial.print(speed_LR);
  Serial.print(" ");

  speed_RF = constrain(speed_RF, -200, 200);
  speed_RR = constrain(speed_RR, -200, 200);
  speed_LF = constrain(speed_LF, -200, 200);
  speed_LR = constrain(speed_LR, -200, 200);

  Serial.print("Switch: ");
  for (int i = 0; i < 8; i ++) {
    switchValue[i] = digitalRead(switchPin[i]);
    Serial.print(switchValue[i]);
    Serial.print(" ");
  }
  Serial.print(" Button: ");
  for (int i = 0; i < 8; i ++) {
    buttonValue[i] = digitalRead(buttonPin[i]);
    Serial.print(buttonValue[i]);
    Serial.print(" ");
  }
  Serial.println();

  trigger1 = analogRead(TRIGGER1);
  trigger2 = analogRead(TRIGGER2);
  trigger3 = analogRead(TRIGGER3);
  trigger4 = analogRead(TRIGGER4);

  Serial.print(" Trigger: ");
  Serial.print(trigger1);
  Serial.print(" ");
  Serial.print(trigger2);
  Serial.print(" ");
  Serial.print(trigger3);
  Serial.print(" ");
  Serial.print(trigger4);
  Serial.print(" ");

  Serial3.write(0xAA);

  for (int i = 0; i < 8; i ++) {
    Serial3.write(switchValue[i]);
  }
  for (int i = 0; i < 8; i ++) {
    Serial3.write(buttonValue[i]);
  }

  sendSpeedDataToSlave(speed_RF);
  sendSpeedDataToSlave(speed_RR);
  sendSpeedDataToSlave(speed_LF);
  sendSpeedDataToSlave(speed_LR);
  int verify = abs(speed_LR + speed_RR + speed_LR + speed_RF) >> 2;
  Serial3.write(verify);

  sendBigDataToSlave(trigger1);

  Serial3.write(0xFF);
  
  delay(100);
}
