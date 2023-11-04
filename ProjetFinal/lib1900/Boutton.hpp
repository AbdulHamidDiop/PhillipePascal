/*************************************************************************************/
/*                                                                                   */
/* Auteurs: Abdul Hamid Diop, Gio Sabbagh, Imed-Eddine Bennour, Maxence Lhuisset     */
/*                                                                                   */
/* Ce code est la définition de la classe Button et ses attributs. les methodes      */
/* de configuration du bouton pour les interuptions sont dans le fichier button.cpp  */
/*                                                                                   */
/*************************************************************************************/

#define F_CPU 8000000 
#include <avr/io.h>
#include <util/delay.h> 
#include <avr/interrupt.h>

class Button {
public:
  Button(); 
  void usefallingEdge();
  void useAnyEdge();
  void useRisingEdge();
  ~Button();
};