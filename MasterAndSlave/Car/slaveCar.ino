// Slave
#include <JY901.h>
#include <PID_v1.h>

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

const double setPoint = 140.0;
double input, output = 0.0;
double Kp=1, Ki=0.05 , Kd=0.25;
PID myPID(&input, &output, &setPoint, 2, 1, 1,P_ON_M, DIRECT);

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

  myPID.SetMode(AUTOMATIC);
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

  if (floor(abs(speed_lr + speed_rr) >> 2) == receivedVerifyCode) {
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

    if (abs(input) < abs(setPoint))
    {
      last_rf = speed_rf + output;
      last_rr = speed_rr + output;
      last_lf = speed_lf - output;
      last_lr = speed_lr - output;
    }
    if (abs(input) > abs(setPoint))
    {
      last_rf = speed_rf - output;
      last_rr = speed_rr - output;
      last_lf = speed_lf + output;
      last_lr = speed_lr + output;
    }
  }
  
  controlRF(1, 0, constrain(last_rf, -200, 200));
  controlRR(1, 0, constrain(last_rr, -200, 200));
  controlLF(1, 0, constrain(last_lf, -200, 200));
  controlLR(1, 0, constrain(last_lr, -200, 200));

  double roll = JY901.getRoll();
  double pitch = JY901.getPitch();
  input = JY901.getYaw();

  Serial.print("Angle:");
	Serial.print(roll);
	Serial.print(" ");
	Serial.print(pitch);
	Serial.print(" ");
	Serial.print(input);
	Serial.print("\n");

  myPID.Compute();
}
