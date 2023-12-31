//libraries
#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>

//function headers
long scanDistance();

void changeDirection();

void forward();
void backward();
void halt();
void turnRight();
void turnLeft();

// void transmit();
// void receive();

//PINS ARE ALL DEFINED HERE
//servo
#define servoPin 13

//ultrasonic sensor
#define echoPin 6
#define trigPin 5

//motor driver
#define in1 10
#define in2 9
#define in3 8
#define in4 7

//nodeMCU interaction
#define nodeMCUInterrupt 2

//objects
Servo servo;

int beaconID = 0;
int dist = 50;
int wait = 150;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  //ULTRASONIC SENSOR
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //SERVO
  servo.attach(servoPin);
  servo.write(90);

  //MOTOR DRIVER 
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);


//set all motors to low 
  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

//nodeMCU interaction
  pinMode(nodeMCUInterrupt, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(nodeMCUInterrupt), startSearch, CHANGE);

}

void loop() {
  // put your main code here, to run repeatedly:

  long distance = scanDistance();
  if (distance <= dist) {
    halt();
    changeDirection();
  } else { 

      forward();
  }
  delay(500);

}

//DEPRECATED FOR NOW - still haven't chosen a signal to use

//radio stuff
//FOR TESTING ONLY- THE ROBOT WILL NOT TRASMIT DATA, IT'LL ONLY RECEIVE
// void transmit(){

//   //TESTING ONLY, REPLACE L8R
//   char *msg = "Hello World!";
//   transmitter.send((uint8_t *)msg, strlen(msg));
//   transmitter.waitPacketSent();
//   delay(1000);

// }

//receive and decrypts a radio wave
// void receive() {

//   //TEST, REPLACE L8R
//   uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
//   uint8_t buflen = sizeof(buf);
//   if(receiver.recv(buf, &buflen)) {
//     Serial.print("Message: ");
//     Serial.println((char*)buf);
//   } else {
//     Serial.println("No message received");
//   }

// }

//----radio stuff over----//

//start searching for beacon
void startSearch() {

  halt();
  
    //check if the interrupt was triggered by a rising edge
    beaconID = Serial.read();
    if (beaconID != 0) {
      findBeacon();
    }
}


//rotate till signal is relative to the robot
void findBeacon() {

  /*lets write the logic here:
  1. IF cant find any beacon, keep rotating
  2. WHEN  signal found, stop rotating
    2.1 Rotate until the  pulse is at the center of the robot
  3. Check  pulse matches the ID we're looking for
  4. IF not, keep rotating
  5. IF yes, stop rotating, move forward
  6. give control to anti-collision system
  7. do find beacon again when anti-collision causes robot to change direction //INCLUDE THIS FUNCTION IN THE changeDirection() function
  8. 
  */

 


}


//look around with servo, orient self correctly, reset servo, exit
void changeDirection() {

  //check left
  servo.write(0);
  long distance = scanDistance();

  delay(2000);

  if(distance > dist) {

    //turn left
    turnLeft();

    //reset servo position
    servo.write(90);
    delay(1000);
    return;
  }

  //check right
  servo.write(180);
  distance = scanDistance();
  
  delay(2000);

  if(distance > dist) {

    //turn right
    turnRight();

    //reset servo position
    servo.write(90);
    delay(1000);
    return;


  }

  //reset servo position
  servo.write(90);
  delay(1000);

  reverse();

}



//get ultrasonic sensor reading (returns distance in cm)
long scanDistance() {

  long distance, duration;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  
  return distance;


}




//lower-level motor functions, pretty standard

void forward() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(wait);
}

void backward() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(wait);

}

void halt() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(wait);


}

void turnRight() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(wait);


}

void turnLeft() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(wait);

}

void reverse() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(wait);
}



