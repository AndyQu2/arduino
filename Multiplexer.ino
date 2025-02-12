class Multiplexer {
	protected:
		int select_pin[4];
		int sig_pin;
		int select[4];
	public:
		Multiplexer(int*, int, int);
		void selectChannel(int);
		void writeSignal(int);
		int readSignal();
    void readSelect();
};

Multiplexer::Multiplexer(int* sp, int spsz, int sigp) {
	for (int i = 0; i < spsz; i ++) {
		select_pin[i] = *sp;
    sp ++;
	}
	
	sig_pin = sigp;
}

void Multiplexer::selectChannel(int channel) {
	int binaryNum[4];
	int bi, si = 0;
	
	while (channel > 0) {
		binaryNum[bi] = channel % 2;
		channel /= 2;
		bi ++;
	}
	
	for (int j = bi - 1; j >= 0; j --) {
		select[si] = binaryNum[j];
    si ++;
	}
	
	for (int i = 0; i < 4; i ++) {
		digitalWrite(select_pin[i], select[i]);
	}
}

void Multiplexer::writeSignal(int signal) {
  analogWrite(sig_pin, signal);
}

int Multiplexer::readSignal() {
	return analogRead(sig_pin);
}

void Multiplexer::readSelect() {
  for (int i = 0; i < 4; i ++) {
    Serial.print(select_pin[i]);
    Serial.print(" ");
  }
  Serial.println();
}

int s_pin[] = {8, 9, 10, 11};
int* pt;
int sz = sizeof(s_pin) / sizeof(s_pin[0]);
int signal_pin = A0;
Multiplexer mp(s_pin, sz, signal_pin);

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i ++) {
    pinMode(s_pin[i], OUTPUT);
  }

  pinMode(signal_pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  mp.selectChannel(15);
  mp.writeSignal(HIGH);
  mp.readSelect();
}