
precision mediump float;
varying vec3 pixCentre;
#define i 0

// ====================================================================
// STRUCTURES DE DONNEES
// ====================================================================

struct rayon {
	vec3 O,V;
	float t;
};

struct sphere {
	vec3 C;
	float r;
};
struct source {
	vec3 pos,pow;

};

// ====================================================================
float raySphere(rayon ray, sphere sph){
	vec3 OC = ray.O - sph.C;
	float delta, t1, t2;
	float b = 2.0*dot(OC,ray.V);//2.0*ray.O.x*ray.V.x+ 2.0*ray.O.z*ray.V.z+ 2.0*ray.O.y*ray.V.y;
	float a = dot(ray.V,ray.V);//ray.V.x*ray.V.x+ray.V.y*ray.V.y+ray.V.z*ray.V.z;
	float c = dot(OC,OC)-(sph.r*sph.r);//ray.O.x*ray.O.x + ray.O.y*ray.O.y + ray.O.z*ray.O.z - (sph.r*sph.r);
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


void initScene1(){

	rayon r1;
	r1.O=vec3(0.0,0.0,0.0);
	r1.V=pixCentre;
	r1.t=1.0;

	sphere s1;
	s1.C = vec3(20.0,150.0,10.0);
	s1.r = 30.0;

	rayon r2;
	r2.O=vec3(0.0,0.0,0.0);
	r2.V=pixCentre;
	r2.t=1.0;

	sphere s2;
	s2.C = vec3(-30.0,190.0,-10.0);
	s2.r = 30.0;

	rayon r3;
	r3.O=vec3(0.0,0.0,0.0);
	r3.V=pixCentre;
	r3.t=1.0;

	sphere s3;
	s3.C = vec3(110.0,400.0,0.0);
	s3.r = 40.0;

	source l1;
	l1.pos=vec3(30.0,100.0,0.0);
	l1.pow=vec3(5.0,5.0,5.0);

	source l2;
	l2.pos=vec3(-50.0,50.0,0.0);
	l2.pow=vec3(5.0,5.0,5.0);

	source tab[2];
	tab[0] = l1;
	tab[1] = l2;


	float t1=raySphere(r1,s1);
	float t2=raySphere(r1,s2);
	if(t1 < 0.0 && t2 < 0.0 ){
		gl_FragColor = vec4(1.0,1.0,1.0,1.0);
	} else {
		vec3 c;
		float t;
		vec3 L = vec3(0.0,0.0,0.0);
		if ((t1<t2||t2<0.0) && t1 >= 0.0){ t=t1; c=s1.C;}
		else if(t2 >=0.0){ t=t2; c=s2.C;}
	
		while (i!=2){
			vec3 I = r1.O + t*r1.V; 
			vec3 N = I - c; 
			N = normalize(N);
			vec3 V = normalize((tab[i].pos)-I);
			float cos = dot(N,V);
			vec3 Kd = vec3(0.8,0.1,0.5);
			L += (tab[i].pow)*Kd/3.14*cos;

			i++;
		}
		/*vec3 I = r1.O + t*r1.V; 
		vec3 N = I - c; 
		N = normalize(N);
		vec3 V = normalize((l1.pos)-I);
		float cos = dot(N,V);
		vec3 Kd = vec3(0.8,0.1,0.5);
		vec3 L = (l1.pow)*Kd/3.14*cos;*/

		gl_FragColor = vec4(L,1.0);

	}


}


void main(void) {
	
	initScene1();


}

