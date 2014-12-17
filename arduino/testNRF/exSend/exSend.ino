#include <SPI.h>
#include <RF24.h>
#include "printf.h" 

typedef struct{
  int acc[3];
  int gyr[3];
  float angle[3];
}IMU_data;

#define LED 2
#define RF_CS 9
#define RF_CSN 10
RF24 radio(RF_CS, RF_CSN);
const uint64_t pipes[2] = { 0xe7e7e7e7e7LL, 0xc2c2c2c2c2LL };
int track_process = 5;

//char data = 'a';
//int data = 7;
//float data = 123.456;
char data[32] = {'a', 'b', 'c', 'd'};  // we'll transmit a 32 byte packet
//int data[9] = {123, 456, 789, 987, 654, 321, 753, 951, 852};
//IMU_data data;

void setup() {
  pinMode(track_process, OUTPUT);
  Serial.begin(19200);
  printf_begin();
  radio.begin();
  radio.openWritingPipe(pipes[0]);
  radio.printDetails();
}
 
void loop() {
//  Serial.print("size of data :");
//  Serial.println(sizeof(data));
  digitalWrite(track_process, HIGH);
  radio.write( &data, sizeof(data) );
  digitalWrite(track_process, LOW);
  delay(5);
  //delay(100); // wait a second and do it again. 
}
