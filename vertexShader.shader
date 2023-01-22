
#version 330 core
layout ( location = 0 ) in vec3 vPos; 
out vec2 fragCoord;
uniform vec2 iResolution;

void main() {
gl_Position = vec4(vPos.xyz, 1.0); 
fragCoord = (vPos.xy * .5 + .5) * iResolution;
//color = vCol;
}





//oude versie voor shadertoy
//#version 330 core
//
//layout(location = 0) in vec3 vPos;
//layout(location = 1) in vec3 vColor;
//layout(location = 2) in vec2 vUv;
//layout(location = 3) in vec3 vNormal;
//
//uniform mat4 world;
//uniform mat4 view;
//uniform mat4 projection;
//
//
//out vec3 vertexColor;
//out vec2 uv;
//out vec3 normal;
//out vec4 worldPixel;
//
//void main() {
//	vec4 worldPixel = world * vec4(vPos, 1.0f);
//	//projection - view - world
//	gl_Position = projection * view * world * vec4(vPos, 1.0f);
//	vertexColor = vColor;
//	uv = vUv;
//	normal = mat3(world) * vNormal;
//}

