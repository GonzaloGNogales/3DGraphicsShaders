#version 330 core

in vec3 inPos;		

mat4 model;
mat4 view;
mat4 proj;

void main()
{
	model = mat4(1.0);
	view = mat4 (1.0);
	view[3].z = -3.0;


	float n = 1.0;
	float f = 15.0;
	proj = mat4(0.0);
	proj[0].x = 1.0 / tan(30.0*3.1419/180.0);
	proj[1].y = proj[0].x;
	proj[2].z = (f+n)/(n-f);
	proj[2].w = -1.;
	proj[3].z = 2.0*n*f/(n-f);
	

	gl_Position =   proj * view * model * vec4 (inPos, 1.0);
}
