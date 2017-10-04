#include <stdlib.h>
#include <math.h>

#include <stdio.h>


typedef unsigned long int Huge;

Huge gcd(Huge a,Huge b){//gcd(a,b)

	while(a!= b){

		if(a>b){
			a = a-b;
		}
		else{
			b = b-a;
		}
	}
	return a;
}
static Huge modexp(Huge a, Huge b, Huge n) {
	Huge y;
	y = 1;
	while (b != 0) {
		if (b & 1)
			y = (y * a) % n;
		a = (a * a) % n;
		b = b >> 1;
	}
	return y;
}

Huge rsa_crypt(Huge e, Huge n, Huge m){

	Huge c;
	c = modexp(m,e,n); // m^e % n 
	return c;
}

Huge rsa_decrypt(Huge d, Huge c, Huge n){

        Huge m;
        m = modexp(c,d,n); // c^d % n 
        return m;
}

void generateur(Huge a, Huge b, Huge *e, Huge *n, Huge *d){
	Huge i,j,P,Q;
	Huge div = 0;
/*---- générateur de nombre premier ------ */	
	for(i=a;i>0;i--){
		div = 0;
		for (j=2;j<sqrt(a);j=j+2){
			if (i%j == 0){
				div = 1;
				break;
			}
		}
		if (div == 0){
			P = i;
			break;
		}
	}

        for(i=b;i>0;i--){
                div = 0;
                for (j=2;j<sqrt(b);j=j+2){
                        if (i%j == 0){
                                div = 1;
                                break;
                        }
                }
                if (div == 0){
                        Q = i;
                        break;
                }
        }

/*------------------------------------------*/
 	printf("P:%li\n",P);
        printf("Q:%li\n",Q);

	*n = P*Q;
        Huge phi= (P-1)*(Q-1);
        
        for(*e=2;*e<phi;(*e)++){
                if (gcd((*e),phi)==1){
                        break;
                }
        }
        
        for(*d=2;*d<phi;(*d)++){
                if ((((*e)*(*d))%phi)==1){
                        break;
                }
        }


}


int main(int argc,char *argv[]){

	if (argc<3){
		printf("Erreur argument");
		return 1;
	}

	Huge P=atol(argv[1]);
	Huge Q=atol(argv[2]);

	printf("P:%li\n",P);
	printf("Q:%li\n",Q);

	Huge e=0,n=0,d=0;
	generateur(P,Q,&e,&n,&d);
		

	printf("clef publique : %li,%li\n",e,n);
	printf("clef privée : %li\n",d);
	
	Huge m= 3333;
	Huge c=rsa_crypt(e,n,m);
	printf(" c: %li\n",c);
	
	m=rsa_decrypt(d,c,n);
	printf(" m: %li\n",m);

	return 0;


}
