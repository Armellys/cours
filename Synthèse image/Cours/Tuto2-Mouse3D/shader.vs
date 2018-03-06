attribute vec3 aVertexPosition; // attribute = on va les récupérer une a une dans un tableau vertex buffer
attribute vec3 aVertexCentre;
uniform mat4 uMVMatrix; // uniform = donnée une fois au début pas petit à petit
uniform mat4 uPMatrix;
varying vec3 pixCentre;

void main(void) {
	pixCentre = vec3(aVertexCentre);
	gl_Position = /*uPMatrix * uMVMatrix **/vec4(aVertexPosition, 1.0); //on applique les matrice de tranformation
}
