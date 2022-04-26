/*
 * Note: This Code was not tested on the roboter, but should work
 */

#define PIR_FRONT 4
#define PIR_LEFT 5
#define PIR_RIGHT 6
unsigned int pirInterval = 500;
unsigned long lastPIRUpdate = 0;
byte pir_front;
byte pir_left;
byte pir_right;

void setup() {
  pinMode(PIR_FRONT, INPUT);
  digitalWrite(PIR_FRONT, HIGH); 
 
  pinMode(PIR_LEFT, INPUT);
  digitalWrite(PIR_LEFT, HIGH); 

  pinMode(PIR_RIGHT, INPUT);
  digitalWrite(PIR_RIGHT, HIGH); 
}

void loop() {
  if (millis() - lastPIRUpdate > pirInterval){
    pir_front = digitalRead(PIR_FRONT);
    pir_left = digitalRead(PIR_LEFT);
    pir_right = digitalRead(PIR_RIGHT);
  
    if(pir_front == HIGH)
      Serial.print(F("\nPIR Front High"));
    if(pir_left == HIGH)
      Serial.print(F("\nPIR Left High"));
    if(pir_right == HIGH)
      Serial.print(F("\nPIR Right High"));
  
    lastPIRUpdate = millis();
  }
}
