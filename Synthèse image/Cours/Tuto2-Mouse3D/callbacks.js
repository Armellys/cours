

// =====================================================
// Mouse management
// =====================================================
var mouseDown = false;//si la souri est cliquer
var lastMouseX = null;
var lastMouseY = null;// les dernière coordonées de la sourie
var lastMouseZ = null;
var rotY = 0;
var rotX = 0;
var rotZ = 0;

// =====================================================
window.requestAnimFrame = (function() //regarde si il se passe quelquechose en boucle
{
	return window.requestAnimationFrame ||
         window.webkitRequestAnimationFrame ||
         window.mozRequestAnimationFrame ||
         window.oRequestAnimationFrame ||
         window.msRequestAnimationFrame ||
         function(/* function FrameRequestCallback */ callback,
									/* DOMElement Element */ element)
         {
            window.setTimeout(callback, 1000/60);
         };
})();

// ==========================================
function tick() {
	requestAnimFrame(tick);
	drawScene();
	inc = inc+1;
}

// =====================================================
function degToRad(degrees) { //transforme radian en degree
	return degrees * Math.PI / 180;
}

// =====================================================
function handleMouseDown(event) { //gère le click
	mouseDown = true;
	lastMouseX = event.clientX;
	lastMouseY = event.clientY;
	lastMouseZ = event.clientZ;
}


// =====================================================
function handleMouseUp(event) {//fin du click
	mouseDown = false;
}


// =====================================================
function handleMouseMove(event) {// quand la souri bouge
	if (!mouseDown) {//si on est pas cliquer on fait rien
		return;
	}
	var newX = event.clientX;
	var newY = event.clientY;
	var newZ = event.clientZ;

	var deltaX = newX - lastMouseX;
	var deltaY = newY - lastMouseY;
	var deltaZ = newZ - lastMouseZ;
	
//on calcul la rotation
	rotY += degToRad(deltaX / 8);
	rotX += degToRad(deltaY / 8);
	rotZ += degToRad(deltaY / 8);
//on met à jour la matrice de rotation :
	mat4.identity(objMatrix);
	mat4.rotate(objMatrix, rotY, [0, 0, -1]);
	mat4.rotate(objMatrix, rotX, [1, 0, 0]);
	//mat4.rotate(objMatrix, rotZ, [0, 1, 1]);
//on met à jour :
	lastMouseX = newX
	lastMouseY = newY;
	lastMouseZ = newZ;
}
