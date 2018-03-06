attribute vec2 vertexPosition;//contient x et y
void main(void) {
  gl_Position = vec4(vertexPosition, 0.0, 1.0);//comme on doit avoir 4 dimension, on rajoute 0 pour z et toujours 1 pour w
}
