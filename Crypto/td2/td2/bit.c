/****************************************************************
*                                                               *
*  -------------------------- bit.c --------------------------  *
*                                                               *
****************************************************************/

#include <string.h>

#include "bit.h"

/****************************************************************
*                                                               *
*  ------------------------- bit_get -------------------------  *
*                                                               *
****************************************************************/

int bit_get(const unsigned char *bits, int pos) {

unsigned char      masque;

int                i;

/****************************************************************
*                                                               *
*  Configure un masque pour le bit à récupérer.                 *
*                                                               *
****************************************************************/

masque = 0x80;//hexadécimal, en binaire -> 1000 0000 (8 -> 1000, 0 -> 0000)

for (i = 0; i < (pos % 8); i++)
   masque = masque >> 1;// on décale le masque de pos pour que le bit "1" soit à la position pos

/****************************************************************
*                                                               *
*  Renvoie le bit.                                              *
*                                                               *
****************************************************************/

return (((masque & bits[(int)(pos / 8)]/*pour prendre le bon char dans le tableau*/) == masque) ? 1 : 0);// si le masque est egale au & entre le char et lui cela veut dire que le bit en position pos est 1 sinon c'est 0

}

/****************************************************************
*                                                               *
*  -------------------------- bit_set ------------------------  *
*                                                               *
****************************************************************/

void bit_set(unsigned char *bits, int pos, int etat) {

unsigned char      masque;

int                i;

/****************************************************************
*                                                               *
*  Configure un masque pour le bit à positionner.               *
*                                                               *
****************************************************************/

masque = 0x80;

for (i = 0; i < (pos % 8); i++)
   masque = masque >> 1;//on met le 1 à la position qu'on veut

/****************************************************************
*                                                               *
*  Positionne le bit.                                           *
*                                                               *
****************************************************************/

if (etat)//si notre etat est 1
   bits[pos / 8] = bits[pos / 8] | masque;//on fait un ou avec le masque et ça met un 1 à la position dans la bonne case du tableau de char
else
   bits[pos / 8] = bits[pos / 8] & (~masque);//l'inverse pour mettre un 0 on fait un "& !masque", on veut que là ou y a le 1 dans le masque y ai 0 dans notre texte c'est pour ça qu'on fait non puis et.

return;

}

/****************************************************************
*                                                               *
*  ------------------------- bit_xor -------------------------  *
*                                                               *
****************************************************************/

void bit_xor(const unsigned char *bits1,
             const unsigned char *bits2, 
             unsigned char *bitsx, int taille) {

int                i;

/****************************************************************
*                                                               *
*  Effectue le XOR (OU exclusif) bit à bit des deux tampons.    *
*                                                               *
****************************************************************/

for (i = 0; i < taille; i++) {

   if (bit_get(bits1, i) != bit_get(bits2, i))
      bit_set(bitsx, i, 1);
   else
      bit_set(bitsx, i, 0);

}

return;

}

/****************************************************************
*                                                               *
*  ---------------------- bit_rot_left -----------------------  *
*                                                               *
****************************************************************/

void bit_rot_left(unsigned char *bits, int taille, int nbre) {

int                fbit,
                   lbit,
                   i,
                   j;

/****************************************************************
*                                                               *
*  Rotation du tampon du nbre de fois spécifié vers la gauche.  *
*                                                               *
****************************************************************/

if (taille > 0) {

   for (j = 0; j < nbre; j++) {

      for (i = 0; i <= ((taille - 1) / 8); i++) {

         /*******************************************************
         *                                                      *
         *  Récupère le bit à sortir de l'octet courant.        *
         *                                                      *
         *******************************************************/

         lbit = bit_get(&bits[i], 0);

         if (i == 0) {

            /****************************************************
            *                                                   *
            *  Sauve le bit sorti du premier octet.             *
            *                                                   *
            ****************************************************/

            fbit = lbit;

            }

         else {

            /****************************************************
            *                                                   *
            *  Positionne le bit le plus à droite de l'octet    *
            *  précédent avec le bit le plus à gauche,          *
            *  qui sortira de l'octet courant.                  *
            *                                                   *
            ****************************************************/

            bit_set(&bits[i - 1], 7, lbit);

         }

         /*******************************************************
         *                                                      *
         *  Décale l'octet courant vers la gauche.              *
         *                                                      *
         *******************************************************/

         bits[i] = bits[i] << 1;

      }

      /**********************************************************
      *                                                         *
      *  Positionne le bit le plus à droite du tampon           *
      *   avec le bit sorti du premier octet.                   *
      *                                                         *
      **********************************************************/

      bit_set(bits, taille - 1, fbit);

   }

}

return;

}
