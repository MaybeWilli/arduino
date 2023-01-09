#include <Servo.h>
#include <Stepper.h>

int valvePin1 = 17;
int trashPin = 2;
int valvePin2 = 12;
Servo s1;
Servo t1;
Servo airServo;
Servo loadServo;
Servo armBase;
Servo s2;
Servo feedServo;
Servo shrimpServo;
Servo foreArm;
Servo switchServo;
int switchPin = 16; //controls the switching of the water pipes;
int forePin = 14; //upper half of arm
int shrimpPin = 15; //drop unhatched shrimp pin
int feedPin = 6; //feed into the arm pin
int basePin = 5; //base of arm
int loadPin = 3; //gut loading valve
int airServoPin = 4; //gut loading air valve
int userInput = 0;
int servoRelay = 10; //turns on/off servos
int airRelay1 = 8; //air pump #1
int airRelay2 = 11; //air pump #2
int saltRelay = 7; //salt water
int waterRelay = 9; //fresh water
int switchAngle = 90;

//Stepper pump = Stepper(1048, 3, 4, 5, 6);

void setup() {
  // put your setup code here, to run once:
  pinMode(servoRelay, OUTPUT);
  pinMode(airRelay1, OUTPUT);
  pinMode(airRelay2, OUTPUT);
  pinMode(saltRelay, OUTPUT);
  pinMode(waterRelay, OUTPUT);
  digitalWrite(servoRelay, LOW);
  delay(300);
  digitalWrite(airRelay1, LOW);
  delay(300);
  digitalWrite(airRelay2, LOW);
  delay(300);
  digitalWrite(saltRelay, LOW);
  delay(300);
  digitalWrite(waterRelay, LOW);
  /*s1.attach(valvePin1);
  s1.write(15);
  delay(200);
  airServo.write(90);
  airServo.attach(airServoPin);
  delay(200);
  loadServo.write(15);
  loadServo.attach(loadPin);
  delay(200);
  armBase.write(120);
  armBase.attach(basePin);
  delay(200);
  feedServo.write(15);
  feedServo.attach(feedPin);*/
  //pump.setSpeed(5);
  Serial.begin(9600);
}

void loop() {
  FeedAndRinse();
  delay(24*60*60*1000); //wait a day
  ChangeHatchery1();
  delay(24*60*60*1000); //wait a day
}

  //pump.step(300);

void WriteWater(float angle)
{
  Serial.println("Doin the things!");
  switchServo.write(switchAngle);
  switchServo.attach(switchPin);
  float curr = switchServo.read();
  int frames = 50;
  float adjust = (angle-curr) / (float)frames;
  for (int i = 0; i < frames; i++)
  {
    curr+=adjust;
    switchServo.write(round(curr));
    Serial.print("Writing ");
    Serial.print(curr);
    Serial.print("\n");
    delay(300);
  }
  switchAngle = angle;
  switchServo.detach();
  Serial.print("Last printed: ");
  Serial.println(curr);
  Serial.print("\n");
}

void DailyCycle()
{
  AirPumpActivate(0); //turn off air
  //delay(30000); //wait a while for nauplii to settle
  HarvestInstar2(); //harvest shrimp and feed to tank
  delay(500);
  Serial.println("Fed to tank");
  ServoRelay(true);
  delay(500);
  OpenAndCloseServo(feedServo, feedPin, 180, 15, 15000); //dump garbage in the enriching vessel down the drain
  delay(500);
  //delay(500);
  Serial.println("Dump enriching vessel garbage");
  WriteWater(90);
  delay(500);
  ServoRelay(false);
  delay(500);
  
  AddWater(waterRelay, 15000); //rinse enriching vessel
  //Serial.println("Watering 1!");
  delay(500);
  ServoRelay(true);
  delay(500);
  OpenAndCloseServo(feedServo, feedPin, 180, 15, 15000); //dump garbage in the enriching vessel down the drain
  Serial.println("Dump rinsed enriching vessel");
  delay(500);
  HarvestInstar1(1); //drop instar 1 nauplii into enriching vessel
  Serial.println("Drop instar 1");
  delay(500);
  AddEnrichment(); //load enrichment into enriching vessel
  Serial.println("Add enrichment");
  ServoRelay(false);
  delay(500);

  //add salt water to enriching vessel
  ServoRelay(true);
  delay(500);
  WriteWater(90);
  delay(500);
  ServoRelay(false);
  delay(500);
  //Serial.println("Watering 2!");
  AddWater(saltRelay, 10000);
  Serial.println("Fill enriching vessel");

  //Set water pipe above hatching vessel
  ServoRelay(true);
  delay(500);
  WriteWater(15);
  delay(500);

  //drain and rinse hatching vessel
  OpenAndCloseServo(t1, trashPin, 180, 15, 15000); //dump garbage in the hatching vessel down the drain
  Serial.println("Dump garbage");
  delay(500);
  ServoRelay(false);
  //Serial.println("Watering 3!");
  AddWater(waterRelay, 15000);
  Serial.println("Rinse hatching vessel");
  ServoRelay(true);
  delay(500);
  OpenAndCloseServo(t1, trashPin, 180, 15, 15000); //dump garbage in the hatching vessel down the drain
  Serial.println("Done rinsing hatching vessel");
  delay(500);
  ServoRelay(false);
  //Serial.println("Watering 4!");
  AddWater(saltRelay, 10000);
  Serial.println("Done filling hatching vessel");

  Serial.println("End of cycle function");
  
  /*DepositUnhatchedShrimp();

  //switch air pumps every 15 minutes for the next 24 hours
  for (int i = 0; i < 96; i++)
  {
    if (i % 2 == 0)
    {
      AirPumpActivate(1);
    }
    else
    {
      AirPumpActivate(2);
    }
    delay(900000);
  }*/
  
}

void SetupCycle()
{
  ServoRelay(true);
  delay(500);
  WriteWater(90);
  delay(500);
  ServoRelay(false);
  delay(500);
  AddWater(saltRelay, 5000);
  delay(500);
  ServoRelay(true);
  delay(500);
  WriteWater(15);
  delay(500);
  ServoRelay(false);
  delay(500);
  AddWater(saltRelay, 15000);
  delay(500);
  DepositUnhatchedShrimp();
  delay(500);
  AirPumpActivate(1);
  Serial.println("Setup Done");
}


/*
 * Changes the water in the first hatching vessel,
 * drops the instar 2 nauplii into the enriching
 * vessel, rinses the hatching vessel, adds salt water,
 * adds enrichment to enriching vessel,
 * and adds new eggs
 */
void ChangeHatchery1()
{
  ServoRelay(true);
  delay(500);
  Serial.println("Harvesting instar 1 nauplii");
  HarvestInstar1(1); //drop shrimp into vessel
  delay(500);
  ServoRelay(true);
  delay(500);
  WriteWater(90);
  delay(500);
  ServoRelay(false);
  delay(500);
  AddWater(saltRelay, 17000); //filling enrichment vessel
  delay(500);
  AddEnrichment(); //add enrichment
  delay(500);
  ServoRelay(true);
  delay(500);
  Serial.println("Dumping garbage into drain");
  OpenAndCloseServo(t1, trashPin, 180, 15, 15000); //dump garbage in the hatching vessel down the drain
  delay(500);
  WriteWater(15);
  delay(500);
  ServoRelay(false);
  delay(500);
  AddWater(waterRelay, 15000);
  delay(500);
  ServoRelay(true);
  Serial.println("Dumping garbage into drain");
  OpenAndCloseServo(t1, trashPin, 180, 15, 15000); //dump garbage in the hatching vessel down the drain
  delay(500);
  ServoRelay(false);
  delay(500);
  Serial.println("Filling vessel!");
  AddWater(saltRelay, 10000);
  delay(500);
  Serial.println("Adding new eggs");
  
  //DepositUnhatchedShrimp();
}

/*
 * Empties out enriching vessel, feesd shrimp
 * to the tank, and rinses.
 */
void FeedAndRinse()
{
  HarvestInstar2(); //harvest shrimp and feed to tank
  delay(500);
  Serial.println("Fed to tank");
  ServoRelay(true);
  delay(500);
  OpenAndCloseServo(feedServo, feedPin, 180, 15, 15000); //dump garbage in the enriching vessel down the drain
  delay(500);
  //delay(500);
  Serial.println("Dump enriching vessel garbage");
  WriteWater(90);
  delay(500);
  ServoRelay(false);
  delay(500);
  
  AddWater(waterRelay, 15000); //rinse enriching vessel
  //Serial.println("Watering 1!");
  delay(500);
  ServoRelay(true);
  delay(500);
  OpenAndCloseServo(feedServo, feedPin, 180, 15, 15000); //dump garbage in the enriching vessel down the drain
  Serial.println("Dump rinsed enriching vessel");
  delay(500);
  ServoRelay(false);
}



void AddWater(int pin, int interval)
{
  ServoRelay(false);
  delay(500);
  digitalWrite(pin, HIGH);
  delay(interval);
  digitalWrite(pin, LOW);
}

void WriteAir(int angle)
{
  airServo.attach(airServoPin);
  float curr = airServo.read();
  int frames = 20;
  float adjust = (angle-curr) / (float)frames;
  for (int i = 0; i < frames; i++)
  {
    curr+=adjust;
    airServo.write(round(curr));
    Serial.print("Writing ");
    Serial.print(curr);
    Serial.print("\n");
    delay(500);
  }
  //switchAngle = angle;
  airServo.detach();
  Serial.print("Last printed: ");
  Serial.println(curr);
  Serial.print("\n");
}

void DeactivateServos()
{
  airServo.detach();
  armBase.detach();
  loadServo.detach();
  feedServo.detach();
  s1.detach();
  s2.detach();
  shrimpServo.detach();
  foreArm.detach();
  switchServo.detach();
  t1.detach();
}

void ActivateAllServos()
{
  s1.attach(valvePin1);
    airServo.attach(airServoPin);
    armBase.attach(basePin);
    loadServo.attach(loadPin);
    feedServo.attach(feedPin);
}

void ResetAllServos()
{
  shrimpServo.write(0);
  shrimpServo.attach(shrimpPin);
  delay(1000);
  s1.attach(valvePin1);
  s1.write(15);
  delay(1000);
  s1.detach();
  airServo.attach(airServoPin);
  airServo.write(90);
  delay(1000);
  airServo.detach();
  armBase.attach(basePin);
  armBase.write(120);
  delay(1000);
  armBase.detach();
  loadServo.attach(loadPin);
  loadServo.write(15);
  delay(1000);
  loadServo.detach();
  feedServo.attach(feedPin);
  feedServo.write(15);
  delay(1000);
  feedServo.detach();
  foreArm.attach(forePin);
  foreArm.write(90);
  delay(1000);
  foreArm.detach();
  
}

void SetupServos()
{
  MoveServo(s1, valvePin1, 180);
  MoveServo(t1, trashPin, 180);
  MoveServo(shrimpServo, shrimpPin, 0);
  MoveServo(feedServo, feedPin, 180);
  MoveServo(armBase, basePin, 120);
  MoveServo(foreArm, forePin, 0);
  airServo.write(180);
  WriteAir(180);
  delay(5000);
  MoveServo(loadServo, loadPin, 0);
  WriteWater(90);
  delay(5000);
  Serial.println("Setup done!");
}

void UnSetupServos()
{
  MoveServo(s1, valvePin1, 15);
  MoveServo(t1, trashPin, 15);
  MoveServo(feedServo, feedPin, 15);
  MoveServo(loadServo, loadPin, 150);
  //WriteWater(90);
}

void MoveServo(Servo &s, int pin, int angle)
{
  s.write(angle);
  s.attach(pin);
  delay(2000);
  s.detach();
}

void ServoRelay(bool on)
{
  if (on)
  {
    digitalWrite(servoRelay, HIGH);
  }
  else
  {
    digitalWrite(servoRelay, LOW);
  }
  DeactivateServos();
}

void AddEnrichment()
{
  ServoRelay(true);
  WriteAir(60);
  OpenAndCloseServo(loadServo, loadPin, 0, 150, 1000);
  WriteAir(180);
  ServoRelay(false);
}

void HarvestInstar1(int vessel)
{
  ServoRelay(true);
  if (vessel == 1)
  {
    OpenAndCloseServo(s1, valvePin1, 180, 15, 3000);
  }
  else if (vessel == 2)
  {
    OpenAndCloseServo(s2, valvePin2, 180, 15, 3000);
  }
  ServoRelay(false);
}

void HarvestInstar2() //drops shrimp into net, and feeds the shrimp to the tank
{
  ServoRelay(true);
  foreArm.write(0);
  foreArm.attach(forePin);
  delay(1000);
  foreArm.detach();
  armBase.attach(basePin);
  armBase.write(0);
  delay(2000);
  OpenAndCloseServo(feedServo, feedPin, 180, 15, 3000);
  delay(500);
  armBase.detach();
  delay(500);
  foreArm.write(0);
  foreArm.attach(forePin);
  delay(1000);
  foreArm.detach();
  armBase.attach(basePin);
  armBase.write(120);
  delay(2000);
  armBase.detach();
  foreArm.attach(forePin);
  foreArm.write(0);
  delay(500);
  foreArm.write(90);
  delay(3000);
  foreArm.write(0);
  delay(1000);
  ServoRelay(false);  
}

void DepositUnhatchedShrimp()
{
  ServoRelay(true);
  OpenAndCloseServo(shrimpServo, shrimpPin, 0, 120, 500);
  ServoRelay(false);
}

void OpenAndCloseServo(Servo &s, int pin, int initialPos, int finalPos, int openTime)
{
  s.write(initialPos);
  s.attach(pin);
  delay(500);

  s.write(finalPos);
  delay(openTime);
  s.write(initialPos);
  delay(1000);

  s.detach();
}

void AirValve(bool on)
{
  airServo.attach(airServoPin);
  if (on)
  {
    airServo.write(180);
  }
  else
  {
    airServo.write(90);
  }
  delay(1000);
  airServo.detach();
}

void RunAirPumps(int interval, int switchInterval)
{
  int switchNum = interval / switchInterval;
  for (int i = 0; i < switchNum; i++)
  {
    if (i % 2 == 0)
    {
      AirPumpActivate(1);
    }
    else
    {
      AirPumpActivate(2);
    }
    delay(switchInterval);
  }
  AirPumpActivate(0);
  
}

void AirPumpActivate(int num)
{
  if (num == 0)
  {
    digitalWrite(airRelay1, LOW);
    digitalWrite(airRelay2, LOW);
  }
  else if (num == 1)
  {
    digitalWrite(airRelay1, HIGH);
    digitalWrite(airRelay2, LOW);
  }
  else if (num == 2)
  {
    digitalWrite(airRelay1, LOW);
    digitalWrite(airRelay2, HIGH);
  }
}
