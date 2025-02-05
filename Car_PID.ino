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

class PIDController {
	protected:
		double kp;
		double ki;
		double kd;
		double previous_error;
		double integral;
    unsigned long previous_time;
    const double maxIntegral = 1000;
	public:
		PIDController(double, double, double);
		double PIDCompute(double, double, double, unsigned long);
    unsigned long getPreviousTime();
};

class Motor {
	protected:
		int BI1Pin;
		int BI2Pin;
		int PWMPin;
	public:
		void InitMotor(int, int, int);
		void Control(int, int, int);
};

void Motor::InitMotor(int PWM_Pin, int BI1_Pin, int BI2_Pin) {
	BI1Pin = BI1_Pin;
	BI2Pin = BI2_Pin;
	PWMPin = PWM_Pin;
	pinMode(PWM_Pin, OUTPUT);
	pinMode(BI1_Pin, OUTPUT);
	pinMode(BI2_Pin, OUTPUT);
}

void Motor::Control(int BI1, int BI2, int speed) {
	if (speed >= 0) {
		digitalWrite(BI1Pin, BI1);
		digitalWrite(BI2Pin, BI2);
		digitalWrite(PWMPin, speed);
	}
	else if (speed < 0) {
		digitalWrite(BI1Pin, BI2);
		digitalWrite(BI2Pin, BI1);
		digitalWrite(PWMPin, abs(speed));
	}
}

PIDController::PIDController(double Kp, double Ki, double Kd) {
	kp = Kp;
	ki = Ki;
	kd = Kd;
	previous_error = 0.0;
	integral = 0.0;
}

double PIDController::PIDCompute(double setpoint, double measured_value, double dt, unsigned long currentTime) {
	double error = setpoint - measured_value;
	integral += error * dt;
  integral = constrain(integral, -maxIntegral, maxIntegral);
	double derivative = (error - previous_error) / dt;
	double output = (kp * error) + (ki * integral) + (kd * derivative);
	previous_error = error;
  previous_time = currentTime;
	return output;
}

unsigned long PIDController::getPreviousTime() {
  return previous_time;
}

int speed_rf, speed_rr, speed_lf, speed_lr;
double output;
const double setpoint = 0;
double deltatime;
unsigned long currentTime;
double Kp = 1.0, Ki = 0.1, Kd = 0.05;
PIDController pid(Kp, Ki, Kd);

Motor RFMotor;
Motor RRMotor;
Motor LFMotor;
Motor LRMotor;

void setup() {
  Serial.begin(9600);
  
  Serial3.begin(9600);
  JY901.attach(Serial3);

  RFMotor.InitMotor(RF_PWM, RF_BI1, RF_BI2);
  RRMotor.InitMotor(RR_PWM, RR_BI1, RR_BI2);
  LFMotor.InitMotor(LF_PWM, LF_BI1, LF_BI2);
  LRMotor.InitMotor(LR_PWM, LR_BI1, LR_BI2);
}

void loop() {
  currentTime = millis();
  deltatime = (currentTime - pid.getPreviousTime()) / 1000.0;

  JY901.receiveSerialData();
  double angle = JY901.getYaw();

  x1=analogRead(joy1X);
  y1=analogRead(joy1Y);
  x2=analogRead(joy2X);
  y2=analogRead(joy2Y);

  output = pid.PIDCompute(setpoint, angle, deltatime, currentTime);

  speed_rf = -output;
  speed_rr = -output;
  speed_lf = output;
  speed_lr = output;

  RFMotor.Control(1, 0, constrain(speed_rf, -200, 200));
  RRMotor.Control(1, 0, constrain(speed_rr, -200, 200));
  LFMotor.Control(0, 1, constrain(speed_lf, -200, 200));
  LRMotor.Control(0, 1, constrain(speed_lr, -200, 200));
}