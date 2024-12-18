#define LED1_PIN 8
#define LED2_PIN 9
#define LED3_PIN 10
#define LED4_PIN 11
#define BUTTON_PIN 2

int x = 0, y = 0, z = 0;
bool led = false;
bool flag = false;
unsigned long long time, t1 = 0, t2 = 0, t3 = 0;

void switchLED() {
  flag = !flag;

  if (flag) led = !led;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), switchLED, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  time = millis();

  if ((time - t1) == 1000) {
    x = 1 - x;
    t1 = millis();
    digitalWrite(LED1_PIN, x);
  }
  if ((time - t2) == 2000) {
    y = 1 - y;
    t2 = millis();
    digitalWrite(LED2_PIN, y);
  }
  if ((time - t3) == 3000) {
    z = 1 - z;
    t3 = millis();
    digitalWrite(LED3_PIN, z);
  }
  digitalWrite(LED4_PIN, led);
}