//set Nova SM3 version
#define VERSION 5.2

//load default active values, to be refreshed from eprom on boot
byte debug1 = 0;
byte uss_active = 1;              //activate Ultra-Sonic sensors
byte uss_debug = 0;               //debug uss sensors, not linked to debug 1

#include <Wire.h>
#include <EEPROM.h>
#include <NewPing.h>

//serial connection
#define RESET_PIN 3
#define SLAVE_ID 1
byte serial_oled = 0;
int resp;
char req;

//ultrasonic sensors
#define SONAR_NUM 2
#define L_TRIGPIN 7
#define L_ECHOPIN 6
#define R_TRIGPIN 5
#define R_ECHOPIN 4
#define MAX_DISTANCE 300
NewPing sonar[SONAR_NUM] = {
  NewPing(L_TRIGPIN, L_ECHOPIN, MAX_DISTANCE),
  NewPing(R_TRIGPIN, R_ECHOPIN, MAX_DISTANCE), 
};

void save_ep_data();
void load_ep_data();
void reset_slave();

void setup() {
  Serial.begin(19200);
  delay(200);

  //=======================================
  //IMPORTANT: FLASH EEPROM BEFORE USING!!
  //=======================================
  // set debug1 off and set all switches to active, then uncomment the two lines below and upload the code to flash EEPROM.
  // after the Nano finishes booting, comment-out these two lines, and re-upload the code again.
  // now the main Nova code running on the Teensy will set these switches according to their setting in Teensy Code ;)
  //=======================================
  
  //save_ep_data();
  //delay(1000);


  load_ep_data();
  delay(500);

  if (debug1) {
    Serial.println("eeprom loaded:");
    Serial.print("debug1: ");Serial.println(debug1);
    Serial.print("uss_active: ");Serial.println(uss_active);
    Serial.print("uss_debug: ");Serial.println(uss_debug);
    Serial.println();
  }

  digitalWrite(RESET_PIN, HIGH);
  delay(20);
  pinMode(RESET_PIN, OUTPUT);   
  
  Wire.begin(SLAVE_ID); 
  Wire.onRequest(requestCallback);
  Wire.onReceive(receiveEvent);
  delay(200);
  if (debug1) Serial.println(F("Ready!"));
}

void loop(){
  if(uss_debug){
    delay(50);
    Serial.print("\nLeft Sonar:\t");
    Serial.print(get_distance(0));
    Serial.print("\nRight Sonar:\t");
    Serial.print(get_distance(1));
  }
}

int get_distance(int side) {
  int dist = sonar[side].ping_cm();
  if(dist == 0)
      dist = MAX_DISTANCE;
      
  if (debug1 && !uss_debug) {
    if(side)
      Serial.print(F("Right ultrasonic sensor:\t"));
    else 
      Serial.print(F("Left ultrasonic sensor:\t"));
    Serial.println(dist);
  }
  return dist;
}

void receiveEvent(int aCount) {
  byte x;
  while (0 < Wire.available()) {
    x = Wire.read();
    req = x;
  }
}

void requestCallback() {  
  byte send_resp = 0;
  resp = 0;
  if (debug1) {
    //Serial.print(F("System Command! req: "));
    //Serial.println(req);
  }
  switch (req) {  
    //uss distances
    case 'a':
      send_resp = 1;
      resp = get_distance(0);
      break;
    case 'b':
      send_resp = 1;
      resp = get_distance(1);
      break;

    case 'q': //reserved
      break;
    case 'r': //reserved
      break;
      
    case 'Z':
      delay(300);
      reset_slave();
      break;  
  }
  
  if (send_resp) {
    if (debug1) Serial.print(F("\nSending response... "));
    uint8_t buffer[2];
    buffer[0] = resp >> 8;
    buffer[1] = resp & 0xff;  
    Wire.write(buffer, 2);
    //Wire.write(resp);
    if (debug1){
      Serial.print(resp);
      Serial.print(F(" sent!"));
    }
  }
}


void load_ep_data() {
  //retrieve stored vars from eprom
  debug1 = EEPROM.read(0);
  uss_active = EEPROM.read(1);
}


void save_ep_data() {
  //save vars to eprom
  EEPROM.write(0,debug1);
  EEPROM.write(1,uss_active);
}


void reset_slave() {
  digitalWrite(RESET_PIN, LOW);
}
