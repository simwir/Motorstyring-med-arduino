int stateAIn = 0; //Analog Read pin for the stateswitch
int potAIn = 1; //Analog Read pin for the potentiometer
int directionDOut = 12; //Digital write pin for the motor direction
int speedPwmOut = 11; //Digital PWM write pin for the speed of the motor
int debugPin = 13;

double waitCurve = -16000/1023; //The gradient for the wait curve. -16000/1023 = between 20 and 4 sek
double maxWait = 20000; //The maximum wait in millis
unsigned long waitBegin; //The begin time of the wait in millis

void setup() {
  //Setting the pinmodes for the pins
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
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
      fastMove();
      break;
  }
}
/**
 * Returns the state of the state switch as a int between 1-4.
 * instead of between 1-1023.
 */
int getSate(){
  int state = analogRead(stateAIn);
  if(state>884){
    beginWait();
    return 1;
  }else if(state<=884&&state>628){
    return 2;
  }else if(state<=628&&state>383){
    return 3;
  }else if(state<=383){
    return 3;
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
  if(millis()>waitCurve*analogRead(1)+maxWait){
    slowMove();
    beginWait();
  }
}

/**
 * Moves the wiper slowly up and back a single time
 */
void slowMove(){
  
}

/**
 * Moves the wiper fast up and back a single time
 */
void fastMove(){
  
}
