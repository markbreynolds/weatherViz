#version 330 core
layout(location=0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 vertexUV_modelspace;
layout(location=2) in vec3 vertexNormal_modelspace;

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 v;

uniform vec3 lightPos;

out vec2 uv;

void main(){
  gl_Position = mvp * vec4(vertexPosition_modelspace,1);

  uv = vertexUV_modelspace;
}
