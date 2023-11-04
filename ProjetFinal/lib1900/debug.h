/**********************************************************************************/
/*                                                                                */
/* Auteurs: Abdul Hamid Diop, Gio Sabbagh, Imed-Eddine Bennour, Maxence Lhuisset  */
/*                                                                                */
/* Ce fichier fait la definition de la fonction de debug selon si elle a été      */
/* définie ou non par le makefile                                                 */
/*                                                                                */
/**********************************************************************************/

#ifdef DEBUG
#define DEBUG_PRINT(x) printf(x)
#else 
#define DEBUG_PRINT(x) do {} while(0)
#endif