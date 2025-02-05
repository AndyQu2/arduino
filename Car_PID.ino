#include <JY901.h>

#define joy1X A10
#define joy1Y A11
#define joy2X A15
#define joy2Y A14

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

typedef struct {
  double kp;
  double ki;
  double kd;
  double previous_error;
  double integral;
} PIDController;

int speed_rf, speed_rr, speed_lf, speed_lr;
double output;
PIDController pid;
const double setpoint = 0, dt = 0.8;

double Kp = 30, Ki = 25, Kd = 20;

int x1=0,y1=0,x2=0,y2=0;
int middle_x1=477, middle_y1=490, middle_x2=497, middle_y2=483;
int max=977;
int min=0;

void PID_Init(PIDController *pid, double kp, double ki, double kd) {
  pid->kp = kp;
  pid->ki = ki;
  pid->kd = kd;
  pid->previous_error = 0.0;
  pid->integral = 0.0;
}

double PID_Compute(PIDController *pid, double setpoint, double measured_value, double dt) {
  double error = setpoint - measured_value;
  pid->integral += error * dt;
  double derivative = (error - pid->previous_error) / dt;
  double output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;
  pid->previous_error = error;
  return output;
}

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
  
  Serial3.begin(9600);
  JY901.attach(Serial3);

  pinMode(RF_PWM,OUTPUT);
  pinMode(RR_PWM,OUTPUT);
  pinMode(LF_PWM,OUTPUT);
  pinMode(LR_PWM,OUTPUT);

  pinMode(RF_BI1,OUTPUT);
  pinMode(RF_BI2,OUTPUT);
  pinMode(RR_BI1,OUTPUT);
  pinMode(RR_BI2,OUTPUT);
  pinMode(LF_BI1,OUTPUT);
  pinMode(LF_BI2,OUTPUT);
  pinMode(LR_BI1,OUTPUT); 
  pinMode(LR_BI2,OUTPUT);
}

void loop() {
  JY901.receiveSerialData();
  double angle = JY901.getYaw();
  PID_Init(&pid, Kp, Ki, Kd);

  x1=analogRead(joy1X);
  y1=analogRead(joy1Y);
  x2=analogRead(joy2X);
  y2=analogRead(joy2Y);

  output = PID_Compute(&pid, setpoint, angle, dt);

  speed_rf = -output;
  speed_rr = -output;
  speed_lf = output;
  speed_lr = output;
  Serial.println(angle);

  controlRF(1, 0, constrain(speed_rf, -200, 200));
  controlRR(1, 0, constrain(speed_rr, -200, 200));
  controlLF(1, 0, constrain(speed_lf, -200, 200));
  controlLR(1, 0, constrain(speed_lr, -200, 200));
}