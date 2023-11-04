/**********************************************************************************/
/*                                                                                */
/* Auteurs: Abdul Hamid Diop, Gio Sabbagh, Imed-Eddine Bennour, Maxence Lhuisset  */
/*                                                                                */
/* Ce fichier fait la definition de la fonction de debug selon si elle a été      */
/* définie ou non par le makefile                                                 */
/*                                                                                */
/**********************************************************************************/
#include <string.h>
#ifdef DEBUG
#define DEBUG_PRINT(x) printuart(x)
#else 
#define DEBUG_PRINT(x) do {} while(0)
#endif

void printuart(const char* message);