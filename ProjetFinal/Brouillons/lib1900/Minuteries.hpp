/**********************************************************************************/
/*                                                                                */
/* Auteurs: Abdul Hamid Diop, Gio Sabbagh, Imed-Eddine Bennour, Maxence Lhuisset  */
/*                                                                                */
/* Ce code est la définition des classes Timer0/1/2. les methodes d'allumage et   */
/* et d'arrêt des timer sont dans le fichier Minuteries.cpp                       */
/*                                                                                */
/**********************************************************************************/

#define F_CPU 8000000 
#include <avr/io.h> 
#include <avr/interrupt.h>
class Timer0 {
public:
  Timer0(); 
  void startTimer(uint8_t duration);
  void stopTimer();
  ~Timer0();
};

class Timer1 {
public:
  Timer1(); 
  void startTimer(uint16_t duration);
  void stopTimer();
  ~Timer1();
};

class Timer2 {
public:
  Timer2(); 
  void startTimer(uint8_t duration);
  void stopTimer();
  ~Timer2();
};