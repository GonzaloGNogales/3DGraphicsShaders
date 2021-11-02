#version 330 core

out vec4 outColor;

in vec4 vcolor;


void main()
{
	//outColor = vec4(1.0,0.0,0.0,0.0);   
	//outColor = (mod(gl_PrimitiveID,2)==0)?vec4 (0,0,1,0):vec4(1);   
	outColor = vcolor;
}
