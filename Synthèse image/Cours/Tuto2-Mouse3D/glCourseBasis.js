
// =====================================================
var gl;
var shadersLoaded = 0;
var vertShaderTxt;
var fragShaderTxt;
var shaderProgram = null;
var vertexBuffer;
var mvMatrix = mat4.create();
var pMatrix = mat4.create();
var objMatrix = mat4.create();
var matPixmv = mat3.create();
var matPixp = mat3.create();
var inc = 0;
mat4.identity(objMatrix);



// =====================================================
function webGLStart() {
	var canvas = document.getElementById("WebGL-test");
	canvas.onmousedown = handleMouseDown;
	document.onmouseup = handleMouseUp;
	document.onmousemove = handleMouseMove;

	initGL(canvas);
	initBuffers();
	loadShaders('shader');

	gl.clearColor(0.7, 0.7, 0.7, 1.0);

	tick();
}

// =====================================================
function initGL(canvas)
{
	try {
		gl = canvas.getContext("experimental-webgl");
		gl.viewportWidth = canvas.width;
		gl.viewportHeight = canvas.height;
		gl.viewport(0, 0, canvas.width, canvas.height);
	} catch (e) {}
	if (!gl) {
		console.log("Could not initialise WebGL");
	}
}

// =====================================================
function initBuffers() {
	vertexBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
	vertices = [
							-1, -1.0, 0.0,
							-1,  1.0, 0.0,
							 1,  1.0, 0.0,
							 1, -1.0, 0.0
							];
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
	vertexBuffer.itemSize = 3;
	vertexBuffer.numItems = 4;

	centreBuffer = gl.createBuffer();
	PsX = 36 / 720;
	PsY = 24 / 480;
	gl.bindBuffer(gl.ARRAY_BUFFER, centreBuffer);
	center = [
							-18+PsX/2.0, 50, 12-PsY/2.0,
							-18+PsX/2.0, 50, -12+PsY/2.0,
							18-PsX/2.0, 50, -12+PsY/2.0,
							18-PsX/2.0, 50, 12-PsY/2.0,
							
							];
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(center), gl.STATIC_DRAW);
	centreBuffer.itemSize = 3;
	centreBuffer.numItems = 4;

}


// =====================================================
function loadShaders(shader) {
	loadShaderText(shader,'.vs');
	loadShaderText(shader,'.fs');
}

// =====================================================
function loadShaderText(filename,ext) {   // technique car lecture asynchrone...
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == 4 && xhttp.status == 200) {
			if(ext=='.vs') { vertShaderTxt = xhttp.responseText; shadersLoaded ++; }
			if(ext=='.fs') { fragShaderTxt = xhttp.responseText; shadersLoaded ++; }
			if(shadersLoaded==2) {
				initShaders(vertShaderTxt,fragShaderTxt);
				shadersLoaded=0;
			}
    }
  }
  xhttp.open("GET", filename+ext, true);
  xhttp.send();
}

// =====================================================
function initShaders(vShaderTxt,fShaderTxt) {

	vshader = gl.createShader(gl.VERTEX_SHADER);
	gl.shaderSource(vshader, vShaderTxt);
	gl.compileShader(vshader);
	if (!gl.getShaderParameter(vshader, gl.COMPILE_STATUS)) {
		console.log(gl.getShaderInfoLog(vshader));
		return null;
	}

	fshader = gl.createShader(gl.FRAGMENT_SHADER);
	gl.shaderSource(fshader, fShaderTxt);
	gl.compileShader(fshader);
	if (!gl.getShaderParameter(fshader, gl.COMPILE_STATUS)) {
		console.log(gl.getShaderInfoLog(fshader));
		return null;
	}

	shaderProgram = gl.createProgram();
	gl.attachShader(shaderProgram, vshader);
	gl.attachShader(shaderProgram, fshader);

	gl.linkProgram(shaderProgram);

	if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
		console.log("Could not in/itialise shaders");
	}

	gl.useProgram(shaderProgram);

	shaderProgram.vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
	gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);

	shaderProgram.vertexCentreAttribute= gl.getAttribLocation(shaderProgram, "aVertexCentre");
	gl.enableVertexAttribArray(shaderProgram.vertexCentreAttribute);

	shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
	shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
	incLocation = gl.getUniformLocation(shaderProgram,"inc");
}


// =====================================================
function setMatrixUniforms() {
	if(shaderProgram != null) {
		gl.uniformMatrix3fv(shaderProgram.pMatrixUniform, false, mat4.toMat3(pMatrix, matPixp));
		gl.uniformMatrix3fv(shaderProgram.mvMatrixUniform, false, mat4.toMat3(mvMatrix, matPixmv));
		gl.uniform1i(incLocation, inc);
	}
}

// =====================================================
function drawScene() {
  gl.clear(gl.COLOR_BUFFER_BIT);

  if(shaderProgram != null) {
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute,
                           vertexBuffer.itemSize/*3 car 3D*/, gl.FLOAT, false, 0, 0);
     gl.bindBuffer(gl.ARRAY_BUFFER, centreBuffer);
    gl.vertexAttribPointer(shaderProgram.vertexCentreAttribute,
                           centreBuffer.itemSize/*3 car 3D*/, gl.FLOAT, false, 0, 0);

    mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix);
    mat4.identity(mvMatrix);
   // mat4.translate(mvMatrix, [0.0, 0.0, -5.0]);
    mat4.multiply(mvMatrix, objMatrix);

    setMatrixUniforms();

    gl.drawArrays(gl.TRIANGLE_FAN, 0, vertexBuffer.numItems);
  }
}
