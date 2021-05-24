#version 450 core
in vec2 out_coords;

uniform mat4 trans;
uniform float escape;

out vec4 color;

void main(){
  vec4 coord =  trans * vec4(out_coords, 0, 1);
  float x = 0.0, y = 0.0, rsquare = 0.0, isquare = 0.0, zsquare = 0.0;

  int i;
  for(i = 0; i < escape && rsquare + isquare <= 4; i = i + 1){
    x = rsquare - isquare + coord.x;
    y = zsquare - rsquare - isquare + coord.y;
    rsquare = pow(x, 2);
    isquare = pow(y, 2);
    zsquare = pow((x + y), 2);
  }

  color = vec4(1 - i/escape, 1 - i/escape, 1 - i/escape, 1.0);
}
