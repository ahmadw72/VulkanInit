#version 450
//glslangValidator -V shader1.frag -o fragShader1.spv
layout(binding =1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 texCoord;


layout (location = 0) out vec4 outColor;


void main(){
	//outColor = fragColor; //vec4(fragColor, 1.0);
	outColor = texture(texSampler,texCoord );
}