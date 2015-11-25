int const stateAIn = 0; //Analog Read pin for the stateswitch
int const potAIn = 1; //Analog Read pin for the potentiometer
int const directionDOut = 12; //Digital write pin for the motor direction
int const speedPwmOut = 11; //Digital PWM write pin for the speed of the motor
int const debugPin = 13;

int const slowCycle = 2000; //The duration of the slow cycle
int const fastCycle = 1000; //The duration of the fast cycle

double const waitCurve = -16000/1023; //The gradient for the wait curve. -16000/1023 = between 20 and 4 sek
double const maxWait = 20000; //The maximum wait in millis
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
      //Run the wait funktion for the interval
      modeWait();
      break;
    case 3:
      //Run the slow movement funktion directly
      //this will make it so there is no wait in between
      slowMove();
      break;
    case 4:
      //Runs the fast movement funktion
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
  if(state>884){
    return 1;
  }else if(state<=884&&state>628){
    return 2;
  }else if(state<=628&&state>383){
    return 3;
  }else if(state<=383){
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
  if(millis()>waitCurve*analogRead(1)+maxWait+waitBegin){
    slowMove();
    beginWait();
  }
}

/**
 * Moves the wiper slowly up and back a single time
 */
void slowMove(){
  moveMotor(slowCycle);
}

/**
 * Moves the wiper fast up and back a single time
 */
void fastMove(){
  moveMotor(fastCycle);
}

/**
 * Here the movement it self is handled.
 * It's called by either slowMove or fastMove,
 * and the cycle time is the only difference.
 */
void moveMotor(int cycleTime){
  //Saves the times to get the passed time
  long stTime = millis();

  //We need to keep trak of if we have been high or low,
  //on the sinus curve
  bool hasBeenHigh = false;
  bool hasBeenLow = false;


  //While loob that runs as long as the wait time has not passed
  while(millis()-stTime<cycleTime){
    
    //updating varible of the time since the start time
    long timeSinceStart = millis()-stTime;
    

    //Calculating the sinus curve
    double rad = ((2*PI)/cycleTime)*timeSinceStart;
    double sinus = sin(rad);
    int mSpeed = 255*sinus;
    mSpeed = constrain(mSpeed, -255,255);

    //depending on the mSpeed do different things
    //if we are below 21 we run backwards by setting the relay to high
    if(mSpeed<-21){
      //We write the absolute of the speed to remove the -
      analogWrite(speedPwmOut, abs(mSpeed));
      //We set hasBeenLow to true so the change period handles corectly
      hasBeenLow=true;
    //If the speed is 0 or close to 0 change the direction of the relay.
    //The interval is determened by testing where the motor can't move
    }else if(mSpeed>-21&&mSpeed<21){
      //Stop the motor
      digitalWrite(speedPwmOut, LOW);
      //Determen where we are in the sinus curve and handle acordingly
      if(hasBeenLow){
        digitalWrite(directionDOut, LOW);
      }else if(hasBeenHigh){
        digitalWrite(directionDOut, HIGH);
      }else{
        digitalWrite(directionDOut, LOW);
      }
    }else{
      hasBeenHigh=true;
      analogWrite(speedPwmOut, mSpeed);
    }
  }
  //Make sure the motor is not running when we are done
  digitalWrite(speedPwmOut, LOW);
}
