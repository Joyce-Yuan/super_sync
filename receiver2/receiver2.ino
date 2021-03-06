//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <string.h>
#include <time.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

SoftwareSerial mySoftwareSerial(10, 11);                  // RX, TX
DFRobotDFPlayerMini myDFPlayer;                           // Create myDFPlayer of the DFPlayer Class

// constants for receiver:
char print_statement[100];
const uint8_t PIN = 6;
char text[32] = {0};
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
// {RED: 0, GREEN: 1, BLUE: 2, YELLOW: 3}
const int color = 0; 
const char* colorArray[] = {"RED", "GREEN", "BLUE", "YELLOW"};
// senders 0-3 = team 1
// senders 4-7 = team 2

// variables & state
int state = 0;
// state = 0, game setup
// state = 1, listening for input
// state = 2, score update
// state = 3, game over

int buttonState;

int currentColor;
int team1;
int team2;
int senderColor;
int senderTeam;


//address through which two modules communicate.
const byte address[6] = "00001";

void setup() {
  pinMode(PIN, OUTPUT);
  while (!Serial);
    Serial.begin(115200);
  sprintf(print_statement, "Setting up receiver tower");
  Serial.println(print_statement);
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // gamestate and setup
  state = 1;
  currentColor = generateColor();
  team1 = 0;
  team2 = 0;

}

void loop() {
  // put your main code here, to run repeatedly:
  // turn LED on:
  digitalWrite(ledPin, HIGH);
  
  // read the state of the reed switch:
  buttonState = digitalRead(buttonPin);

  // state = 0, game setup
  // state = 1, listening for input
  // state = 2, score update
  // state = 3, game over
  
  // game setup
  if (state == 0) {
    if (buttonState == HIGH){
      delay(500);
      state = 1;
      //play starting game music
    }
  }

  // listening for input
  else if (state == 1) {

    //Read the data if available in buffer
    if (radio.available()) {
      //Serial.println("reading");
      strcpy("", text);
      radio.read(&text, sizeof(text));
      Serial.println(text);
      senderTeam = (atoi(text[0]) < 4) ? 1 : 2;
      senderColor = atoi(text[1]) % 4;
      Serial.print("Team: ");
      Serial.println(senderTeam);
      Serial.print("Color: ");
      Serial.println(senderColor);

      //color check
      if (senderColor == currentColor){
        state = 2;
      }
    }

    if (buttonState == HIGH){
      Serial.print("ending");
      resetGame();
      state = 0;
    }
  }

  //score update
  else if (state == 2) {
    updateScore();
    if (team1 == 7 || team2 == 7){
      Serial.println("someone won");
      state = 3;
      //maybe play some music
    } else {
      state = 1;
      //go back to listening if no conditions met
      currentColor = generateColor();
    }
    if (buttonState == HIGH){
      resetGame();
      state = 0;
    }
  }

  //game end
  else if (state == 3) {
    if (buttonState == HIGH){
      resetGame();
      state = 0;
    }
  }

}

int generateColor() {
  srand(time(0));
  int temp = rand()%4;
  //insert LED logic for tower
  sprintf(print_statement, "tower lights up %s", colorArray[0]);
  Serial.println(print_statement);
  return 0;
  //return temp;
}

void updateScore() {
  if (senderTeam == 1){
    team1 = team1 + 1;
    // tower LED logic here
  }

  else if (senderTeam == 2){
    team2 = team2 + 1;
    // tower LED logic here
  }
  sprintf(print_statement, "updated score team 1: %d, team 2: %d", team1, team2);
  Serial.println(print_statement);
}

void resetGame() {
  Serial.println("resetting game");
  team1 = 0;
  team2 = 0;
  //reset all LEDs
  //maybe play some music
  delay(500);
}
