/*
This code is written by Simple Electronics (https://simpleelectronics.ca) & (https://youtube.com/simpleelectronics) and has been
tested to work as-is - but please feel free to make your own improvements!
ChatGPT was used as a rubber-ducky and did not wholesale write this code - Dan from Simple Electronics did.
Feel free to distribute as you wish - this was meant to help people who can't code well achieve their goals.
*/

#include <Servo.h> // Library needed to handle servo movement

Servo servoOne; // Creating a specific servo "object" - this is from the Servo library

int servoPos = 0; // This variable will output the requested angle for the servo
int potPos = 0; // This variable will track the position of the wiper of the potentiometer

int modeSelect = 1; // This variable will track which mode we are in - pot control, centering, sweeping

int incrementDir = 1; // This variable will be modified to increment or decrement the servo angle

const int potPin = 26; // Sets GP26 to the wiper of the potentiometer
const int selectPin = 8; // Sets GP8 to the button we will use to toggle the mode of the program
const int modeOne = 1; // Sets GP1 to be our mode 1 indicator LED driver
const int modeTwo = 2; // Sets GP2 to be our mode 2 indicator LED driver
const int modeThree = 3; // Sets GP3 to be our mode 3 indicator LED driver

unsigned long lastPressTime = 0; // This variable allows us to track the time since the last button press
const int debounceDelay = 100; // Settable delay to debounce the switch - 100ms debounce as default

void setup() {
  servoOne.attach(0); // Attaches the servo "object" to pin GP0

  pinMode(selectPin, INPUT_PULLUP); // Activating the pullup resistor on the input pin - only needed to activate the resistor
  pinMode(modeOne, OUTPUT);
  pinMode(modeTwo, OUTPUT);
  pinMode(modeThree, OUTPUT);
}

void loop() {

  // This block deals with the mode selection - essentially, when the modeSelect is 1, the program is in center mode, 2 is sweep and 3 is potentiometer control
  if (digitalRead(selectPin) == LOW){
    if (millis() - lastPressTime > debounceDelay) {
      lastPressTime = millis(); // Update last press time
      modeSelect++;
      if (modeSelect > 3){ // We only have 3 modes, so we want to have it cycle instead of incrementing forever
        modeSelect = 1;
      }
    }
  }

  if (modeSelect == 3){
    digitalWrite(modeOne, LOW); // Block of code to turn on only our selected mode indicator LED
    digitalWrite(modeTwo, LOW);
    digitalWrite(modeThree, HIGH);
    potPos = analogRead(potPin); // Reads the position of the potentiometer and stores the value
    servoPos = map(potPos, 0, 1023, 0, 180); // Converts the analogRead values (0-1023) to a servo angle (0-180)
  }

  if (modeSelect == 1) {
    digitalWrite(modeOne, HIGH); // Block of code to turn on only our selected mode indicator LED
    digitalWrite(modeTwo, LOW);
    digitalWrite(modeThree, LOW);
    servoPos = 90; // Sets the servo to center position when in this mode
  }

  if (modeSelect == 2) {
    digitalWrite(modeOne, LOW); // Block of code to turn on only our selected mode indicator LED
    digitalWrite(modeTwo, HIGH);
    digitalWrite(modeThree, LOW);
    servoPos = servoPos + incrementDir; // Creates a sweep of the servo arm
    if (servoPos >= 180 || servoPos <= 0){ // When the servo position is over or under the minimum and the maximum, flip it the other way
      incrementDir = -incrementDir;
    }   
  }
  
  servoOne.write(servoPos); // Outputs the servo position in degrees (0-180)
  delay(15); // This delay allows the servo time to move - adjust this at will.
  
}
