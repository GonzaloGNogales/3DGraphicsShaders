#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId = -1;
int objId2 = -1;

//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);

//Variables de la cámara
double cameraX = 0.0f, cameraZ = -10.0f;

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("../shaders_P1/shader.v3.vert", "../shaders_P1/shader.v3.frag"))
		return -1;
   
	//Se ajusta la cámara
	//Si no se da valor se cojen valores por defecto
	//IGlib::setProjMat(const glm::mat4 &projMat);
	//IGlib::setViewMat(const glm::mat4 &viewMat);

	//Creamos el objeto que vamos a visualizar
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);
	objId2 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);
		
	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);
	//Incluir texturas aquí.
	IGlib::addColorTex(objId, "../img/color.png");

	//Matriz proyección
	float n = 1.0f;
	float f = 15.0f;
	glm::mat4 proj = glm::mat4(0.0f);
	proj[0].x = 1.0f / glm::tan(30.0f * 3.1419f / 180.0f);
	proj[1].y = proj[0].x;
	proj[2].z = (f + n) / (n - f);
	proj[2].w = -1.0f;
	proj[3].z = 2.0f * n * f / (n - f);

	//Establecemos el estado de la escena
	IGlib::setProjMat(proj);
	
	//CBs
	IGlib::setResizeCB(resizeFunc);
	IGlib::setIdleCB(idleFunc);
	IGlib::setKeyboardCB(keyboardFunc);
	IGlib::setMouseCB(mouseFunc);
	IGlib::setMouseMoveCB(mouseMotionFunc);

	
	//Mainloop
	IGlib::mainLoop();
	IGlib::destroy();
	return 0;
}

void resizeFunc(int width, int height)
{
	float a = float (width) / float(height);
	
	float n = 1.0f;
	float f = 15.0f;
	float x = 1.0f / (glm::tan(30.0f * 3.1419f / 180.0f));
	glm::mat4 proj = glm::mat4(0.0f);
	proj[0].x = x * 1.0 / a;
	proj[1].y = x;
	proj[2].z = (f + n) / (n - f);
	proj[2].w = -1.0f;
	proj[3].z = 2.0f * n * f / (n - f);

	IGlib::setProjMat(proj);
}

float angle = 0.0f; 
void idleFunc()
{
	//Matriz de la cámara
	glm::mat4 view = glm::mat4(1.0f);
	view[3].x = cameraX;
	view[3].z = cameraZ;
	
	//Matrices modelo por primer y segundo cubo
	angle = (angle < 2.0f * 3.14159) ? angle + 0.01f: 0.0f;
	glm::mat4 model_frist_cube = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));

	glm::mat4 second_cube_translate = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));
	glm::mat4 second_cube_rotate = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model_second_cube = second_cube_rotate * second_cube_translate * second_cube_rotate;

	//Establecemos el estado de los modelos
	IGlib::setViewMat(view);
	IGlib::setModelMat(objId, model_frist_cube);
	IGlib::setModelMat(objId2, model_second_cube);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
	switch (key) {
		case 'w':  //Alante
			cameraZ += 0.75f;
			break;
		case 'W':
			cameraZ += 0.75f;
			break;
		case 's':  //Atrás
			cameraZ -= 0.75f;
			break;
		case 'S':
			cameraZ -= 0.75f;
			break;
		case 'a':  //Izq
			cameraX += 0.75f;
			break;
		case 'A':
			cameraX += 0.75f;
			break;
		case 'd':  //Der
			cameraX -= 0.75f;
			break;
		case 'D':
			cameraX -= 0.75f;
			break;
		case 'q':  //RotIzq
			break;
		case 'Q':
			break;
		case 'e':  //RotDer
			break;
		case 'E':
			break;
	}
}

void mouseFunc(int button, int state, int x, int y)
{
	if (state==0)
		std::cout << "Se ha pulsado el botón ";
	else
		std::cout << "Se ha soltado el botón ";
	
	if (button == 0) std::cout << "de la izquierda del ratón " << std::endl;
	if (button == 1) std::cout << "central del ratón " << std::endl;
	if (button == 2) std::cout << "de la derecha del ratón " << std::endl;

	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
}

void mouseMotionFunc(int x, int y)
{
}
