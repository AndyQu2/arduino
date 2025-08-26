#define BUTTON_FLAG 0xAA
#define SWITCH_FLAG 0xA1
#define STOP_FLAG 0xFF

int switchs[8] = {0};
int buttons[8] = {0};
int current = 0xFF;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  if (Serial2.read() == 0xAA) {
    Serial.println("Communication begin");

    int size = Serial2.read();
    int checksum = Serial2.read();
    
    int switch_size = Serial2.read();
    int switch_checksum = Serial2.read();
    int button_size = Serial2.read();
    int button_checksum = Serial2.read();

    if (size % 2 == checksum 
      && switch_size % 2 == switch_checksum 
      && button_size % 2 == button_checksum) {
      for (int i = 0; i < switch_size; i ++) {
        switchs[i] = Serial2.read();
      }
      
    }
  }
}
