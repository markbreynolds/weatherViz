#version 330 core
layout(location=0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 vertexUV_modelspace;
layout(location=2) in vec3 vertexNormal_modelspace;

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 v;

uniform vec3 lightPos;

out vec3 vertexNormal_cameraspace;
out vec3 lightRay_cameraspace;
out vec2 uv;

void main(){
  gl_Position = mvp * vec4(vertexPosition_modelspace,1);

  vec3 vertexPosition_cameraspace = (v * m * vec4(vertexPosition_modelspace,1)).xyz;

	vec3 lightPosition_cameraspace = (v * vec4(lightPos.x,lightPos.y,lightPos.z,1)).xyz;

	lightRay_cameraspace = lightPosition_cameraspace - vertexPosition_cameraspace;

  vertexNormal_cameraspace = (v * m * vec4(vertexNormal_modelspace,0)).xyz;

  uv = vertexUV_modelspace;
}
