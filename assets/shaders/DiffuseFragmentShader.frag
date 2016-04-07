#version 330 core

in vec3 vertexNormal_cameraspace;
in vec3 lightRay_cameraspace;

out vec3 color;

void main(){
  vec3 n = normalize(vertexNormal_cameraspace);
  vec3 l = normalize(lightRay_cameraspace);
  float cosTheta = clamp(dot(n,l), 0,1);
  vec3 materialColor = vec3(.8,0,0);
  color = materialColor * cosTheta;
  //color = materialColor;
}
