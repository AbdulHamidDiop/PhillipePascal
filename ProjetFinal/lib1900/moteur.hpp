/**********************************************************************************/
/*                                                                                */
/* Auteurs: Abdul Hamid Diop, Gio Sabbagh, Imed-Eddine Bennour, Maxence Lhuisset  */
/*                                                                                */
/* Ce code est la définition de la classe moteur et ses methodes d'ajustement     */
/* et d'arrêt sont dans le fichier moteur.cpp                                     */
/*                                                                                */
/**********************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif 
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

class Motor {
public:
  Motor(); 
  void adjustMotor(uint8_t rightWheelSpeed, uint8_t leftWheelSpeed, bool rightForward , bool leftForward);
  void turnRight();
  void turnLeft();
  void backward();
  void forward(uint8_t speed1, uint8_t speed2);
  void stop();
  void setSpeed(uint8_t newSpeed);
  ~Motor();

private:
  uint16_t turnDelay = 20;
  uint8_t motorSpeed = 0;

};
