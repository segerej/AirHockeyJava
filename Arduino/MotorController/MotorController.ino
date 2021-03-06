#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <avr/interrupt.h>

// X - Direction Motor Settings
#define X_MOTOR_PIN 10
#define X_MOTOR_DIRECTION_PIN 11
#define X_MOTOR_MAX_SPEED 1500
#define X_MOTOR_ACCELERATION 10000

// Y - Direction Motor Settings
#define Y_MOTOR_PIN 5
#define Y_MOTOR_DIRECTION_PIN 6
#define Y_MOTOR_MAX_SPEED 5000
#define Y_MOTOR_ACCELERATION 100000

// Game Goal detection settings
#define RB_GOAL 2
#define USR_GOAL 3
#define DEBOUNCE_TIME 300

// Other constants
#define FIELD_DELIMITER ','
#define OUTPUT_POSITION_PREFIX '_'
#define PULL_NEXT_POSITION_CHAR 'N'

AccelStepper stepperX(AccelStepper::DRIVER, X_MOTOR_PIN, X_MOTOR_DIRECTION_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_MOTOR_PIN, Y_MOTOR_DIRECTION_PIN);

long serialCoordinates[2];         // Input X,Y coordinates from Serial connection
int serialScore[2];             // Input User and Robot score from Serial connection
volatile uint8_t currentScore[2];             // Current User and Robot scores
volatile unsigned long currentTimeRB;         // RB Goal debounce times
volatile unsigned long lastTimeRB;
volatile unsigned long currentTimeUSR;        // USR Goal debounce times
volatile unsigned long lastTimeUSR;

int directionVector[2];
int fieldIndex = -1;
char type;// Type of data being received
int outputDelayCounter = 0;
Adafruit_7segment matrix = Adafruit_7segment();
String currentSerialOutput = "";   // The current serial message to output

int outputBufferIndex = 0;
String queuedSerialOutput[2];    // The queue of output messages. PLEASE USE FOR ALL PRINT MESSAGES
const int OUTPUT_BUFFER_SIZE = 2;
int serialOutputIndex = 0;
boolean readyForNextInput = false;

void setup(){
    #ifndef __AVR_ATtiny85__
    Serial.begin(9600);            // Set the baud rate on the serial line
    #endif
    stepperX.setMaxSpeed(X_MOTOR_MAX_SPEED);
    stepperX.setAcceleration(X_MOTOR_ACCELERATION);
    
    stepperY.setMaxSpeed(Y_MOTOR_MAX_SPEED);
    stepperY.setAcceleration(Y_MOTOR_ACCELERATION);
    
    directionVector[0] = 1;
    directionVector[1] = 1;
    
    // Initialize and display the score
    matrix.begin(0x70);
    displayScore();

    // Enable RB and USR side goal interrupts for goal detection
    pinMode(RB_GOAL, INPUT_PULLUP);      
    pinMode(USR_GOAL, INPUT_PULLUP);
    attachInterrupt(0, updateRBScore, RISING);
    attachInterrupt(1, updateUSRScore, RISING);
    
    queuedSerialOutput[0] = "";
    queuedSerialOutput[1] = "";

}

/*
* Looks for input messages of this form (X, Y, ScoreU, ScoreR)
* ex. > 12355,123466666,0,1
*/
boolean getSerialInput(){
    boolean gotInput = false;
    
    if (Serial.available()){
        char ch = (char) Serial.read();
        
        // First value in sequence will be the type prefix
        // This will be an alpha character
        if (ch >= 'A' && ch <= 'Z')          // Determine the type of data being passed to the arduino
        {
            type = ch;            // Make type equal to the correct type of data being received  
            fieldIndex++;
      }
        
        else if(ch >= '0' && ch <= '9')     // Is this an ascii digit between 0 and 9?
        {
            if(type == 'P'){                  // Positional data is being received
                if (fieldIndex <= 1){
                    // Accumulate the Coordinate value
                    serialCoordinates[fieldIndex] = (serialCoordinates[fieldIndex] * 10) + (ch - '0');
                }
            }
            else if(type == 'S'){              // Score data is being received
                if (fieldIndex <= 1){
                    // Accumulate the Score value
                    serialScore[fieldIndex] = (serialScore[fieldIndex] * 10) + (ch - '0');
                }
            }
        }
        else if (ch == '-' && fieldIndex <= 1 && fieldIndex >= 0){
          directionVector[fieldIndex] = -1;
        }
        
        else if (ch == FIELD_DELIMITER)      // Move to next field for current type when hitting delimiter
        {
            if (fieldIndex < 1) {
                fieldIndex++;                   // Increment field index
            }
        }
        else {
        
        	if (fieldIndex != 1){
        		//Serial.println("Incorrect Input Format");
        	        fieldIndex = -1;                  // Reset field index for next type
        		return false;
      		}

            // Any character not a digit or comma ends the acquisition of fields
            // Signal back to the Java system that we're ready for the next update
            // This is a "pull" technique to avoid race conditions on values getting overwritten
            // on the line and potentially causing fatal mechanical malfunctions!
                serialCoordinates[0] *=  directionVector[0];
                serialCoordinates[1] *=  directionVector[1];
                directionVector[0] = 1;
                directionVector[1] = 1;
                
        	fieldIndex = -1;                  // Reset field index for next type
        	gotInput = true;
        }
    }else{
        readyForNextInput = true;
    }
    return gotInput;
}

void resetCoordinates(){
    serialCoordinates[0] = 0;
    serialCoordinates[1] = 0;
}

void moveToPosition(){

    //long distanceX = serialCoordinates[0] - stepperX.currentPosition();
    //long distanceY = serialCoordinates[1] - stepperY.currentPosition();
    //Serial.println("Moving " + distanceX + " in the X, and " + distanceY + " in the Y.");
    
    stepperX.moveTo(serialCoordinates[0]);
    stepperY.moveTo(serialCoordinates[1]);
    
    //Reset coordinates for next input
    resetCoordinates();
}

void displayScore(){
    //matrix.writeDigitNum(0, 0, false);
    matrix.writeDigitNum(1, currentScore[0], true);
    matrix.drawColon(true);
    matrix.writeDigitNum(3, currentScore[1], true);
    //matrix.writeDigitNum(4, 0, false);
    matrix.writeDisplay();
    
    serialScore[0] = currentScore[0];
    serialScore[1] = currentScore[1];
}

void updateRBScore(){
  // Debounce the switch 
  currentTimeRB = millis();
  if((currentTimeRB - lastTimeRB) >= DEBOUNCE_TIME){
    currentScore[1]++;    // Increment Robot score
  }
  lastTimeRB = millis();
}

void updateUSRScore(){
  // Debounce Switch
  currentTimeUSR = millis();
  if((currentTimeUSR - lastTimeUSR) >= DEBOUNCE_TIME){
    currentScore[0]++;    // Increment Robot score
  }
  lastTimeUSR = millis();
}

void resetScore(){
    currentScore[0] = 0;
    currentScore[1] = 0;
}

void printNextChar(){
  if(currentSerialOutput != ""){
    if(serialOutputIndex < currentSerialOutput.length()){
      Serial.print(currentSerialOutput[serialOutputIndex]);
      serialOutputIndex++;
    }else{
      Serial.print('\n');
      serialOutputIndex = 0;
      currentSerialOutput = "";
    }
    
  }else{
      currentSerialOutput = queuedSerialOutput[outputBufferIndex];
      queuedSerialOutput[outputBufferIndex] = "";
      outputBufferIndex = (outputBufferIndex + 1) % OUTPUT_BUFFER_SIZE;   
  }
}

void loop(){
    if(getSerialInput()){
        moveToPosition();
    }
    
    if(serialScore[0] != currentScore[0] || serialScore[1] != currentScore[1]){
        displayScore();
    }
    
    stepperX.run();
    stepperY.run();
    
    // if there is room in the outputbuffer send the following messages
    if(Serial.isOutputBufferEmpty()){
      // Send back current motor position
      Serial.println((String) OUTPUT_POSITION_PREFIX + stepperX.currentPosition() + (String) FIELD_DELIMITER + stepperY.currentPosition());
       
       if(readyForNextInput){
          Serial.println(PULL_NEXT_POSITION_CHAR);
          readyForNextInput = false;
       }  
    }
//    printNextChar();
}

