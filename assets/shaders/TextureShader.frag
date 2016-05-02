#version 330 core

in vec3 vertexNormal_cameraspace;
in vec3 lightRay_cameraspace;
in vec2 uv;

out vec3 color;

uniform sampler2D textureSampler;

void main(){
  //vec3 materialColor = vec3(.8,0,0);

  //Diffuse Light
  vec3 n = normalize(vertexNormal_cameraspace);
  vec3 l = normalize(lightRay_cameraspace);
  float cosTheta = clamp(dot(n,l), 0,1);

  //Ambient Light
  vec3 ambientColor = vec3(.15,.15,.15);

  color = ambientColor + (texture(textureSampler, uv).rgb * cosTheta);
}
