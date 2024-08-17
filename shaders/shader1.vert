#version 450
//glslangValidator -V shader1.vert -o vertexShader1.spv
layout(binding =0) uniform stMVP {

mat4 m;
mat4 v;
mat4 p;

} MVP;


layout (location =0) in vec3 pos;
layout (location =2) in vec2 texCoord;
layout (location =1) in vec4 color;


layout(location = 0) out vec4 fragColor;
layout (location =1) out vec2 texCoordOut;
vec3 positions[3] = vec3[](
	
	vec3(0.0, -0.5, 0.0),
	vec3(0.5, 0.5, 0.0),
	vec3(-0.5, -0.5, 0.0)
);

vec3 colors[3] = vec3[](
	
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0)
);

void main() {

	//gl_Position = MVP.p * MVP.v * MVP.m *vec4(positions[gl_VertexIndex], 1.0);
	//gl_Position = vec4(positions[gl_VertexIndex], 1.0);
	gl_Position = MVP.p * MVP.v * MVP.m *vec4(pos, 1.0);
	//fragColor = colors[gl_VertexIndex];
	fragColor = color;// vec3(color.x, color.y, color.z);
	texCoordOut = texCoord;

}


