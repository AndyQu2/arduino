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

int received[47];
int data[16];
int current = 0xFF;
bool hasInvalid = false;
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  for (int i = 0; i < 47; i ++) {
    received[i] = Serial2.read();
    if (i == 0) {
      while (received[i] != 0xAA) {
        received[i] = Serial2.read();
      }
    }
    if (i == 32) {
      while (received[i] != 0xFF) {
        received[i] = Serial2.read();
      }
    }
    while (received[i] < 0) {
      received[i] = Serial2.read();
    }
  }

  Serial.print("Switch: ");
  for (int i = 1; i <= 8; i ++) {
    if (received[i] != 0xAA && received[i] != 0xFF) {
      data[i] = received[i];
      Serial.print(data[i]);
      Serial.print(" ");
    }
  }

  Serial.print(" Button: ");
  for (int i = 9; i <= 16; i ++) {
    if (received[i] != 0xAA && received[i] != 0xFF) {
      data[i] = received[i];
      Serial.print(data[i]);
      Serial.print(" ");
    }
  }

  int speed_rf = received[18];
  if (!received[17]) speed_rf *= -1;
  int speed_rr = received[20];
  if (!received[19]) speed_rr *= -1;
  int speed_lf = received[22];
  if (!received[21]) speed_lf *= -1;
  int speed_lr = received[24];
  if (!received[21]) speed_lr *= -1;
  int verifySpeed = received[25];

  Serial.print(" Speeed: ");
  Serial.print(speed_rf);
  Serial.print(" ");
  Serial.print(speed_rr);
  Serial.print(" ");
  Serial.print(speed_lf);
  Serial.print(" ");
  Serial.print(speed_lr);
  Serial.print(" VerifyCode: ");
  Serial.print(verifySpeed);

  Serial.println();
}
