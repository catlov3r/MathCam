/*

  This is designed to hook up to a serial enabled LCD,
  a 12 button keypad, and an optoisolator to control
  a Canon Rebel digital SLR.

  The software serial library is used to connect the LCD
  to pints 9 and 10. The keypad is connected to pins
  2, 3, 4, 5 (rows), 6, 7, 8 (columns). This is connected
  using an RJ45 cable so the key pad can be easily moved
  to a convenient place away from the microcontroller.
  Pin 12 is connected to the opto-isolator to control the
  shutter release.

  For the opto isolator pin 12 on the arduino is conencted
  to pin 2 on the opto. Pin 1 on the opto goes thought a
  10k resistor to ground. Pins 4 and 5 on the opto are
  connected to the camera via serio jack that fits the
  camera. Experimentation will be needed to figure out
  which 2 of the 3 wires in the cable release the
  shutter. The opto isolator is used to prevent damage
  to the camera. Also, the camera is designed so when
  two pins are connected it either focuses or releases
  the shutter, meaning no external power is needed.

  Analog 0 is used to help with the random number
  generator. The basic idea is to have a 2 or 3
  number math equation that has a positive answer
  generated and displayed on the screen. The keypad
  is then used as input (which is displayed on the
  screen) to answer the math problem. If the problem
  is answered correctly the camera will take a picture.
  Typically the camera is hooked up to a large TV or
  projector so that the image captured is displayed.
  The result is kept positive since the keypad used
  does not have a labeled minus sign.

*/

#include <Keypad.h>
#include <SoftwareSerial.h>
SoftwareSerial lcd(10, 9); // start the softserial port for the LCD

static char buildStr[12];
static byte buildCount;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
// this will be used with the keypad library to interperate what
// each key does.
char keys[ROWS][COLS] = {
  {
    '1', '2', '3'
  }
  ,
  {
    '4', '5', '6'
  }
  ,
  {
    '7', '8', '9'
  }
  ,
  {
    '*', '0', '#'
  }
};

byte rowPins[ROWS] = {
  5, 4, 3, 2
}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  8, 7, 6
}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

boolean ledPin_state;

long randNum1, randNum2, randNum3, mathOne, mathTwo;
char *randOp1, *randOp2;
boolean newEq = true;

int led = 13;
int opto = 12; // pin to connect the opto isolator to

void setup() {
  Serial.begin(9600); // start the serial ports
  lcd.begin(9600);
  pinMode(led, OUTPUT);              // Sets the digital pin as output.
  pinMode(opto, OUTPUT);             // Set pin the optocoupler is hooked up to as output.
  digitalWrite(led, HIGH);           // Turn the LED on.
  ledPin_state = digitalRead(led);   // Store initial LED state. HIGH when LED is on.
  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad

  randomSeed(analogRead(0)); // generate some random information

  delay(500);
  lcd.write(254); // cursor to beginning of first line
  lcd.write(128);
  lcd.write("Welcome to SWVGS"); // clear display + legends
  lcd.write("                ");
  delay(3000);

}

void loop() {
  char key = keypad.getKey(); // read the character pushed on the keypad


  if (newEq == true) {  // generate three random numbers
    randNum1 = random(0, 99);
    randNum2 = random(0, 99);
    randNum3 = random(0, 99);

    //determine if which number is bigger to make a positive result
    if (randNum1 > randNum2) {
      randOp1 = "-";
      mathOne = randNum1 - randNum2;
    }
    else {
      randOp1 = "+";
      mathOne = randNum1 + randNum2;
    }
    /* // determine if the third random number is larger than the result of the first
       // so that the equation ends positive. This section makes a harder math equation
       // if uncommencted
      if (mathOne > randNum3) {
      randOp2 = "-";
      mathTwo = mathOne - randNum3;
      }
      else {
      randOp2 = "+";
      mathTwo = mathOne + randNum3;
      }/**/

    // Print to the serial port for debugging
    Serial.print(randNum1);
    Serial.print(randOp1);
    Serial.print(randNum2);
    //   Serial.print(randOp2);
    //   Serial.println(randNum3);
    //   Serial.println(mathTwo);
    lcd.write(254); // cursor to beginning of first line
    lcd.write(128);
    lcd.write("                "); // clear display + legends
    lcd.write("                ");
    char tmpstr[10];
    sprintf(tmpstr, "%d", randNum1);
    lcd.write(tmpstr);
    lcd.write(randOp1);
    sprintf(tmpstr, "%d", randNum2);
    lcd.write(tmpstr);
    //    lcd.write(randOp2);
    //    sprintf(tmpstr,"%d",randNum3);
    //    lcd.write(tmpstr);
    lcd.write("=");

    //lcd.write(254);
    //lcd.write(192);

    newEq = false;
  }

  // read the key presses
  if (key) {
    if (buildCount < 12) {
      buildStr[buildCount++] = key;
      buildStr[buildCount] = '\0';
      Serial.println(buildStr);
      lcd.write(254);
      lcd.write(192);
      lcd.write(buildStr);
    }
    else {
      lcd.write(254);
      lcd.write(128);
      lcd.write("                ");
      lcd.write("                ");
      buildStr[buildCount];
      buildCount = 0;
    }
  }

}

// Taking care of some special events.
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      if (key == '#') { // if the # key is pressed check the equation
        // check to see if what was entered is correct
        // replace 'mathOne' with 'mathTwo' if you are using the longer equation
        if (atoi(buildStr) == mathOne) { //mathTwo
          lcd.write(254);
          lcd.write(128);
          lcd.write("                ");
          lcd.write("                ");
          lcd.write(254);
          lcd.write(128);
          lcd.write("Correct!        ");
          lcd.write("3..");
          Serial.println("Correct"); // give time to make funny faces
          Serial.println("3.."); // countdown
          delay(1000);
          lcd.write(254);
          lcd.write(192);
          lcd.write("2..");
          Serial.println("2..");
          delay(1000);
          lcd.write(254);
          lcd.write(192);
          lcd.write("1..");
          Serial.println("1..");
          delay(1000);
          digitalWrite(led, HIGH); // turn LED 13 on
          digitalWrite(opto, HIGH); // trigger the camera
          delay(1000);
          digitalWrite(led, LOW);
          digitalWrite(opto, LOW);
        }
        else { // if the answer is incorrect don't take a picture
          Serial.print("Incorrect. Answer should be: ");
          Serial.println(mathOne); // mathTwo
          lcd.write(254);
          lcd.write(128);
          lcd.write("                ");
          lcd.write("                ");
          lcd.write(254);
          lcd.write(128);
          lcd.write("Incorrect, the  ");
          lcd.write("answer is: "); // give the correct answer so they can learn
          char tmpstr[10];
          // if using the harder equation replace 'mathOne' with 'mathTwo'
          sprintf(tmpstr, "%d", mathOne); // mathTwo
          lcd.write(tmpstr);
          delay(2500);
        }
        buildStr[buildCount];
        buildCount = 0;
        newEq = true; // tell it to generate a new equation.
      }
      break;

    case RELEASED:
      if (key == '*') { // if * is released clear the screen and input
        buildStr[buildCount];
        buildCount = 0;
        lcd.write(254);
        lcd.write(192);
        lcd.write("                ");
      }
      if (key == '#') { // when # is released clear the input and screen
        buildStr[buildCount];
        buildCount = 0;
        lcd.write(254);
        lcd.write(192);
        lcd.write("                ");
      }
      break;

    case HOLD:
      if (key == '*') { // when * is held clear the screen and input.
        buildStr[buildCount];
        buildCount = 0;
        lcd.write(254);
        lcd.write(192);
        lcd.write("                ");
      }
      break;
  }
}














