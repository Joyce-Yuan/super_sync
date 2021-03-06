//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint8_t PIN = 6;
char text[32] = {0};

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

char check[] = "Button Pressed";

void setup(){
  pinMode(PIN, OUTPUT);
  while (!Serial);
    Serial.begin(115200);
  
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();
  Serial.print("Started Listening");
}

void loop(){
  //Read the data if available in buffer
  if (radio.available())
  {
    strcpy(text, "");
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
  if (strcmp(text, check) == 0){
    digitalWrite(PIN, HIGH);
  }
  else{
    digitalWrite(PIN, LOW);
  }
}

