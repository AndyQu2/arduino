// Slave
#include <JY901.h>

#define RF_BI1 34
#define RF_BI2 35
#define RF_PWM 12

#define RR_BI1 37
#define RR_BI2 36
#define RR_PWM 8

#define LF_BI1 43
#define LF_BI2 42
#define LF_PWM 9

#define LR_BI1 29
#define LR_BI2 39
#define LR_PWM 5

int received[8] = {0};
int receivedVerifyCode = 0;
int last_rf, last_rr, last_lf, last_lr = 0;

void controlRF(int BI1,int BI2,int speed){
  if (speed >= 0) {
    digitalWrite(RF_BI1, BI1);
    digitalWrite(RF_BI2, BI2);
    analogWrite(RF_PWM, speed);
  }
  else if (speed < 0) {
    digitalWrite(RF_BI1, BI2);
    digitalWrite(RF_BI2, BI1);
    analogWrite(RF_PWM, abs(speed));
  }
}

void controlLF(int BI2,int BI1,int speed){
  if (speed >= 0) {
    digitalWrite(LF_BI1, BI1);
    digitalWrite(LF_BI2, BI2);
    analogWrite(LF_PWM, speed);
  }
  else if (speed < 0) {
    digitalWrite(LF_BI1, BI2);
    digitalWrite(LF_BI2, BI1);
    analogWrite(LF_PWM, abs(speed));
  }
}

void controlRR(int BI1,int BI2,int speed){
  if (speed >= 0) {
    digitalWrite(RR_BI1, BI1);
    digitalWrite(RR_BI2, BI2);
    analogWrite(RR_PWM, speed);
  }
  else if (speed < 0) {
    digitalWrite(RR_BI1, BI2);
    digitalWrite(RR_BI2, BI1);
    analogWrite(RR_PWM, abs(speed));
  }
}

void controlLR(int BI2,int BI1,int speed){
  if (speed >= 0) {
    digitalWrite(LR_BI1, BI1);
    digitalWrite(LR_BI2, BI2);
    analogWrite(LR_PWM, speed);
  }
  else if (speed < 0) {
    digitalWrite(LR_BI1, BI2);
    digitalWrite(LR_BI2, BI1);
    analogWrite(LR_PWM, abs(speed));
  }
}

void sendDoubleDataToMaster(double data)
{
  if (data > 0)
  {
    Serial2.write(1);
    int body = floor(data);
    int remain = (data - body) * 100;
    Serial2.write(body);
    Serial2.write(remain);
  }
  if (data < 0)
  {
    Serial2.write(0);
    int body = ceil(data);
    int remain = (data - body) * -100;
    
    Serial2.write((body * -1));
    Serial2.write(remain);
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

  pinMode(RF_BI1, OUTPUT);
  pinMode(RF_BI2, OUTPUT);
  pinMode(RF_PWM, OUTPUT);

  pinMode(RR_BI1, OUTPUT);
  pinMode(RR_BI2, OUTPUT);
  pinMode(RR_PWM, OUTPUT);

  pinMode(LF_BI1, OUTPUT);
  pinMode(LF_BI2, OUTPUT);
  pinMode(LF_PWM, OUTPUT);
  
  pinMode(LR_BI1, OUTPUT);
  pinMode(LR_BI2, OUTPUT);
  pinMode(LR_PWM, OUTPUT);

  JY901.attach(Serial3);
}
 
void loop() {
  // Receive speed data from master
  JY901.receiveSerialData();

  if (Serial2.available()) {
    for (int i = 0; i < 8; i ++) {
      received[i] = (int)Serial2.read();
    }
  }
  receivedVerifyCode = (int)Serial2.read();

  int speed_rf = received[1];
  if (!received[0]) speed_rf = speed_rf * -1;
  int speed_rr = received[3];
  if (!received[2]) speed_rr = speed_rr * -1;
  int speed_lf = received[5];
  if (!received[4]) speed_lf = speed_lf * -1;
  int speed_lr = received[7];
  if (!received[6]) speed_lr = speed_lr * -1;

  Serial.print("Verify code: ");
  Serial.println(receivedVerifyCode);

  if (abs(speed_rf + speed_rr + speed_lf + speed_lr) % 100 == receivedVerifyCode) {
    if(abs(speed_rf) > 200) speed_rf = 0;
    if(abs(speed_rr) > 200) speed_rr = 0;
    if(abs(speed_lf) > 200) speed_lf = 0;
    if(abs(speed_lr) > 200) speed_lr = 0;

    Serial.print("Received Speed Data: ");
    Serial.print(speed_rf);
	  Serial.print(" ");
    Serial.print(speed_rr);
	  Serial.print(" ");
    Serial.print(speed_lf);
	  Serial.print(" ");
    Serial.print(speed_lr);
	  Serial.print(" ");
    Serial.println();

    last_rf = speed_rf;
    last_rr = speed_rr;
    last_lf = speed_lf;
    last_lr = speed_lr;
  }

  controlRF(1, 0, last_rf);
  controlRR(1, 0, last_rr);
  controlLF(1, 0, last_lf);
  controlLR(1, 0, last_lr);

  double roll = JY901.getRoll();
  double pitch = JY901.getPitch();
  double yaw = JY901.getYaw();

  Serial.print("Angle:");
	Serial.print(roll);
	Serial.print(" ");
	Serial.print(pitch);
	Serial.print(" ");
	Serial.print(yaw);
	Serial.print("\n");

  // Sending JY901 data back to master
  sendDoubleDataToMaster(roll);
  sendDoubleDataToMaster(pitch);
  sendDoubleDataToMaster(yaw);
}
