#include <SPI.h>
#include <mcp2515.h>

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

int16_t current = 1000;
const int target_speed = 3000;
const double Kp = 3.01, Ki = 0.81, Kd = 0.1;
double deltatime;
unsigned long currentTime;
word speed;
struct can_frame canRead;
struct can_frame canCurrent;
MCP2515 mcp2515(53);

PIDController pid(Kp, Ki, Kd);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  canCurrent.can_id = 0x200;
  canCurrent.can_dlc = 8;
  for (int i = 0; i < 8; i ++) canCurrent.data[i] = 0x00;

  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  currentTime = millis();
  deltatime = (currentTime - pid.getPreviousTime()) / 1000.0;

  if (mcp2515.readMessage(&canRead) == MCP2515::ERROR_OK) {
    speed = word(canRead.data[2], canRead.data[3]);

    Serial.print("Receive-Angle: ");
    Serial.print(word(canRead.data[0], canRead.data[1]));
    Serial.print(" Receive-Speed: ");
    Serial.print(speed);
    Serial.print(" Receive-Current: ");
    Serial.print(word(canRead.data[4], canRead.data[5]));
    Serial.print(" Temperature: ");
    Serial.println(canRead.data[6]);
  }

  canCurrent.data[0] = (current >> 8) & 0xFF;
  canCurrent.data[1] = current & 0xFF;
  mcp2515.sendMessage(&canCurrent);

  current = (int16_t)(pid.PIDCompute(target_speed, speed, deltatime, currentTime));
  Serial.print("Current-Write: ");
  Serial.println(current);
}
