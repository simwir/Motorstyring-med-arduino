int stateAIn = 0; //Analog Read pin for the stateswitch
int potAIn = 1; //Analog Read pin for the potentiometer
int directionDOut = 12; //Digital write pin for the motor direction
int speedPwmOut = 11; //Digital PWM write pin for the speed of the motor
int debugPin = 13;

int slowRight = 2;
int slowLeft = 3;
int fastRight = 4;
int fastLeft = 5;

double waitCurve = -16000/1023; //The gradient for the wait curve. -16000/1023 = between 20 and 4 sek
double maxWait = 20000; //The maximum wait in millis
unsigned long waitBegin; //The begin time of the wait in millis

void setup() {
  //Setting the pinmodes for the pins
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  //A switch depending on the state of the state switch
  switch(getState()){
    case 1:
      //Does nothing since this means off
      break;
    case 2:
      modeWait();
      break;
    case 3:
      slowMove();
      break;
    case 4:
      Serial.println("Calling fast move");
      fastMove();
      break;
  }
}
/**
 * Returns the state of the state switch as a int between 1-4.
 * instead of between 1-1023.
 */
int getState(){
  int state = analogRead(stateAIn);
  Serial.println(state);
  if(state>884){
    Serial.println("1");
    return 1;
  }else if(state<=884&&state>628){
    //beginWait();
    Serial.println("2");
    return 2;
  }else if(state<=628&&state>383){
    Serial.println("3");
    return 3;
  }else if(state<=383){
    Serial.println("4");
    return 4;
  }else{
    //Should never happen, if it does something went very wrong.
    digitalWrite(debugPin, HIGH);
    return 1;
  }
}

/**
 * Sets the wait timer to the current time
 */
void beginWait(){
  waitBegin = millis();
}

/**
 * Checks if the current tima has exceeded the time to wait.
 * If that is true then is calls a slowMove() and beginWait()
 */
void modeWait(){
  Serial.println("modeWait()");
  Serial.print("wait ");
  Serial.println(waitCurve*analogRead(potAIn)+maxWait+waitBegin);
  Serial.print("millis ");
  Serial.println(millis());
  if(millis()>waitCurve*analogRead(potAIn)+maxWait+waitBegin){
    Serial.println("Millis>");
    slowMove();
    beginWait();
    //waitBegin = millis();
  }
  delay(300);
}

/**
 * Moves the wiper slowly up and back a single time
 */
void slowMove(){
  Serial.println("slowMove()");
  digitalWrite(slowRight, HIGH);
  delay(1000);
  digitalWrite(slowRight, LOW);
  digitalWrite(slowLeft, HIGH);
  delay(1000);
  digitalWrite(slowLeft, LOW);
}

/**
 * Moves the wiper fast up and back a single time
 */
void fastMove(){
  Serial.println("fastMove()");
  digitalWrite(fastRight, HIGH);
  delay(200);
  digitalWrite(fastRight, LOW);
  digitalWrite(fastLeft, HIGH);
  delay(200);
  digitalWrite(fastLeft, LOW);
}
