int stateAIn = 0; //Analog Read pin for the stateswitch
int potAIn = 1; //Analog Read pin for the potentiometer
int directionDOut = 12; //Digital write pin for the motor direction
int speedPwmOut = 11; //Digital PWM write pin for the speed of the motor
int debugPin = 13;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);

}

void loop() {
  //A switch depending on the state of the state switch
  switch(getState()){
    case 1:
      digitalWrite(speedPwmOut, LOW);
      break;
    case 2:
      Left();
      break;
    case 3:
      digitalWrite(speedPwmOut, LOW);
      break;
    case 4:
      Right();
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

void Left(){
  moveRL(false);
}

void Right(){
  moveRL(true);
}

void moveRL(boolean right){
  if(right){
    digitalWrite(directionDOut, HIGH);
  }else{
    digitalWrite(directionDOut, LOW);
  }
  analogWrite(speedPwmOut, analogRead(potAIn)/4);
}
