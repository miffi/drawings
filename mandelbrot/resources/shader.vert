#version 450 core
layout (location=0) in vec2 vertex;
layout (location=1) in vec2 coords;

out vec2 out_coords;

void main(){
  gl_Position = vec4(vertex, 0.0, 1.0);
  out_coords = coords;
}
