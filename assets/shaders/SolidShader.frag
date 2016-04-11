#version 330 core

in vec3 vertexNormal_cameraspace;
in vec3 lightRay_cameraspace;

out vec3 color;

uniform vec3 materialColor;

void main(){
  vec3 n = normalize(vertexNormal_cameraspace);
  vec3 l = normalize(lightRay_cameraspace);
  float cosTheta = clamp(dot(n,l), 0,1);

  //Ambient Light
  vec3 ambientColor = vec3(.1,.1,.1);

  color = ambientColor + (materialColor * cosTheta);
}
