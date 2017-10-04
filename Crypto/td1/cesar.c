#include<stdio.h>

int main(int argc, char* argv[]){

FILE *fi,*fo;
char *cp;
int c;

if((cp = argv[1])){
        if((fi = fopen(argv[2],"r")) != NULL){
        if((fo = fopen(argv[3],"w")) != NULL){

                while ((c=getc(fi)) != EOF ){
                        //if(*cp == '\0') cp=argv[1];
			if(('A'<= c) && (c <= 'Z')){
			c = c-'A';
			
			c = c + *cp-'A';
			c = 'A'+ (c % 26);
			}
/*
déchiffrement :

 c = c-'A';
 c = c - *cp-'A';
 c = 'A'+ (c % 26);

On fait - la clef au lieu de +
*/
			if(('a'<= c) && (c <= 'z')){
                        c = c-'a';
                        c = c + *cp-'A';
                        c = 'a'+ (c % 26);
                        }
			if(('0'<= c) && (c <= '9')){
                        c = c-'0';
                        c = c + *cp-'A';// on suppose que la clef est une majuscule tout le temps, sinon il faudrait faire des test pour savoir ce qu'est la clef
                        c = '0'+ (c % 10);
                        }
                        //c^=*(cp++);
                        putc(c,fo);
                }

                fclose(fo);
        }
        fclose(fi);
        }

}



return 1;
}

