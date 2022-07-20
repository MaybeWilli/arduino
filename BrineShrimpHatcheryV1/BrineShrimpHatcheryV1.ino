#include <Servo.h>

int washRelayPin = 6;
int saltRelayPin = 7;
int rinseRelayPin = 8;
int servoRelayPin = 9;
int airRelayPin = 10;

int shrimpServoPin = 2;
int releaseServoPin = 3;
int armBaseServoPin = 4;
int forearmServoPin = 5;

Servo shrimpServo;
Servo releaseServo;
Servo armBaseServo;
Servo forearmServo;

int userInput = 1;

void setup() {
  // put your setup code here, to run once:11
  //servo1.attach(servoPin);
  pinMode(washRelayPin, OUTPUT);
  pinMode(saltRelayPin, OUTPUT);
  pinMode(rinseRelayPin, OUTPUT);
  pinMode(servoRelayPin, OUTPUT);
  pinMode(airRelayPin, OUTPUT);

  shrimpServo.attach(shrimpServoPin);
  releaseServo.attach(releaseServoPin);
  armBaseServo.attach(armBaseServoPin);
  forearmServo.attach(forearmServoPin);

  digitalWrite(washRelayPin, LOW);
  digitalWrite(saltRelayPin, LOW);
  digitalWrite(rinseRelayPin, LOW);
  digitalWrite(servoRelayPin, LOW);
  digitalWrite(airRelayPin, LOW);
  activateServos();
  delay(5000);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //oneStep();

  if (Serial.available())
  {
    delay(300);
    userInput = Serial.parseInt();
  }//*/

  if (userInput == 0)
  {
    //do nothing
  }//*/
  if (userInput == 1)
  {
    forearmServo.write(0); //arm straight
    //harvestShrimp();
    userInput = 0;
  }
  else if (userInput == 2)
  {
    forearmServo.write(70); //arm lowered
    //addShrimp(500);
    userInput = 0;
  }//*/
  else if (userInput == 3)
  {
    armBaseServo.write(0); //arm under valve
    //addShrimp(500);
    userInput = 0;
  }
  else if (userInput == 4)
  {
    armBaseServo.write(180); //arm over imaginary tank
    //addShrimp(500);
    userInput = 0;
  }
  else if (userInput == 5)
  {
    releaseServo.write(15); //valve closed
    //addShrimp(500);
    userInput = 0;
  }
  else if (userInput == 6)
  {
    releaseServo.write(160); //valvue opened
    //addShrimp(500);
    userInput = 0;
  }
  else if (userInput == 7)
  {
    deactivateServos();
    userInput = 0;
  }
  else if (userInput == 8)
  {
    activateServos();
    //addShrimp(500);
    userInput = 0;
  }
  else if (userInput == 9)
  {
    harvestShrimp();
    userInput = 0;
  }

  else if (userInput == 10)
  {
    harvest(1000);
  }

  else if (userInput == 11)
  {
    digitalWrite(rinseRelayPin, HIGH);
    userInput = 0;
  }
  else if (userInput == 12)
  {
    digitalWrite(rinseRelayPin, LOW);
    userInput = 0;
  }
  else if (userInput == 13)
  {
    shrimpServo.write(0);
    //addShrimp(500);
    userInput = 0; //close shrimp
  }
  else if (userInput == 14)
  {
    shrimpServo.write(140);
    //addShrimp(500);
    userInput = 0; //open shrimp
  }
  else if (userInput == 15)
  {
    digitalWrite(airRelayPin, HIGH);
    userInput = 0;
  }
  else if (userInput == 16)
  {
    digitalWrite(airRelayPin, LOW);
    userInput = 0;
  }
  else if (userInput == 17)
  {
    digitalWrite(saltRelayPin, HIGH);
    userInput = 0;
  }
  else if (userInput == 18)
  {
    digitalWrite(saltRelayPin, LOW);
    userInput = 0;
  }
  else if (userInput == 19)
  {
    addSaltWater();
    userInput = 0;
  }
  else if (userInput == 20)
  {
    dispenseShrimp(500);
    userInput = 0;
  }
}

void water(int pin, int milis)
{
  digitalWrite(pin, HIGH);
  delay(milis);
  digitalWrite(pin, LOW);
}

void harvestShrimp()
{
  Serial.println("Harvesting!");
  armBaseServo.write(0);
  delay(1000);
  harvest(2000);
  water(rinseRelayPin, 1000);
  armBaseServo.write(180);
  delay(2000);
  forearmServo.write(70);
  delay(3000);
  forearmServo.write(0);
  delay(2000);
  armBaseServo.write(0);
  delay(1000);
  
}

void harvestShrimpAttempt1()
{
  activateServos();
  delay(300);
  Serial.println("Harvesting!");
  armBaseServo.write(180);
  delay(1000); //move arm to position and wait
  //harvest(1500); //harvest for 1500 milliseconds
  getShrimp();
  getShrimp();
  getShrimp();
  
  water(rinseRelayPin, 1000); //rinse for one second
  armBaseServo.write(0);
  delay(2000); //move arm
  forearmServo.write(90);
  delay(3000);
  forearmServo.write(0); //dunk arm in tank for 3 seconds
  delay(2000);
  armBaseServo.write(180); //move arm back to position
  delay(1000);
  deactivateServos();
  
  
}

void harvest(int milis)
{
  releaseServo.write(160);
  delay(milis);
  releaseServo.write(15);
  delay(1000);
}

void addShrimp(int milis)
{
  shrimpServo.write(120);
  delay(milis);
  shrimpServo.write(0);
  delay(1000);
}

void activateServos()
{
  shrimpServo.write(0);
  releaseServo.write(15);
  armBaseServo.write(0);
  forearmServo.write(0);
  digitalWrite(servoRelayPin, HIGH);
  shrimpServo.attach(shrimpServoPin);
  releaseServo.attach(releaseServoPin);
  armBaseServo.attach(armBaseServoPin);
  forearmServo.attach(forearmServoPin);
}

void deactivateServos()
{
  digitalWrite(servoRelayPin, LOW);
  shrimpServo.detach();
  releaseServo.detach();
  armBaseServo.detach();
  forearmServo.detach();
}

void addSaltWater()
{
  digitalWrite(saltRelayPin, HIGH);
  delay(16000);
  digitalWrite(saltRelayPin, LOW);
}

void rinseBottle()
{
  digitalWrite(saltRelayPin, HIGH);
  delay(16000);
  digitalWrite(saltRelayPin, LOW);
}

void getShrimp()
{
  releaseServo.write(160); //valve opened
  delay(1000);
  releaseServo.write(15); //valve closed
  delay(1000);
}

void dispenseShrimp(int time)
{
  shrimpServo.write(140); //open valve
  delay(time);
  shrimpServo.write(0); //close valve
}
