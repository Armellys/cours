
precision mediump float;
varying vec3 pixCentre;
uniform int inc;

#define NOMBRE_BOULE 18 //nombre de boules
#define PI 3.14 
#define NOMBRE_SOURCE 2 //nombre de sources
#define NB_PLANS 4

// ====================================================================
// STRUCTURES DE DONNEES
// ====================================================================

struct rayon {
	vec3 O,V;
	float t;
};

struct sphere {
	vec3 C,Kd;
	float r;
};

struct source {
	vec3 pos,pow;

};


struct plan{
	vec4 N;
	vec3 Kd;
};

struct sphereMin{
	sphere sph;
	float t;
};

// ====================================================================

float raySphere(rayon ray, sphere sph){
	vec3 OC = ray.O - sph.C;
	float delta, t1, t2;
	float b = 2.0*dot(OC,ray.V);
	float a = dot(ray.V,ray.V);
	float c = dot(OC,OC)-(sph.r*sph.r);
	delta= b*b-4.0*a*c;

	if(delta < 0.0){	
		return -1.0;
	} else{
		t1=(-b-sqrt(delta))/(2.0*a);
		t2=(-b+sqrt(delta))/(2.0*a);

		if(t1<0.0) ray.t = t2;
		else if(t2<0.0) ray.t = t1;
		else ray.t=(t1<t2)?t1:t2;

		return ray.t;
	}
}

// ====================================================================

float rayPlan(rayon ray, plan p){
	vec3 OS = p.N.w-ray.O;
	vec3 Norm = p.N.xyz;
	float d = dot(Norm,ray.V);
	float t= dot(Norm,OS)/d;
	return t;
}

// ====================================================================

vec3 eclairementMat(rayon r,float t,sphere sph,source tab[NOMBRE_SOURCE]){
	vec3 L = vec3(0.0,0.0,0.0);
	for(int i=0;i<NOMBRE_SOURCE;i++){
		vec3 I = r.O + t*r.V; 
		vec3 n = I - sph.C; 
		n = normalize(n);
		vec3 V = normalize((tab[i].pos)-I);
		float cos = dot(n,V);
		if(cos<0.0) cos=0.0;
		L += (tab[i].pow)*sph.Kd/3.14*cos;

		}
		return L;
}

// ====================================================================

vec3 eclairementBrillant(rayon r,float t,sphere sph,source tab[NOMBRE_SOURCE]){
	vec3 L = vec3(0.0,0.0,0.0);
	for(int i=0;i<NOMBRE_SOURCE;i++){
		vec3 I = r.O + t*r.V; 
		vec3 n = I - sph.C; 
		n = normalize(n);
		vec3 V = normalize((tab[i].pos)-I);
		float cos = dot(n,V);
		if(cos<0.0) cos=0.0;
		vec3 Ks = vec3(0.2,0.2,0.1);
		float N = 20.0;
		vec3 V0=normalize(-r.V);
		vec3 H = normalize(V0+V);
		float cosalpha=dot(H,n);
		if(cosalpha<0.0) cosalpha=0.0;
		vec3 fr=(sph.Kd/3.14)+((N+2.0)*Ks*pow(cosalpha,N))/(2.0*3.14);
		L += (tab[i].pow)*fr*cos;

		}
		return L;
}

// ====================================================================

bool affichageOmbre(rayon r,float t,sphere sph, sphere tabS[NOMBRE_BOULE],source tab[NOMBRE_SOURCE]){
	bool ombre = false;
		for(int i = 0 ; i<NOMBRE_SOURCE ; i++){
			rayon ra=rayon(vec3(r.O + t*r.V),tab[i].pos,1.0);
			for(int j = 0 ; j<NOMBRE_BOULE ; j++){
				if(raySphere(ra,tabS[j])>=0.0 && sph!=tabS[j]){
					gl_FragColor = vec4(0.0,0.0,0.0,1.0);
					ombre = true;
					break;
				}
			}
			if(ombre)break;
		}	
	return ombre;
}

// ====================================================================

sphereMin testBoule(rayon r, sphere tabS[NOMBRE_BOULE]){
	float test = raySphere(r,tabS[0]);
	sphereMin result;
	for(int i = 0 ; i<NOMBRE_BOULE ; i++){
		float t = raySphere(r,tabS[i]);
		if(t >= 0.0 && (t<= test || test < 0.0)){
			result.t=t; 
			result.sph=tabS[i]; 
		}
		test = t;
	}
	return result;
}


// ====================================================================

void afficheBoules(sphere tabS[NOMBRE_BOULE], source tab[NOMBRE_SOURCE], bool brillant, bool shadow, vec4 fond ){
	
	rayon r = rayon(vec3(0.0,0.0,0.0),pixCentre,1.0);
	vec3 L;
	int vide = 0;
	for(int i = 0 ; i<NOMBRE_BOULE ; i++){
		if(raySphere(r,tabS[i]) < 0.0 ){
			vide = vide+1;
		} 
	}	
	if(vide==NOMBRE_BOULE){
		gl_FragColor = fond;
	} else {
		/******** Test de quelles boules est devant ***********/
		sphereMin boule= testBoule(r,tabS);

		/************ Test Ombre *******************/
		bool ombre = false;		
		if(shadow) ombre = affichageOmbre(r,boule.t,boule.sph,tabS,tab); 

		/********** affiche et eclairement ******/
		if(!ombre){
			if(brillant) L = eclairementBrillant(r,boule.t,boule.sph,tab);
			else L = eclairementMat(r,boule.t,boule.sph,tab);
			gl_FragColor = vec4(L,1.0);
		}
	}
}

// ====================================================================

void CollierBoule( float R, source tab[NOMBRE_SOURCE]){
	
	sphere tabS[NOMBRE_BOULE];
	float a = 0.0;
	for(int i = 0 ; i<NOMBRE_BOULE ; i++){
		tabS[i] = sphere(vec3(cos(a)*R,200.0,-sin(a)*R),vec3(0.89,0.73,0.47),3.6);
		a = a +0.35;
	}

	afficheBoules(tabS,tab,true,false,vec4(1.0,1.0,1.0,1.0));	
}

// ====================================================================

void droiteBoule( rayon droite, source tab[NOMBRE_SOURCE]){
	
	sphere tabS[NOMBRE_BOULE];
	
	for(int i = 0 ; i<NOMBRE_BOULE ; i++){
		tabS[i] = sphere(vec3(droite.O),vec3(0.2,0.73,0.47),25.0);
		droite.O = droite.O+droite.V;
	}
	afficheBoules(tabS,tab,false,false,vec4(1.0,1.0,1.0,1.0));
}

// ====================================================================

void initSystemSolaireOptimise(){


	sphere tab_s[NOMBRE_BOULE];
	source tab_l[NOMBRE_SOURCE];

	sphere s1;/*soleil*/
	s1.C = vec3(0.0,600.0,0.0);
	s1.r = 40.0;
	s1.Kd=vec3(1.0,1.0,0.1);
	tab_s[0]=s1;

	sphere s2;/*venus*/
	s2.C = vec3(-200,700.0,0.0);
	s2.r = 10.0;
	s2.Kd=vec3(0.89,0.73,0.47);
	//tab_s[4]=s2;

	sphere s3;/*terre*/
	s3.C = vec3(200.0,700.0,0.0);
	s3.r = 15.0;
	s3.Kd=vec3(0.04,0.03,0.2);
	//tab_s[2]=s3;

	sphere s4;/*mars*/
	s4.C = vec3(100.0,800.0,0.0);
	s4.r = 10.0;
	s4.Kd=vec3(0.35,0.16,0.10);
	//tab_s[3]=s4;

	sphere s5;/*lune*/
	s5.C =vec3(sin(float(inc*8)/360.0)*25.0+200.0,cos(float(inc*8)/360.0)*25.0+700.0,0.0);
	s5.r = 3.0;
	s5.Kd=vec3(0.61,0.61,0.61);
	//tab_s[1]=s5;

	sphere s6;/*mercure*/
	s6.C = vec3(-70.0,600.0,0.0);
	s6.r = 7.0;
	s6.Kd=vec3(0.79,0.60,0.34);
	//tab_s[5]=s6;

	source l1;
	l1.pos=vec3(0.0,540.0,0.0);
	l1.pow=vec3(7.0,7.0,7.0);
	tab_l[0]=l1;

	afficheBoules(tab_s,tab_l,false,false,vec4(0.0,0.0,0.0,1.0));
	

}

// ====================================================================

void initSceneCollier(){

	source l1;
	l1.pos=vec3(100.0,170.0,0.0);
	l1.pow=vec3(4.0,4.0,4.0);

	source l2;
	l2.pos=vec3(-100.0,170.0,0.0);
	l2.pow=vec3(3.0,3.0,3.0);

	source tab[NOMBRE_SOURCE];
	tab[0] = l1;
	tab[1] = l2;

	CollierBoule(20.0,tab);
}

// ====================================================================

void initSceneDroite(){

	source l1;
	l1.pos=vec3(100.0,170.0,0.0);
	l1.pow=vec3(4.0,4.0,4.0);

	source l2;
	l2.pos=vec3(-100.0,170.0,0.0);
	l2.pow=vec3(3.0,3.0,3.0);

	source tab[NOMBRE_SOURCE];
	tab[0] = l1;
	tab[1] = l2;

	rayon d = rayon(vec3(-35.0,200.0,-10.0),vec3(10.0,0.0,5.0),0.0);

	droiteBoule(d,tab);
}

// ====================================================================


void initScene1(){

	sphere s1;
	s1.C = vec3(20.0,200.0,10.0);
	s1.Kd = vec3(0.8,0.0,0.5);
	s1.r = 30.0;

	sphere s2;
	s2.C = vec3(-30.0,240.0,-10.0);
	s2.Kd = vec3(0.8,0.0,0.5);
	s2.r = 30.0;

	source l1;
	l1.pos=vec3(100.0,170.0,0.0);
	l1.pow=vec3(3.0,3.0,3.0);

	source l2;
	l2.pos=vec3(-100.0,170.0,0.0);
	l2.pow=vec3(3.0,3.0,3.0);

	source tab[NOMBRE_SOURCE];
	tab[0] = l1;
	tab[1] = l2;

	sphere tabS[NOMBRE_BOULE];
	tabS[0]= s1;
	tabS[1]= s2;

	afficheBoules(tabS,tab,false,false,vec4(1.0,1.0,1.0,1.0));


}

// ====================================================================

void initTestOmbre()
{	
	sphere tab_s[NOMBRE_BOULE];

	sphere s1;
	s1.C = vec3(70.0,300.0,10.0);
	s1.r = 20.0;
	s1.Kd = vec3(0.8,0.0,0.5);
	tab_s[0]=s1;

	sphere s2;
	s2.C = vec3(-30.0,300.0,10.0);
	s2.r = 45.0;
	s2.Kd = vec3(0.8,0.0,0.5);
	tab_s[1]=s2;

	sphere s3;
	s3.C = vec3(50.0,300.0,50.0);
	s3.r = 20.0;
	s3.Kd = vec3(0.8,0.0,0.5);
	//tab_s[2]=s3;

	source l1;
	l1.pos=vec3(200.0,130.0,0.0);
	l1.pow=vec3(3.0,3.0,3.0);

	source l2;
	l2.pos=vec3(200.0,130.0,-50.0);
	l2.pow=vec3(3.0,3.0,3.0);

	source tab[NOMBRE_SOURCE];
	tab[0] = l1;
	tab[1] = l2;

	afficheBoules(tab_s,tab,false,true,vec4(1.0,1.0,1.0,1.0));


}

// ====================================================================

void initTestOmbreFauxPlan()
{	
	sphere tab_s[NOMBRE_BOULE];

	sphere s1;
	s1.C = vec3(0.0,300.0,-10.0);
	s1.r = 20.0;
	s1.Kd = vec3(0.8,0.8,0.0);
	tab_s[0]=s1;

	sphere s2;
	s2.C = vec3(0.0,400.0,1000.0);
	s2.r = 1000.0;
	s2.Kd = vec3(0.6,0.8,0.7);
	tab_s[1]=s2;

	sphere s3;
	s3.C = vec3(70.0,300.0,-20.0);
	s3.r = 20.0;
	s3.Kd = vec3(0.0,0.8,0.8);
	tab_s[2]=s3;

	source l1;
	l1.pos=vec3(0.0,300.0,-100.0);
	l1.pow=vec3(3.0,3.0,3.0);

	source l2;
	l2.pos=vec3(200.0,130.0,-50.0);
	l2.pow=vec3(3.0,3.0,3.0);

	source tab[NOMBRE_SOURCE];
	tab[0] = l1;
	tab[1] = l2;

	afficheBoules(tab_s,tab,true,true,vec4(1.0,1.0,1.0,1.0));


}


void main(void) {
	
	//initSystemSolaireOptimise();
	//initScene1();
	//initSceneDroite();
	initSceneCollier();
	//initTestOmbre();
	//initTestOmbreFauxPlan();

}

