#include <SPI.h>
#include <RF24.h>
#include "printf.h"

typedef struct{
  int acc[3];
  int gyr[3];
  float angle[3];
}IMU_data;

#define RF_CS 9
#define RF_CSN 7
RF24 radio(RF_CS, RF_CSN);
const uint64_t pipes[2] = { 0xe7e7e7e7e7LL, 0xc2c2c2c2c2LL };
int track_process = 5;

//IMU_data rx_data;
//char rx_data;
//int rx_data;
//float rx_data;
char rx_data[32];
void setup() {
  pinMode(track_process, OUTPUT);
  Serial.begin(19200);
  printf_begin();
  radio.begin();
//  radio.openWritingPipe(pipes[1]);    // note that our pipes are the same above, but that
  radio.openReadingPipe(1, pipes[0]); // they are flipped between rx and tx sides.
  radio.startListening();
  radio.printDetails();
}

void loop() {
  if (radio.available()) {
 
//    Serial.println("--------------------------------------------------------------------------------");
   digitalWrite(track_process, HIGH);
    bool done = false;
    while (!done) {
      done = radio.read( &rx_data, sizeof(rx_data) );
    }
    digitalWrite(track_process, LOW);
    Serial.print(rx_data);
    printf("\nGot payload @ %lu... \r\n", millis());
    
  }
}
