#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crypt.h"
#include "encrypt.h"
#include <math.h>

/**
 *  * chiffrement utilisant le ou exclusif
 *   */
void xor_crypt(char * key, char * texte, char* chiffre, int* taille)
{
	int i=0;
	char *clef;
	char c;
	clef = key;

	while (texte[i] != '\0'){
		c= texte[i];
		if(*clef == '\0') clef=key;
		c^=*(clef++);
		chiffre[i] = c;
		i++;
	}
	*(chiffre+i) = '\0';
	*taille = i;

}


/**
 *  * dÈchiffrement utilisant le ou exclusif
 *   */
void xor_decrypt(char * key, char * texte, char* chiffre, int taille)
{
	int i=0;
	char *clef;
	char c;
	clef = key;

	while (i<taille){
		c= texte[i];
		if(*clef == '\0') clef=key;
		c^=*(clef++);
		*(chiffre+i) = c;
		i++;
	}
	*(chiffre+i) = '\0';
	
}

/**
 *  * chiffrement utilisant cesar
 *   */
void cesar_crypt(int decallage, char * texte, char* chiffre)
{
	int i=0;
	char c;
	while (texte[i] != '\0'){
		c = texte[i];
		if(c>='A' && c<='Z')
			chiffre[i] = ((c + decallage)-'A')%26 +'A';
		if(c>='a' && c<='z')
			chiffre[i] = ((c + decallage)-'a')%26+'a';
		if(c>='0' && c<='9')
			chiffre[i] = ((c + decallage)-'0')%10+'0';
		i++;
	}
	*(chiffre+i) = '\0';
}

/**
 *  * dÈchiffrement utilisant  cesar
 *   */
void cesar_decrypt(int decallage, char * texte, char* chiffre)
{
	int i=0;
	char c;
	while (texte[i] != '\0'){
		c = texte[i];
		if(c>='A' && c<='Z')
			chiffre[i] = ((c - decallage)-'A'+26)%26+'A';
		if(c>='a' && c<='z')
			chiffre[i] = ((c - decallage)-'a'+26)%26+'a';
		if(c>='0' && c<='9')
			chiffre[i] = ((c - decallage)-'0'+10)%10+'0';
		i++;
	}
	*(chiffre+i) = '\0';

}

/**
 *  * chiffrement utilisant viginere
 *   */
void viginere_crypt(char * key, char * texte, char* chiffre)
{
	int i=0;
	char c;
	int decallage=1;
	int taille = strlen(key);
	int clef;
	while (texte[i] != '\0'){
		c = texte[i];
		clef = key[i%taille]; 
		if( clef>='A' && clef<='Z')
			decallage = clef - 'A';
		if( clef>='a' && clef<='z')
			decallage = clef - 'a';
		if( clef>='0' && clef<='1')
			decallage = clef - '0';
		if(c>='A' && c<='Z')
			chiffre[i] = ((c + decallage)-'A')%26 +'A';
		if(c>='a' && c<='z')
			chiffre[i] = ((c + decallage)-'a')%26+'a';
		if(c>='0' && c<='9')
			chiffre[i] = ((c + decallage)-'0')%10+'0';
		i++;
	}
	*(chiffre+i) = '\0';

}

/**
 *  * dÈchiffrement utilisant viginere
 *   */
void viginere_decrypt(char * key, char * texte, char* chiffre)
{
		int i=0;
	char c;
	int decallage;
	while (texte[i] != '\0'){
		c = texte[i];
		if( key[i%strlen(key)]>='A' && key[i%strlen(key)]<='Z')
			decallage = key[i%strlen(key)] - 'A';
		if( key[i%strlen(key)]>='a' && key[i%strlen(key)]<='z')
			decallage = key[i%strlen(key)] - 'a';
		if( key[i%strlen(key)]>='0' && key[i%strlen(key)]<='1')
			decallage = key[i%strlen(key)] - '0';
		if(c>='A' && c<='Z')
			chiffre[i] = ((c - decallage)-'A')%26 +'A';
		if(c>='a' && c<='z')
			chiffre[i] = ((c - decallage)-'a')%26+'a';
		if(c>='0' && c<='9')
			chiffre[i] = ((c - decallage)-'0')%10+'0';
		i++;
	}
	*(chiffre+i) = '\0';

}

/**
 *  * chiffrement utilisant des
 *   */
void des_crypt(char * key, char * texte, char* chiffre, int size)
{
	int i,j;
	int bol=0;
	unsigned char blocT[8];
	unsigned char blocR[8];
	

	for(i=0;i<size*8;i+=8){
		// initialisation du bloc de 8octets (64 bits)

		for (j= 0; j < 8; j++)
		{
			
			if(texte[i+j]=='\0' || bol){
				blocT[j] = 0;
				bol = 1;
				continue;
			}
			
			blocT[j] = texte[i+j];
		}
		

		des_encipher(blocT, blocR, key); // chiffrage du bloc
		
		for (j= 0; j < 8; j++)
		{
			chiffre[i+j] = blocR[j];
		}
	}
}


/**
 *  * dÈchiffrement utilisant des
 *   */
void des_decrypt(char * key, char * texte, char* chiffre, int size)
{
	int i,j;
	int bol=0;
	unsigned char blocT[8];
	unsigned char blocR[8];

	for(i=0;i<size*8;i+=8){

		// initialisation du bloc de 8 octets (64 bits)
		for (j= 0; j < 8; j++)
		{
			blocT[j] = texte[i+j];
		}
		bol = 0;

		des_decipher(blocT, blocR, key); // chiffrage du bloc
		

		// on met le bloc chiffrÈ dans le rÈsultat
		for (j= 0; j < 8; j++)
		{
			chiffre[i+j] = blocR[j];
		}
	}

}

void tripledes_crypt(char * key1, char * key2, char * texte, char* chiffre,int size)
{
	
	int i,j;
	int bol = 0;
	unsigned char blocT[8];
	unsigned char blocR[8];
	unsigned char blocA[8];
	unsigned char blocB[8];

	for(i=0;i<size*8;i+=8){
		// initialisation du bloc de 8octets (64 bits)
		for (j= 0; j < 8; j++)
		{
			
			if(texte[i+j]=='\0' || bol){
				blocT[j] = 0;
				bol = 1;
				continue;
			}
			
			blocT[j] = texte[i+j];
		}

		des_encipher(blocT, blocB, key1); // chiffrage du bloc
		des_decipher(blocB, blocA, key2); 
		des_encipher(blocA, blocR, key1); 
		// on met le bloc chiffrÈ dans le rÈsultat
		for (j= 0; j < 8; j++)
		{
			chiffre[i+j] = blocR[j];
		}
	}
	

}

void tripledes_decrypt(char* key1, char* key2, char* texte, char* chiffre, int size)
{
	
	int i,j;
	unsigned char blocT[8];
	unsigned char blocR[8];
	unsigned char blocA[8];
	unsigned char blocB[8];

	for(i=0;i<size*8;i+=8){
		// initialisation du bloc de 8octets (64 bits)
		for (j= 0; j < 8; j++)
		{
			
			blocT[j] = texte[i+j];
		}

		des_decipher(blocT, blocB, key1); // chiffrage du bloc
		des_encipher(blocB, blocA, key2); 
		des_decipher(blocA, blocR, key1); 

		// on met le bloc chiffrÈ dans le rÈsultat
		for (j= 0; j < 8; j++)
		{
			chiffre[i+j] = blocR[j];
		}
	}
}



/****************************************************************
 *                                                               *
 *  -------------------------- modexp -------------------------  *
 *                                                               *
 ****************************************************************/

static Huge modexp(Huge a, Huge b, Huge n) {
	
	Huge               y;
	
	/****************************************************************
	 *                                                               *
	 *  Calcule (pow(a, b) % n) avec la mÈthode du carrÈ binaire     *
	 *  et de la multiplication.                                     *
	 *                                                               *
	 ****************************************************************/
	
	y = 1;
	
	while (b != 0) {
		
		/*************************************************************
		 *                                                            *
		 *  Pour chaque 1 de b, on accumule dans y.                   *
		 *                                                            *
		 *************************************************************/
		
		if (b & 1)
			y = (y * a) % n;
		
		/*************************************************************
		 *                                                            *
		 *  …lÈvation de a au carrÈ pour chaque bit de b.             *
		 *                                                            *
		 *************************************************************/
		
		a = (a * a) % n;
		
		/*************************************************************
		 *                                                            *
		 *  On se prÈpare pour le prochain bit de b.                  *
		 *                                                            *
		 *************************************************************/
		
		b = b >> 1;
		
	}
	
	return y;
	
}


/**
 * Transforme une chaine de caractère en chaine d'entier
 */
void texttoint(char * texte, char* chiffre, int size){
	*chiffre='\0';
	int tmp;
	int i;
	for(i=0;i<size;i++){		
	    // on ajoute 10 pour éviter le problème de disparition du 0 devnt les entiers entre 1 et 9 (01 a 09)
		// ceci évite de découper le texte en bloc de taille < n et de les normaliser ensuite
		tmp=(*(texte+i)-'a'+10);
		sprintf(chiffre+strlen(chiffre),"%d%c",tmp,'\0');
	}
}

/**
 * Transforme une chaine d'entier en chaine de caractère
 */ 
void inttotext(char * texte, char* chiffre){
	*chiffre='\0';
	int tmp=0;
	while((*texte) != '\0'){	
	    // lettre de l'alphabet (0..25 correspond pour nous à 10..35)	
		if(10*tmp+(*(texte)-'0') > 36){
		    // on déduit donc 10 pour obtenir la bonne lettre dans l'alphabet
			sprintf(chiffre+strlen(chiffre),"%c%c",tmp+'a'-10, '\0');
			tmp=0;
		}
		tmp=10*tmp+(*(texte)-'0');
		texte++;
	}
}

/**
 * Chiffrement RSA
 */
void rsa_crypt(int e, int n, char * texte, char* chiffre, int size)
{
    int tmp;
	Huge buf=0;
	char* pt;
	char* btmp = (char *)malloc(strlen(texte) * sizeof(char)); 
	
	texttoint(texte,btmp,size);
	pt = btmp;
	*chiffre='\0';
	while((*pt) != '\0'){
		tmp=*pt-'0';
		if(10*buf + tmp >= n){
		    // on utilise le $ comme séparateur de bloc
			sprintf(chiffre+strlen(chiffre),"%ld$%c",modexp(buf,e,n)/* TODO Chiffrement de buf */,'\0');
			buf=0;
		}
		buf=10*buf+tmp;
		pt++;
	}
	sprintf(chiffre+strlen(chiffre),"%ld$%c", modexp(buf,e,n)/* TODO Chiffrement de Buf */,'\0');
	printf("\n");
}

/**
 * Déchiffrement RSA
 */
void rsa_decrypt(int d, int n, char * texte, char* chiffre)
{
	int tmp;
	char* pt=texte;
	char* tmpc= (char *)malloc(strlen(texte) * sizeof(char)); 
	Huge buf=0;
	
	*tmpc='\0';
	while((*pt) != '\0'){
		// on utilise le $ comme séparateur de bloc
	    if((*pt) == '$'){
			sprintf(tmpc+strlen(tmpc),"%ld%c", modexp(buf,d,n)/* Dechiffrement de buf */,'\0');
			buf=0;
		}else{
			tmp=*pt-'0';
			buf=10*buf+tmp;
		}
		pt++;
	}
	sprintf(tmpc+strlen(tmpc),"%ld%c",modexp(buf,d,n)/* Dechiffrement de buf*/,'\0');
	
	inttotext(tmpc,chiffre);
}


