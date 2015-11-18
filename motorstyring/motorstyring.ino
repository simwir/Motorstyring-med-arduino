int stateAIn = 0; //Analog Read pin for the stateswitch
int potAIn = 1; //Analog Read pin for the potentiometer
int directionDOut = 12; //Digital write pin for the motor direction
int speedPwmOut = 13; //Digital PWM write pin for the speed of the motor
int debugPin = 13;

int slowSpeed = 128;
int fastSpeed = 255;
int waitTime = 250;

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
    Serial.println("case2");
      modeWait();
      break;
    case 3:
      Serial.println("case3");
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
  Serial.println("slowMove");
  long stTime = millis();
  Serial.println(stTime);
  long timeSinceStart = millis()-stTime;
  Serial.println(timeSinceStart);
  while(millis()-stTime<2000){
    double rad = ((2*PI)/2000)*timeSinceStart;
    Serial.println(rad);
    double sinus = sin(rad);
    Serial.println(timeSinceStart);
    int mSpeed = map(sinus,-1,1,-255,255);
    Serial.println(mSpeed);
    mSpeed = constrain(mSpeed, -255,255);
    if(mSpeed<0){
      digitalWrite(directionDOut, LOW);
      analogWrite(speedPwmOut, mSpeed+mSpeed^2);
    }else if(mSpeed==0){
      digitalWrite(speedPwmOut, LOW);
      delay(50);
    }else{
      digitalWrite(directionDOut, HIGH);
      analogWrite(speedPwmOut, mSpeed);
    }
  }

  
  /*
  digitalWrite(directionDOut, HIGH);
  analogWrite(speedPwmOut, slowSpeed);
  delay(1000);
  digitalWrite(speedPwmOut, LOW);
  digitalWrite(directionDOut, LOW);
  delay(waitTime);
  analogWrite(speedPwmOut, slowSpeed);
  delay(1000);
  digitalWrite(speedPwmOut, LOW);
  digitalWrite(directionDOut, HIGH);
  delay(waitTime);*/
}

/**
 * Moves the wiper fast up and back a single time
 */
void fastMove(){
  digitalWrite(directionDOut, HIGH);
  analogWrite(speedPwmOut, fastSpeed);
  delay(500);
  digitalWrite(speedPwmOut, LOW);
  digitalWrite(directionDOut, LOW);
  delay(waitTime);
  analogWrite(speedPwmOut, fastSpeed);
  delay(500);
  digitalWrite(speedPwmOut, LOW);
  digitalWrite(directionDOut, HIGH);
  delay(waitTime);
}
