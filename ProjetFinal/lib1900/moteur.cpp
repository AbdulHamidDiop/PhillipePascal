/*source: tp4 pb3
PD4 -> enable left wheel
PD5 -> enable right wheel
PD6 -> Direction left wheel
PD7 -> Direction right wheel
*/
#include "moteur.hpp"

  Motor::Motor(){ 
    DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
  }
  void Motor::adjustMotor(uint8_t rightWheelSpeed, uint8_t leftWheelSpeed, bool rightForward = true, bool leftForward = true){

    if (rightForward)
        PORTD &= ~(1 << PORTD7);
    else
        PORTD |= (1 << PORTD7);

    if (leftForward)
        PORTD &= ~(1 << PORTD6);
    else
        PORTD |= (1 << PORTD6);

    cli();
    TCNT1 = 0;
    OCR1A = rightWheelSpeed;
    OCR1B = leftWheelSpeed;

    TCCR1A = (1 << COM1A0) | (1 << COM1A1)| (1 << COM1B1) |  (1 << COM1B0) | (1 << WGM10);
    TCCR1B = (1 << CS11);
    TCCR1C = 0;  
    sei();      
}

void Motor::backward(){
  adjustMotor(motorSpeed, motorSpeed, false, false);
}

void Motor::forward(uint8_t speed1, uint8_t speed2){
  adjustMotor(speed1, speed2, true, true);
}

void Motor::stop(){
    cli();
    TCCR1A &= ~(1 << COM1A0) & ~(1 << COM1A1) & ~(1 << COM1B1) &  ~(1 << COM1B0) & ~(1 << WGM10);
    TCCR1B &= ~(1 << CS11);
    sei();

}  

void Motor::turnRight(){
  adjustMotor(motorSpeed, motorSpeed, false, true);
  while(static_cast<bool>(turnDelay--)){ //////////////////////////////Faire une fonction myDelayMS
    _delay_ms(25);
  }
  stop();
}

void Motor::turnLeft(){
  adjustMotor(motorSpeed, motorSpeed, true, false);
  while(static_cast<bool>(turnDelay--)){ //////////////////////////////Faire une fonction myDelayMS
    _delay_ms(25);
  }
  stop();
}

void Motor::setSpeed(uint8_t newSpeed){
  motorSpeed = newSpeed;
}


Motor::~Motor(){}
