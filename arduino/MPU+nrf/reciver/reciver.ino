#include <SPI.h>
#include <RF24.h>
//#include "printf.h"

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
IMU_data rx_data;
//char rx_data;
//char rx_data[32];
char ack = '1';
void setup() {
  pinMode(track_process, OUTPUT);
  Serial.begin(19200);
//  printf_begin();
  radio.begin();
//  radio.openWritingPipe(pipes[1]);    // note that our pipes are the same above, but that
  radio.openReadingPipe(1, pipes[0]); // they are flipped between rx and tx sides.
  radio.startListening();
  radio.printDetails();
}

void loop() {
  if (radio.available()) {
 
//    Serial.println("--------------------------------------------------------------------------------");
    bool done = false;
//    while (!done) {
    digitalWrite(track_process, HIGH);
    done = radio.read( &rx_data, sizeof(rx_data) );
    digitalWrite(track_process, LOW);
//    Serial.print("Got payload @");
//    Serial.println(millis());
//    }
  Serial.print(F("DEL:"));   Serial.print(0, DEC);
  Serial.print(F("#ACC:"));              
  Serial.print(rx_data.acc[0]);       Serial.print(F(","));
  Serial.print(rx_data.acc[1]);       Serial.print(F(","));
  Serial.print(rx_data.acc[2]);
  Serial.print(F("#GYR:"));
  Serial.print(rx_data.gyr[0]);       Serial.print(F(","));
  Serial.print(rx_data.gyr[1]);       Serial.print(F(","));
  Serial.print(rx_data.gyr[2]);
  Serial.print(F("#FIL:"));             
  Serial.print(rx_data.angle[0], 2);  Serial.print(F(","));
  Serial.print(rx_data.angle[1], 2);  Serial.print(F(","));
  Serial.print(rx_data.angle[2], 2); 
Serial.println(F(""));
    //print struct data type
//    for(int i = 0; i<3; i++)
//      Serial.println(rx_data.acc[i]);
//    
//    for(int i = 0; i<3; i++)
//      Serial.println(rx_data.gyr[i]);
//    
//     for(int i = 0; i<3; i++)
//      Serial.println(rx_data.angle[i]);
  }  else{
//    radio.printDetails();
  }
}
