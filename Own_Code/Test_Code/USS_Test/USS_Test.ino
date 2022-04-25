#include <NewPing.h>

// trigger and echo pins for each sensor
#define L_TRIGPIN 7
#define L_ECHOPIN 6
#define R_TRIGPIN 5
#define R_ECHOPIN 4
#define MAX_DISTANCE 200 // maximum distance for sensors in cm
#define NUM_SONAR 2 // number of sonar sensors

NewPing sonar[NUM_SONAR] = { // array of sonar sensor objects
  NewPing(R_TRIGPIN, R_ECHOPIN, MAX_DISTANCE),
  NewPing(L_TRIGPIN, L_ECHOPIN, MAX_DISTANCE),
};

int distance[NUM_SONAR]; // array stores distances for each sensor in cm

void setup() {
  Serial.begin(115200); //Make sure to set the baud rate in the Serial Monitor to this number, otherwise it will debug nonesense characters
}

void loop() {
  delay(50);
  updateSonar();
  Serial.print("\nRight Sonar: ");
  Serial.print(distance[0]);
  Serial.print("\nLeft Sonar: ");
  Serial.print(distance[1]);
}

// takes a new reading from each sensor and updates the distance array
void updateSonar() {
  for (int i = 0; i < NUM_SONAR; i++) {
    distance[i] = sonar[i].ping_cm(); // update distance
    
    // sonar sensors return max distance if no obstacle is detected
    if (distance[i] == 0)
      distance[i] = MAX_DISTANCE;
  }
}
