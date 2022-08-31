// Soccer droid
// Defend the ball at the goal

#include <Arduino.h>
#include <Pixy2.h>

// -------------------------------------------------------
// Global Variables
// -------------------------------------------------------
int rightMotorLogicPin1 = 7;    
int rightMotorLogicPin2 = 8;  
int rightMotorEnable = 9;  
int leftMotorLogicPin1 = 2;    
int leftMotorLogicPin2 = 10; 
int leftMotorEnable = 3; 
int backMotorLogicPin1 = 4;    
int backMotorLogicPin2 = 5;  
int backMotorEnable = 6;
int LeftSpeed = 235/2;
int RightSpeed = 245/2;
int BackSpeed = 230/2;
int MaxLeftSpeed = 255;
int MaxRightSpeed = 255;

// Camera variables
Pixy2 pixy;
int ballX;
int ballY;
int ballWidth;
int ballArea;
int goalX;
int goalY;

// -------------------------------------------------------
// Subroutine: Rotate Left
// -------------------------------------------------------
void rotateLeft() {
  analogWrite(leftMotorEnable, 0.6*LeftSpeed);
  analogWrite(rightMotorEnable, 0.6*RightSpeed); 
  analogWrite(backMotorEnable, 0.6*BackSpeed); 
  digitalWrite(leftMotorLogicPin1, LOW);
  digitalWrite(leftMotorLogicPin2, HIGH);
  digitalWrite(rightMotorLogicPin1, LOW);
  digitalWrite(rightMotorLogicPin2, HIGH);
  digitalWrite(backMotorLogicPin1, LOW);
  digitalWrite(backMotorLogicPin2, HIGH);
  
}

// -------------------------------------------------------
// Subroutine: Rotate Right
// -------------------------------------------------------
void rotateRight(){
  analogWrite(leftMotorEnable, LeftSpeed);
  analogWrite(rightMotorEnable, RightSpeed); 
  analogWrite(backMotorEnable, BackSpeed); 
  digitalWrite(leftMotorLogicPin1, HIGH);
  digitalWrite(leftMotorLogicPin2, LOW);
  digitalWrite(rightMotorLogicPin1, HIGH);
  digitalWrite(rightMotorLogicPin2, LOW);
  digitalWrite(backMotorLogicPin1, HIGH);
  digitalWrite(backMotorLogicPin2, LOW);
  
}

// -------------------------------------------------------
// Subroutine: Move Forward
// -------------------------------------------------------
void moveForward() {
  analogWrite(leftMotorEnable, LeftSpeed);
  analogWrite(rightMotorEnable, RightSpeed); 
  analogWrite(backMotorEnable, 0); 
  digitalWrite(leftMotorLogicPin1, HIGH);
  digitalWrite(leftMotorLogicPin2, LOW);
  digitalWrite(rightMotorLogicPin1, LOW);
  digitalWrite(rightMotorLogicPin2, HIGH);   
  digitalWrite(backMotorLogicPin1, HIGH);
  digitalWrite(backMotorLogicPin2, HIGH);  
}

// -------------------------------------------------------
// Subroutine: Move Forward
// -------------------------------------------------------
void moveBackward() {
  analogWrite(leftMotorEnable, LeftSpeed);
  analogWrite(rightMotorEnable, RightSpeed); 
  analogWrite(backMotorEnable, 0); 
  digitalWrite(leftMotorLogicPin1, LOW);
  digitalWrite(leftMotorLogicPin2, HIGH);
  digitalWrite(rightMotorLogicPin1, HIGH);
  digitalWrite(rightMotorLogicPin2, LOW);   
  digitalWrite(backMotorLogicPin1, HIGH);
  digitalWrite(backMotorLogicPin2, HIGH);  
}

// -------------------------------------------------------
// Subroutine: Full Speed Forward
// -------------------------------------------------------
void speedForward() {
  analogWrite(leftMotorEnable, MaxLeftSpeed);
  analogWrite(rightMotorEnable, MaxRightSpeed); 
  digitalWrite(leftMotorLogicPin1, HIGH);
  digitalWrite(leftMotorLogicPin2, LOW);
  digitalWrite(rightMotorLogicPin1, LOW);
  digitalWrite(rightMotorLogicPin2, HIGH); 
  delay(1000);    
}

// -------------------------------------------------------
// Subroutine: Orbit the ball
// -------------------------------------------------------
void Orbit() {
  analogWrite(backMotorEnable, BackSpeed);
  digitalWrite(backMotorLogicPin1, LOW);
  digitalWrite(backMotorLogicPin2, HIGH);      
}

// -------------------------------------------------------
// Subroutine: Stay still
// -------------------------------------------------------
void Still(){
  analogWrite(leftMotorEnable, 0);
  analogWrite(rightMotorEnable, 0); 
  analogWrite(backMotorEnable, 0); 
  digitalWrite(leftMotorLogicPin1, HIGH);
  digitalWrite(leftMotorLogicPin2, HIGH);
  digitalWrite(rightMotorLogicPin1, HIGH);
  digitalWrite(rightMotorLogicPin2, HIGH);
  digitalWrite(backMotorLogicPin1, HIGH);
  digitalWrite(backMotorLogicPin2, HIGH);
}

// -------------------------------------------------------
// Function: Locate Ball
// -------------------------------------------------------
bool locateBall() {

  int area;
  int maxArea = 0;
  bool result = false;

  // Scan for signature blocks
  pixy.ccc.getBlocks();

  // Enumerate any blocks
  for (int i = 0; i < pixy.ccc.numBlocks; i++) {
    
    // Is this block the ball, ie. signature 1
    if (pixy.ccc.blocks[i].m_signature == 1) {
      area = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
      
      // Is this the biggest block
      if ((area > 200) && (area > maxArea)) {
        maxArea = area;
        ballArea = area;
        ballWidth = pixy.ccc.blocks[i].m_width;
        ballX = pixy.ccc.blocks[i].m_x;
        ballY = pixy.ccc.blocks[i].m_y;
        result = true;
      }
    }
  }

  return result;
   
}


// -------------------------------------------------------
// Subroutine: Initialize Motor Pins
// -------------------------------------------------------
void initializeMotorPins(){

  // Configuration the motor pins
  pinMode(rightMotorLogicPin1, OUTPUT);   
  pinMode(rightMotorLogicPin2, OUTPUT);
  pinMode(rightMotorEnable, OUTPUT);
  pinMode(leftMotorLogicPin1, OUTPUT);
  pinMode(leftMotorLogicPin2, OUTPUT);
  pinMode(leftMotorEnable, OUTPUT);
  pinMode(backMotorLogicPin1, OUTPUT);
  pinMode(backMotorLogicPin2, OUTPUT);
  pinMode(backMotorEnable, OUTPUT);

  // Set the Motors to 0
  analogWrite(rightMotorLogicPin1, 0);   
  analogWrite(rightMotorLogicPin2, 0);   
  analogWrite(leftMotorLogicPin1, 0);   
  analogWrite(leftMotorLogicPin2, 0);
  analogWrite(backMotorLogicPin1, 0);   
  analogWrite(backMotorLogicPin2, 0);
  

  // Print the motor pin configuration for wiring
  Serial.println("Right Motor Pin 1 = 12");
  Serial.println("Right Motor Pin 2 = 13"); 
  Serial.println("Left Motor Pin 1 = 2"); 
  Serial.println("Left Motor Pin 2 = 1");
  Serial.println("Back Motor Pin 1 = 8"); 
  Serial.println("Back Motor Pin 2 = 9");
}

void setup()
{
  Serial.begin(115200);
  
  // Print program details
  Serial.println("Program: Soccer Droid");
  Serial.println("Initializing...");
  
  // Call a subroutine to initialize the motor pins
  initializeMotorPins();
  
  pixy.init();
  
  // Initialization completed successfully
  Serial.println("Initialization complete");
}

void loop() { 
  if (locateBall()== true) {
    if (ballX < 80) {
      // Rotate right
      Serial.println("go right");
      rotateRight();          
  
    } else if(ballX > 235) {
      // Rotate left
      Serial.println("go left");
      rotateLeft();
        
    } else {
      if (ballWidth > 20) {
        speedForward();
        Serial.println("Forward");
        moveBackward();
        Serial.println("Backward");
        delay(1300);
        Still();
        delay(1000); 
      }
    }
  }
  delay(5);  
}