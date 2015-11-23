int stateAIn = 0; //Analog Read pin for the stateswitch
int potAIn = 1; //Analog Read pin for the potentiometer
int directionDOut = 12; //Digital write pin for the motor direction
int speedPwmOut = 11; //Digital PWM write pin for the speed of the motor
int debugPin = 13;

int slowSpeed = 128;
int fastSpeed = 255;
int waitTime = 250;
int slowCycle = 2000;
int fastCycle = 1000;

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
    //Serial.println("case2");
      modeWait();
      break;
    case 3:
      //Serial.println("case3");
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


void moveMotor(int cycleTime){
  //Saves the times to get the passed time
  long stTime = millis();

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
    if(mSpeed<-21){
      analogWrite(speedPwmOut, mSpeed+mSpeed^2);
      //Serial.println("<0 HIGH");
      Serial.println(mSpeed);
      hasBeenLow=true;
    }else if(mSpeed>-21&&mSpeed<21){
      digitalWrite(speedPwmOut, LOW);
      if(hasBeenLow){
        digitalWrite(directionDOut, LOW);
        //Serial.println("nulpunkt low");
      }else if(hasBeenHigh){
        digitalWrite(directionDOut, HIGH);
        //Serial.println("nulpunkt high");
      }else{
        digitalWrite(directionDOut, LOW);
        //Serial.println("nulpunkt low");
      }
      Serial.println("0");
    }else{
      digitalWrite(directionDOut, LOW);
      //Serial.println(">0 LOW");
      hasBeenHigh=true;
      analogWrite(speedPwmOut, mSpeed);
      Serial.println(mSpeed);
    }
  }
  digitalWrite(speedPwmOut, LOW);
}
