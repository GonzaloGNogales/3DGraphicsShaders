#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const float ORBIT_RADIUS = 10.0f;


//Idenficadores de los objetos de la escena
int objId, objId2, objId3 = -1;

//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);

//Header funciones
glm::vec3 calcularPuntoBezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float t);

//Variables de la cámara
float cameraX = 0.0f, cameraZ = -10.0f, cameraAlphaY = 0.0f, cameraAlphaX = 0.0f;
glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 right = glm::vec3(-10.0f, 0.0f, -10.0f);

//Variables del viewport
int w, h;

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
	objId3 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);
	IGlib::setModelMat(objId2, modelMat);
	IGlib::setModelMat(objId3, modelMat);
	//Incluir texturas aquí.
	//IGlib::addColorTex(objId2, "../img/color.png");
	IGlib::addColorTex(objId, "../img/texturaCustom.png");

	//Matriz proyección
	float n = 1.0f;
	float f = 15.0f;
	glm::mat4 proj = glm::mat4(0.0f);
	proj[0].x = 1.0f / glm::tan(30.0f * 3.1419f / 180.0f);
	proj[1].y = proj[0].x;
	proj[2].z = (f + n) / (n - f);
	proj[2].w = -1.0f;
	proj[3].z = 2.0f * n * f / (n - f);

	//Matriz de la cámara
	glm::mat4 view = glm::mat4(1.0f);
	view[3].z = cameraZ;

	//Establecemos el estado de la escena
	IGlib::setViewMat(view);
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
	w = width; h = height;
	
	float n = 1.0f;
	float f = 60.0f;
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
float bezier_t = 0.0f;
const float pi = 2 * acos(0.0f);
void idleFunc()
{
	//Matrices modelo por primer y segundo cubo
	angle = (angle < 2.0f * pi) ? angle + 0.01f: 0.0f;
	glm::vec3 punto_bezier_curva = glm::vec3(0.0f);
	if ((0.0f <= bezier_t) && (bezier_t <= 1.0f)) {  //Cuadrante I: [p0 = 0, p2 = pi/2]
		glm::vec3 p0 = glm::vec3(8.0f, 0.0f, 0.0f);
		glm::vec3 p1 = glm::vec3(9.0f, 0.0f, 10.0f);
		glm::vec3 p2 = glm::vec3(0.0f, 0.0f, 8.0f);
		punto_bezier_curva = calcularPuntoBezier(p0, p1, p2, bezier_t);
		bezier_t += 0.01f;
	}
	else if ((1.0f <= bezier_t) && (bezier_t <= 2.0f)) {  //Cuadrante II [p0 = pi/2, p2 = pi] restamos 1 a t
		glm::vec3 p0 = glm::vec3(0.0f, 0.0f, 8.0f);
		glm::vec3 p1 = glm::vec3(-11.0, 0.0f, 10.0f);
		glm::vec3 p2 = glm::vec3(-8.0f, 0.0f, 0.0f);
		float bezier_T_cuadrante2 = (bezier_t - 1);
		punto_bezier_curva = calcularPuntoBezier(p0, p1, p2, bezier_T_cuadrante2);
		bezier_t += 0.01f;
	}
	else if ((2.0f <= bezier_t) && (bezier_t <= 3.0f)) {  //Cuadrante III [p0 = pi, p2  = 3pi/2] restamos 2 a t
		glm::vec3 p0 = glm::vec3(-8.0f, 0.0f, 0.0f);
		glm::vec3 p1 = glm::vec3(-15.0f, 0.0f, -12.5f);
		glm::vec3 p2 = glm::vec3(0.0f, 0.0f, -8.0f);
		float bezier_T_cuadrante3 = (bezier_t - 2);
		punto_bezier_curva = calcularPuntoBezier(p0, p1, p2, bezier_T_cuadrante3);
		bezier_t += 0.01f;
	}
	else if ((3.0f <= bezier_t) && (bezier_t <= 4.0f)) {  //Cuadrante IV [p0 = 3pi/2, p2 = 2pi] restamos 3 a t
		glm::vec3 p0 = glm::vec3(0.0f, 0.0f, -8.0f);
		glm::vec3 p1 = glm::vec3(13.0f, 0.0f, -8.3f);
		glm::vec3 p2 = glm::vec3(8.0f, 0.0f, 0.0f);
		float bezier_T_cuadrante4 = (bezier_t - 3);
		punto_bezier_curva = calcularPuntoBezier(p0, p1, p2, bezier_T_cuadrante4);
		bezier_t += 0.01f;
	}
	if (bezier_t >= 4.0f) bezier_t = 0.0f;  //Reseteo: t pertenece al intervalo[0, 1]

	glm::mat4 model_frist_cube = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));

	glm::mat4 second_cube_translate = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f));
	glm::mat4 second_cube_rotate = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model_second_cube = second_cube_rotate * second_cube_translate * second_cube_rotate;

	glm::mat4 third_cube_translate = glm::translate(glm::mat4(1.0f), punto_bezier_curva);
	glm::mat4 model_third_cube = third_cube_translate;

	//Establecemos el estado de los modelos
	IGlib::setModelMat(objId, model_frist_cube);
	IGlib::setModelMat(objId2, model_second_cube);
	IGlib::setModelMat(objId3, model_third_cube);
}

glm::vec3 calcularPuntoBezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float t)
{
	float coeficiente_punto0 = pow((1 - t), 2);
	float coeficiente_punto1 = 2 * t * (1 - t);
	float coeficiente_punto2 = pow(t, 2);	    
	return (coeficiente_punto0 * p0) + (coeficiente_punto1 * p1) + (coeficiente_punto2 * p2);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
	const float SPEED = 1.0f;
	const float ALPHA = 5.0f;
	switch (key) {
		case 'w':  //Alante
			cameraX += SPEED * glm::sin(glm::radians(-cameraAlphaY));
			cameraZ += SPEED * glm::cos(glm::radians(-cameraAlphaY));
			break;
		case 'W':
			cameraX += SPEED * glm::sin(glm::radians(-cameraAlphaY));
			cameraZ += SPEED * glm::cos(glm::radians(-cameraAlphaY));
			break;
		case 's':  //Atrás
			cameraX -= SPEED * glm::sin(glm::radians(-cameraAlphaY));
			cameraZ -= SPEED * glm::cos(glm::radians(-cameraAlphaY));
			break;
		case 'S':
			cameraX -= SPEED * glm::sin(glm::radians(-cameraAlphaY));
			cameraZ -= SPEED * glm::cos(glm::radians(-cameraAlphaY));
			break;
		case 'a':  //Izq
			cameraX += SPEED * glm::cos(glm::radians(cameraAlphaY));
			cameraZ += SPEED * glm::sin(glm::radians(cameraAlphaY));
			break;
		case 'A':
			cameraX += SPEED * glm::cos(glm::radians(cameraAlphaY));
			cameraZ += SPEED * glm::sin(glm::radians(cameraAlphaY));
			break;
		case 'd':  //Der
			cameraX -= SPEED * glm::cos(glm::radians(cameraAlphaY));
			cameraZ -= SPEED * glm::sin(glm::radians(cameraAlphaY));
			break;
		case 'D':
			cameraX -= SPEED * glm::cos(glm::radians(cameraAlphaY));
			cameraZ -= SPEED * glm::sin(glm::radians(cameraAlphaY));
			break;
		case 'q':  //RotIzq
			cameraAlphaY -= ALPHA;
			break;
		case 'Q':
			cameraAlphaY -= ALPHA;
			break;
		case 'e':  //RotDer
			cameraAlphaY += ALPHA;
			break;
		case 'E':
			cameraAlphaY += ALPHA;
			break;
	}
	glm::mat4 camera_movement = glm::mat4(1.0f);
	//Inicializar estado actual de cámara (traslación)
	camera_movement[3].x = cameraX;
	camera_movement[3].z = cameraZ;
	//Rotación
	glm::mat4 center_camera = glm::translate(camera_movement, glm::vec3(-cameraX, 0.0f, -cameraZ));  // Matriz para trasladar al centro
	glm::mat4 rotate_camera = glm::rotate(center_camera, glm::radians(cameraAlphaY), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 final_camera_state = glm::translate(rotate_camera, glm::vec3(cameraX, 0.0f, cameraZ));
	IGlib::setViewMat(final_camera_state);

	lookAt.x = cameraX;
	lookAt.z = cameraZ;
	right.x = cameraX;
	right.z = cameraZ;
	lookAt = glm::vec3(lookAt.x + ORBIT_RADIUS * glm::sin(glm::radians(-cameraAlphaY)), 0.0f, lookAt.z + ORBIT_RADIUS * glm::cos(glm::radians(cameraAlphaY)));
	right = glm::vec3(right.x + ORBIT_RADIUS * -glm::cos(glm::radians(-cameraAlphaY)), 0.0f, right.z + ORBIT_RADIUS * -glm::cos(glm::radians(cameraAlphaY)));
	std::cout << "Lookat x: " << lookAt.x << " - lookAt z: " << lookAt.z << std::endl;
	std::cout << "Right x: " << right.x << " - Right z: " << right.z << std::endl;
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
	glm::mat4 camera_orbit = glm::mat4(1.0f);
	//Inicializar estado actual de cámara (traslación)
	camera_orbit[3].x = cameraX;
	camera_orbit[3].z = cameraZ;
	//Rotación
	glm::mat4 center_camera = glm::translate(camera_orbit, glm::vec3(-cameraX, 0.0f, -cameraZ));  // Matriz para trasladar al centro
	glm::mat4 rotate_camera = glm::rotate(center_camera, glm::radians(cameraAlphaY), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 actual_camera_state = glm::translate(rotate_camera, glm::vec3(cameraX, 0.0f, cameraZ));

	float movX = 0.0f - lookAt.x;
	float movZ = 0.0f - lookAt.z;
	//Calcular angle con respecto al cambio en la x del mouse click en el viewport
	float angleX = x;
	float angleZ = y;

	glm::vec3 normalized_lookAt = glm::normalize(lookAt);
	glm::vec3 normalized_right = glm::normalize(right);

	/*std::cout << "Cross product X: " << glm::cross(normalized_lookAt, normalized_right).x << " Cross product y: " <<
		glm::cross(normalized_lookAt, normalized_right).y <<
		" Cross product z: " << glm::cross(normalized_lookAt, normalized_right).z << std::endl;*/

	glm::mat4 translate_to_rot_center = glm::translate(actual_camera_state, glm::vec3(movX, 0.0f, movZ));
	glm::mat4 rotation_from_rot_centerX = glm::rotate(translate_to_rot_center, glm::radians(angleX), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotation_from_rot_centerZ = glm::rotate(rotation_from_rot_centerX, glm::radians(angleZ), glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::mat4 rotation_from_rot_centerZ = glm::rotate(rotation_from_rot_centerX, glm::radians(angleZ), glm::cross(normalized_lookAt, normalized_right));
	glm::mat4 final_view = glm::translate(rotation_from_rot_centerZ, glm::vec3(-movX, 0.0f, -movZ));
	IGlib::setViewMat(final_view);
}
