//libraries
#include <Arduino.h>

#include <Servo.h>

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

//function headers
long scanDistance();

void changeDirection();

void forward();
void backward();
void halt();
void turnRight();
void turnLeft();

void transmit();
void receive();

//PINS ARE ALL DEFINED HERE
//servo
#define servoPin 13

//ultrasonic sensor
#define echoPin 12
#define trigPin 11

//motor driver
#define in1 10
#define in2 9
#define in3 8
#define in4 7

//objects
Servo servo;

RH_ASK transmitter;
RH_ASK receiver;
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

//radio stuff
  transmitter.init();
  receiver.init();

}

void loop() {
  // put your main code here, to run repeatedly:

  // long distance = scanDistance();
  // if (distance <= 30) {
  //   halt();
  //   changeDirection();
  // } else { 

  //     forward();
  // }
  // delay(500);

  transmit();

  receive();
}

//radio stuff
//FOR TESTING ONLY- THE ROBOT WILL NOT TRASMIT DATA, IT'LL ONLY RECEIVE
void transmit(){

  //TESTING ONLY, REPLACE L8R
  char *msg = "Hello World!";
  transmitter.send((uint8_t *)msg, strlen(msg));
  transmitter.waitPacketSent();
  delay(1000);

}


//----radio stuff over----//

//receive and decrypts a radio wave
void receive() {

  //TEST, REPLACE L8R
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  if(receiver.recv(buf, &buflen)) {
    Serial.print("Message: ");
    Serial.println((char*)buf);
  } else {
    Serial.println("No message received");
  }

}




//look around with servo, orient self correctly, reset servo, exit
void changeDirection(){

  //check left
  servo.write(0);
  long distance = scanDistance();

  delay(2000);

  if(distance > 30) {

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

  if(distance > 30) {

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
  delay(200);
}

void backward(){

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(200);

}

void halt() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(200);


}

void turnRight() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(200);


}

void turnLeft() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(200);

}


