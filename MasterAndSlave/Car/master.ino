// Master
#define joy1X A8
#define joy1Y A9
#define joy2X A7
#define joy2Y A6

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

int x1, x2, y1, y2 = 0;
const int max=1040;
const int min=0;
double roll, pitch, yaw = 0.0;

int x1_mid = 520;
int x2_mid = 533;
int y1_mid = 497;
int y2_mid = 508;
int deadzone = 50;
int speed_x1, speed_x2, speed_y1 = 0;

double output;
const double setpoint = 0;
double deltatime;
unsigned long currentTime;
double Kp = 0.5, Ki = 0.1, Kd = 0.01;
PIDController pid(Kp, Ki, Kd);

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

void Setdeadzone(int& a, int b, byte c){
  if (abs(a - b) <= deadzone){
    a = b;
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
  currentTime = millis();
  deltatime = (currentTime - pid.getPreviousTime()) / 1000.0;

  x1 = analogRead(joy1X);
  y1 = analogRead(joy1Y);
  x2 = analogRead(joy2X);
  y2 = analogRead(joy2Y);

  // Setdeadzone(x1, x1_mid, joy1X);
  // Setdeadzone(x2, x2_mid, joy2X);
  // Setdeadzone(y1, y1_mid, joy1Y);

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

  int speed_RF = speed_y1 - speed_x1 - speed_x2; // - output; 
  int speed_RR = speed_y1 + speed_x1 - speed_x2; // - output; 
  int speed_LF = speed_y1 + speed_x1 + speed_x2; // + output; 
  int speed_LR = speed_y1 - speed_x1 + speed_x2; // + output; 

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
  Serial.println(abs(speed_RF + speed_RR + speed_LF + speed_LR) % 100);
  Serial1.write(abs(speed_RF + speed_RR + speed_LF + speed_LR) % 100);

  // Serial.print(speed_RF);
  // Serial.print(" ");
  // Serial.print(speed_RR);
  // Serial.print(" ");
  // Serial.print(speed_LF);
  // Serial.print(" ");
  // Serial.println(speed_LR);

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

  output = pid.PIDCompute(setpoint, yaw, deltatime, currentTime);
}
