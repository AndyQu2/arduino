#include <SPI.h>

void setup (void) {
   Serial.begin(115200);
   digitalWrite(SS, HIGH);
   SPI.begin ();
   SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop (void) {
  
  char c;
  digitalWrite(SS, LOW);
   
  for (const char* p = num ; c = *p; p++) 
  {
    SPI.transfer (c);
    Serial.print(c);
  }

  digitalWrite(SS, HIGH);
  delay(2000);
}
