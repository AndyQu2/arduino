#define JOYSTICK1_X A2
#define JOYSTICK1_Y A3
#define JOYSTICK2_X A4
#define JOYSTICK2_Y A5

const int switchPin[] = {37, 36, 35, 34, 33, 32, 31, 30};
const int buttonPin[] = {22, 23, 24, 25, 26, 27, 28, 29};
int switchValue[8] = {0};
int buttonValue[8] = {0};

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
  for (int i = 0; i < 8; i ++) {
    switchValue[i] = analogRead(switchPin[i]);
  }
  for (int i = 0; i < 8; i ++) {
    buttonValue[i] = analogRead(buttonPin[i]);
  }

  Serial3.write(0xAA);  // begin transmission
  size_t switch_size = sizeof(switchValue) / sizeof(switchValue[0]);
  size_t button_size = sizeof(buttonValue) / sizeof(buttonValue[0]);
  size_t body_size = switch_size + button_size;
  int body_checksum = body_size % 2;
  int switch_checksum = switch_size % 2;
  int button_checksum = button_size % 2;
  
  Serial3.write(body_size);
  Serial3.write(body_checksum);
  Serial3.write(switch_size);
  Serial3.write(switch_checksum);
  Serial3.write(button_size);
  Serial3.write(button_checksum);
  Serial.println(body_size);
  Serial.println(body_checksum);

  for (int i = 0; i < switch_size; i ++) {
    
  }
}
