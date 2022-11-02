/* TRANSFORMARI.
Realizarea unei scene 2D in care obiectele se misca
   - unele primitive raman fixe, altele isi schimba pozitia
   - functii pentru tastatura: processNormalKeys, processSpecialKeys
   - pentru animatie: glutIdleFunc
*/
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include <chrono>

#include "loadShaders.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

//////////////////////////////////////

GLuint
	VaoId,
	VboId,
	ColorBufferId,
	ProgramId,
	myMatrixLocation,
	matrScaleLocation,
	matrTranslLocation,
	matrRotlLocation,
	codColLocation;

int codCol, stopGame = false, isGameLost = false, isGameWon = false, isGamePaused = false, isSignalTurned = false, isSignalOn = false, pauseSignal = false;
float PI = 3.141592, angle = 0;
float xCar2 = 0; float yCar2 = 0; 
float width = 400, height = 200;
float iCar1 = 0, stepCar1 = 0.0025;
float iCar2 = 0, stepCar2 = 0.0025;
float iCar3 = 0, stepCar3 = 0.0035;
std::chrono::time_point<std::chrono::steady_clock> startTime, currentTime;
glm::mat4 
	myMatrix, resizeMatrix, matrTranslCar1, matrTranslCar2,
	matrRotCar3, matrPozitionareCar3, matrTranslCar3, matrTranslInitPosCar3, matrFixRotation3;


bool isCollisionWithCar1(int x, int y) {
	return ((x >= 75 + iCar1 && x <= 155 + iCar1
			 && y >= 34 && y <= 76)
		|| (x >= 155 + iCar1 && x <= 183 + iCar1
			&& y >= 41 && y <= 69));
}

bool isCollsionWithCar3(int x, int y) {
	return (x >= 170 + iCar3 && x <= 210 + iCar3
			&& y >= 125 && y <= 155);
}
bool isColissionBetweenCar2AndCar1() {
	return isCollisionWithCar1(20 + iCar2 + xCar2, 40 + yCar2)
		|| isCollisionWithCar1(20 + iCar2 + xCar2, 70 + yCar2)
		|| isCollisionWithCar1(60 + iCar2 + xCar2, 40 + yCar2)
		|| isCollisionWithCar1(60 + iCar2 + xCar2, 70 + yCar2);
}

bool isColissionBetweenCar2AndCar3() {
	return isCollsionWithCar3(20 + iCar2 + xCar2, 40 + yCar2)
		|| isCollsionWithCar3(20 + iCar2 + xCar2, 70 + yCar2)
		|| isCollsionWithCar3(60 + iCar2 + xCar2, 40 + yCar2)
		|| isCollsionWithCar3(60 + iCar2 + xCar2, 70 + yCar2);
}
bool isColissionWithBottomMargin(int y) {
	return y < 0;
}

bool isColissionBetweenCar2AndBottomMargin() {
	return isColissionWithBottomMargin(40 + yCar2);
}

bool isColissionWithUpperMargin(int y) {
	return y > 200;
}

bool isColissionBetweenCar2AndUpperMargin() {
	return isColissionWithUpperMargin(70 + yCar2);
}

bool isColissionWithLeftMargin(int x) {
	return x < 0;
}

bool isColissionBetweenCar2AndLeftMargin() {
	return isColissionWithLeftMargin(20 + iCar2 + xCar2);
}

bool isColissionWithUpperRightMargin(int x, int y) {
	return y > 100 && x >= 400;
}

bool hasCar1Finished() {
	return 183 + iCar1 >= 400;
}

bool isColissionBetweenCar2AndUpperRightMargin() {
	return isColissionWithUpperRightMargin(60 + iCar2 + xCar2, 70 + yCar2);
}
bool hasCar2Collision() {
	return isColissionBetweenCar2AndCar1()
		|| isColissionBetweenCar2AndCar3()
		|| hasCar1Finished()
		|| isColissionBetweenCar2AndBottomMargin()
		|| isColissionBetweenCar2AndUpperMargin()
		|| isColissionBetweenCar2AndLeftMargin()
		|| isColissionBetweenCar2AndUpperRightMargin();
}

bool finishTheRoadOnTheRightLine(int x, int y) {
	return x > 400 && y <= 100;
}
bool hasCar2WonTheGame() {
	return finishTheRoadOnTheRightLine(20 + iCar2 + xCar2, 70 + yCar2);
}

void moveCarsByDefault(void) {
	if (hasCar2Collision()) {
		isGameLost = true;
		pauseSignal = true;
	}

	if (hasCar2WonTheGame()) {
		isGameWon = true;
		pauseSignal = true;
	}

	if (isGameLost == false && isGameWon == false && isGamePaused == false) {
		iCar1 += stepCar1;
		iCar2 += stepCar2;
		iCar3 -= stepCar3;
	}
	
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key) {
	case 32:
		// pause/resume
		if (isGameLost == false && isGameLost == false) {
			isGamePaused = ~isGamePaused;
			pauseSignal = ~pauseSignal;
		}
		break;
	case 'r':
		// reset
		iCar1 = 0;
		iCar2 = 0;
		iCar3 = 0;
		xCar2 = 0;
		yCar2 = 0;
		isGameWon = false;
		isGameLost = false;
		isGamePaused = false;
		isSignalOn = false;
		isSignalTurned = false;
		pauseSignal = false;
		break;
	case 's':
		isSignalOn = ~isSignalOn;
		startTime = std::chrono::steady_clock::now();
		break;
	}
	
	if (key == 27)
		exit(0);
}
void processSpecialKeys(int key, int xx, int yy) 
{
	if (isGameLost == false && isGameWon == false && isGamePaused == false) {
		switch (key)
		{
		case GLUT_KEY_LEFT:
			xCar2 -= 5;
			break;
		case GLUT_KEY_RIGHT:
			xCar2 += 5;
			break;
		case GLUT_KEY_UP:
			yCar2 += 5;
			break;
		case GLUT_KEY_DOWN:
			yCar2 -= 5;
			break;
		}
	}
	
}


void CreateVBO(void)
{
	// varfurile 
	GLfloat Vertices[] = {
		// middle of the road
		0.0f, 100.0f, 0.0f, 1.0f,
		40.0f, 100.0f, 0.0f, 1.0f,

		60.0f, 100.0f, 0.0f, 1.0f,
		110.0f, 100.0f, 0.0f, 1.0f,

		130.0f, 100.0f, 0.0f, 1.0f,
		180.0f, 100.0f, 0.0f, 1.0f,

		200.0f, 100.0f, 0.0f, 1.0f,
		250.0f, 100.0f, 0.0f, 1.0f,

		270.0f, 100.0f, 0.0f, 1.0f,
		320.0f, 100.0f, 0.0f, 1.0f,

		340.0f, 100.0f, 0.0f, 1.0f,
		390.0f, 100.0f, 0.0f, 1.0f,

		// car 2
		20.0f, 40.0f, 0.0f, 1.0f,
		20.0f, 70.0f, 0.0f, 1.0f,
		60.0f, 70.0f, 0.0f, 1.0f,
		60.0f, 40.0f, 0.0f, 1.0f,

		// car2 rear window
		25.0f, 45.0f, 0.0f, 1.0f,
		30.0f, 43.0f, 0.0f, 1.0f,
		30.0f, 67.0f, 0.0f, 1.0f,
		25.0f, 65.0f, 0.0f, 1.0f,


		// car 2 windscreen
		50.0f, 45.0f, 0.0f, 1.0f,
		55.0f, 43.0f, 0.0f, 1.0f,
		55.0f, 67.0f, 0.0f, 1.0f,
		50.0f, 65.0f, 0.0f, 1.0f,

		//car 2 top window
		55.0f, 68.0f, 0.0f, 1.0f,
		48.0f, 65.0f, 0.0f, 1.0f,
		33.0f, 65.0f, 0.0f, 1.0f,
		30.f, 68.0f, 0.0f, 1.0f,

		// car 2 down window
		55.0f, 42.0f, 0.0f, 1.0f,
		48.0f, 45.0f, 0.0f, 1.0f,
		33.0f, 45.0f, 0.0f, 1.0f,
		30.f, 42.0f, 0.0f, 1.0f,
		
		// car 2 top headlight
		59.0f, 69.0f, 0.0f, 1.0f,
		57.5f, 69.0f, 0.0f, 1.0f,
		57.5f, 64.0f, 0.0f, 1.0f,
		59.0f, 64.0f, 0.0f, 1.0f,

		// car 2 down headlight
		59.0f, 41.0f, 0.0f, 1.0f,
		57.5f, 41.0f, 0.0f, 1.0f,
		57.5f, 46.0f, 0.0f, 1.0f,
		59.0f, 46.0f, 0.0f, 1.0f,

		// car 2 top rear light
		21.0f, 69.0f, 0.0f, 1.0f,
		22.5f, 69.0f, 0.0f, 1.0f,
		22.5f, 64.0f, 0.0f, 1.0f,
		21.0f, 64.0f, 0.0f, 1.0f,

		// car 2 down rear light
		21.0f, 41.0f, 0.0f, 1.0f,
		22.5f, 41.0f, 0.0f, 1.0f,
		22.5f, 46.0f, 0.0f, 1.0f,
		21.0f, 46.0f, 0.0f, 1.0f,

		// car 1 tow
		75.0f, 34.0f, 0.0f, 1.0f,
		75.0f, 76.0f, 0.0f, 1.0f,
		155.0f, 76.0f, 0.0f, 1.0f,
		155.0f, 34.0f, 0.0f, 1.0f,

		// car 1 front part
		155.0f, 41.0f, 0.0f, 1.0f,
		155.0f, 69.0f, 0.0, 1.0f,
		183.0f, 69.0f, 0.0f, 1.0f,
		183.0f, 41.0f, 0.0f, 1.0f,

		// car 1 aprt between tow and front part
		155.0f, 41.0f, 0.0f, 1.0f,
		155.0f, 69.0f, 0.0f, 1.0f,
		156.5f, 69.0f, 0.0f, 1.0f,
		156.5f, 41.0f, 0.0f, 1.0f,

		// car 1 windscreen
		177.0f, 66.0f, 0.0f, 1.0f,
		177.0f, 44.0f, 0.0f, 1.0f,
		171.0f, 46.0f, 0.0f, 1.0f,
		171.0f, 64.0f, 0.0f, 1.0f,

		// car 1 top window
		162.0f, 69.0f, 0.0f, 1.0f,
		170.0f, 69.0f, 0.0f, 1.0f,
		170.0f, 67.5f, 0.0f, 1.0f,
		162.0f, 67.5f, 0.0f, 1.0f,

		// car 1 down window
		162.0f, 41.0f, 0.0f, 1.0f,
		170.0f, 41.0f, 0.0f, 1.0f,
		170.0f, 42.5f, 0.0f, 1.0f,
		162.0f, 42.5f, 0.0f, 1.0f,

		// car 1 top headight
		182.0f, 68.0f, 0.0f, 1.0f,
		182.0f, 63.0f, 0.0f, 1.0f,
		180.5f, 63.0f, 0.0f, 1.0f,
		180.5f, 68.0f, 0.0f, 1.0f,

		// car 1 down headlight
		182.0f, 42.0f, 0.0f, 1.0f,
		182.0f, 47.0f, 0.0f, 1.0f,
		180.5f, 47.0f, 0.0f, 1.0f,
		180.5f, 42.0f, 0.0f, 1.0f,

		// game over message
		
		// G
		150.0f, 125.0f, 0.0f, 1.0f,
		140.0f, 125.0f, 0.0f, 1.0f,

		140.0f, 125.0f, 0.0f, 1.0f,
		140.0f, 75.0f, 0.0f, 1.0f,

		140.0f, 75.0f, 0.0f, 1.0f,
		150.0f, 75.0f, 0.0f, 1.0f,

		150.0f, 75.0f, 0.0f, 1.0f,
		150.0f, 100.0f, 0.0f, 1.0f,

		150.0f, 100.0f, 0.0f, 1.0f,
		145.0f, 100.0f, 0.0f, 1.0f,

		// A
		155.0f, 75.0f, 0.0f, 1.0f,
		160.0f, 125.0f, 0.0f, 1.0f,
		
		160.0f, 125.0f, 0.0f, 1.0f,
		165.0f, 75.0f, 0.0f, 1.0f,

		157.5f, 100.0f, 0.0f, 1.0f,
		162.5f, 100.0f, 0.0f, 1.0f,

		// M
		170.0f, 75.0f, 0.0f, 1.0f,
		170.0f, 125.0f, 0.0f, 1.0f,

		170.0f, 125.0f, 0.0f, 1.0f,
		175.0f, 100.0f, 0.0f, 1.0f,

		175.0f, 100.0f, 0.0f, 1.0f,
		180.0f, 125.0f, 0.0f, 1.0f,

		180.0f, 125.0f, 0.0f, 1.0f,
		180.0f, 75.0f, 0.0f, 1.0f,

		// E 
		195.0f, 125.0f, 0.0f, 1.0f,
		185.0f, 125.0f, 0.0f, 1.0f,

		185.0f, 125.0f, 0.0f, 1.0f,
		185.0f, 75.0f, 0.0f, 1.0f,

		185.0f, 75.0f, 0.0f, 1.0f,
		195.0f, 75.0f, 0.0f, 1.0f,

		185.0f, 100.0f, 0.0f, 1.0f,
		195.0f, 100.0f, 0.0f, 1.0f,

		// O
		205.0f, 125.0f, 0.0f, 1.0f,
		205.0f, 75.0f, 0.0f, 1.0f,

		205.0f, 75.0f, 0.0f, 1.0f,
		215.0f, 75.0f, 0.0f, 1.0f,

		215.0f, 75.0f, 0.0f, 1.0f,
		215.0f, 125.0f, 0.0f, 1.0f,

		215.0f, 125.0f, 0.0f, 1.0f,
		205.0f, 125.0f, 0.0f, 1.0f,

		// V
		220.0f, 125.0f, 0.0f, 1.0f,
		225.0f, 75.0f, 0.0f, 1.0f,

		225.0f, 75.0f, 0.0f, 1.0f,
		230.0f, 125.0f, 0.0f, 1.0f,

		// E
		245.0f, 125.0f, 0.0f, 1.0f,
		235.0f, 125.0f, 0.0f, 1.0f,

		235.0f, 125.0f, 0.0f, 1.0f,
		235.0f, 75.0f, 0.0f, 1.0f,

		235.0f, 75.0f, 0.0f, 1.0f,
		245.0f, 75.0f, 0.0f, 1.0f,

		235.0f, 100.0f, 0.0f, 1.0f,
		245.0f, 100.0f, 0.0f, 1.0f,

		// R
		250.0f, 75.0f, 0.0f, 1.0f,
		250.0f, 125.0f, 0.0f, 1.0f,

		250.0f, 125.0f, 0.0f, 1.0f,
		260.0f, 125.0f, 0.0f, 1.0f,

		260.0f, 125.0f, 0.0f, 1.0f,
		260.0f, 100.0f, 0.0f, 1.0f,

		260.0f, 100.0f, 0.0f, 1.0f,
		250.0f, 100.0f, 0.0f, 1.0f,

		250.0f, 100.0f, 0.0f, 1.0f,
		260.0f, 75.0f, 0.0f, 1.0f,

		// YOU WIN message
		// Y
		155.0f, 125.0f, 0.0f, 1.0f,
		160.0f, 100.0f, 0.0f, 1.0f,

		160.0f, 100.0f, 0.0f, 1.0f,
		165.0f, 125.0f, 0.0f, 1.0f,

		160.0f, 100.0f, 0.0f, 1.0f,
		160.0f, 75.0f, 0.0f, 1.0f,

		// O
		170.0f, 125.0f, 0.0f, 1.0f,
		170.0f, 75.0f, 0.0f, 1.0f,

		170.0f, 75.0f, 0.0f, 1.0f,
		180.0f, 75.0f, 0.0f, 1.0f,

		180.0f, 75.0f, 0.0f, 1.0f,
		180.0f, 125.0f, 0.0f, 1.0f,

		180.0f, 125.0f, 0.0f, 1.0f,
		170.0f, 125.0f, 0.0f, 1.0f,

		// U
		185.0f, 125.0f, 0.0f, 1.0f,
		185.0f, 75.0f, 0.0f, 1.0f,

		185.0f, 75.0f, 0.0f, 1.0f,
		195.0f, 75.0f, 0.0f, 1.0f,

		195.0f, 75.0f, 0.0f, 1.0f,
		195.0f, 125.0f, 0.0f, 1.0f,

		// W
		205.0f, 125.0f, 0.0f, 1.0f,
		207.5f, 75.0f, 0.0f, 1.0f,

		207.5f, 75.0f, 0.0f, 1.0f,
		210.0f, 125.0f, 0.0f, 1.0f,

		210.0f, 125.0f, 0.0f, 1.0f,
		212.5f, 75.0f, 0.0f, 1.0f,

		212.5f, 75.0f, 0.0f, 1.0f,
		215.0f, 125.0f, 0.0f, 1.0f,

		// I
		225.0f, 125.0f, 0.0f, 1.0f,
		225.0f, 75.0f, 0.0f, 1.0f, 

		// N
		235.0f, 75.0f, 0.0f, 1.0f,
		235.0f, 125.0f, 0.0f, 1.0f,

		235.0f, 125.0f, 0.0f, 1.0f,
		245.0f, 75.0f, 0.0f, 1.0f,

		245.0f, 75.0f, 0.0f, 1.0f,
		245.0f, 125.0f, 0.0f, 1.0f,

		// car3
		180.0f, 85.0f, 0.0f, 1.0f,
		180.0f, 115.0f, 0.0f, 1.0f,
		220.0f, 115.0f, 0.0f, 1.0f,
		220.0f, 85.0f, 0.0f, 1.0f,



	};

	// culorile varfurilor din colturi
	GLfloat Colors[] = {
		// middle of the road
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		// car 1
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		// car 2
		1.0f, 0.65f, 0.0f, 1.0f,
		1.0f, 0.65f, 0.0f, 1.0f,
		1.0f, 0.65f, 0.0f, 1.0f,
		1.0f, 0.65f, 0.0f, 1.0f,

		

	};

	// se creeaza un buffer nou
	glGenBuffers(1, &VboId);
	// este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	// 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	// un nou buffer, pentru culoare
	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	// atributul 1 =  culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void CreateShaders(void)
{
	ProgramId = LoadShaders("03_04_Shader.vert", "03_04_Shader.frag");
	glUseProgram(ProgramId);
}
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void Initialize(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); 
	CreateVBO();
	CreateShaders();
	codColLocation = glGetUniformLocation(ProgramId, "codCuloare");
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	resizeMatrix = glm::ortho(float(0), width, float(0), height);
}
void RenderFunction(void)
{

	
	//matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, 0.0));
	//matrRot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 0.0, 1.0));
	//matrTransl2 = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0.0, 0.0)); // controleaza translatia de-a lungul lui Ox
	// car 1
	matrTranslCar1 = glm::translate(glm::mat4(1.0f), glm::vec3(iCar1, 0.0, 0.0));

	//car 2
	matrTranslCar2 = glm::translate(glm::mat4(1.0f), glm::vec3(iCar2 + xCar2, yCar2, 0.0));

	// car 3
	GLfloat angleCar3 = PI;
	matrRotCar3 = glm::rotate(glm::mat4(1.0f), angleCar3, glm::vec3(0.0, 0.0, 1.0));
	//matrTranslInainteRotCar3 = glm::translate(glm::mat4(1.0), glm::vec3(240.f, 200.f, 0.0));
	matrTranslInitPosCar3 = glm::translate(glm::mat4(1.0f), glm::vec3(-10.f, 40.f, 0.0));
	matrFixRotation3 = glm::translate(glm::mat4(1.0f), glm::vec3(240.f, 155.f, 0.0));
	matrTranslCar3 = glm::translate(glm::mat4(1.0f), glm::vec3(iCar3, 0.0, 0.0));
	glClear(GL_COLOR_BUFFER_BIT);
	myMatrix = resizeMatrix;


	// Matricea de redimensionare (pentru elementele "fixe")
	// middle of the road
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glLineWidth(20);
	glDrawArrays(GL_LINES, 0, 12);

	// Matricea pentru elementele care isi schimba pozitia
	// car 1
	//if (stopGame == false) {
		myMatrix = resizeMatrix * matrTranslCar1;
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		codCol = 1;
		glUniform1i(codColLocation, codCol);

		// car 1 tow
		glDrawArrays(GL_POLYGON, 48, 4);

		// car 1 front part
		glDrawArrays(GL_POLYGON, 52, 4);

		// car 1 part between tow and front part
		codCol = 4;
		glUniform1i(codColLocation, codCol);
		glDrawArrays(GL_POLYGON, 56, 4);

		// car 1 windscreen
		glDrawArrays(GL_POLYGON, 60, 4);

		// car 1 top window
		glDrawArrays(GL_POLYGON, 64, 4);

		// car 1 down window
		glDrawArrays(GL_POLYGON, 68, 4);

		// car 1 top headlight
		codCol = 5;
		glUniform1i(codColLocation, codCol);
		glDrawArrays(GL_POLYGON, 72, 4);

		// car 1 down headlight
		glDrawArrays(GL_POLYGON, 76, 4);

		
	

	// car 2 
	myMatrix = resizeMatrix * matrTranslCar2;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	codCol = 9;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 12, 4);
	codCol = 4;
	glUniform1i(codColLocation, codCol);

	//rear window
	glDrawArrays(GL_POLYGON, 16, 4);

	// windscreen
	glDrawArrays(GL_POLYGON, 20, 4);

	// top window
	glDrawArrays(GL_POLYGON, 24, 4);

	// down window
	glDrawArrays(GL_POLYGON, 28, 4);

	// top headlight
	codCol = 5;
	glUniform1i(codColLocation, codCol);
	if (isSignalTurned && pauseSignal == false) {
		codCol = 2;
		glUniform1i(codColLocation, codCol);
	}
	else {
		codCol = 5;
		glUniform1i(codColLocation, codCol);
	}
	if (isSignalOn && pauseSignal == false) {
		currentTime = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= 0.001) {
			isSignalTurned = ~isSignalTurned;
			startTime = currentTime;

			if (isSignalTurned) {
				codCol = 8;
				glUniform1i(codColLocation, codCol);
			}
			else {
				codCol = 5;
				glUniform1i(codColLocation, codCol);
			}
		}
		
	}
	else {
		codCol = 5;
		glUniform1i(codColLocation, codCol);
	}
	
	glDrawArrays(GL_POLYGON, 32, 4);

	// down headlight
	codCol = 5;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 36, 4);

	// top rear light
	codCol = 6;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 40, 4);

	// down rear light
	glDrawArrays(GL_POLYGON, 44, 4);


	// car3
	myMatrix = resizeMatrix * matrTranslCar3 * matrTranslInitPosCar3 * matrFixRotation3 * matrRotCar3 ;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 12, 4);
	codCol = 4;
	glUniform1i(codColLocation, codCol);

	//rear window
	glDrawArrays(GL_POLYGON, 16, 4);

	// windscreen
	glDrawArrays(GL_POLYGON, 20, 4);

	// top window
	glDrawArrays(GL_POLYGON, 24, 4);

	// down window
	glDrawArrays(GL_POLYGON, 28, 4);

	// top headlight
	codCol = 5;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 32, 4);

	// down headlight
	glDrawArrays(GL_POLYGON, 36, 4);

	// top rear light
	codCol = 6;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 40, 4);

	// down rear light
	glDrawArrays(GL_POLYGON, 44, 4);


	// GAME OVER message
	if (isGameLost == true) {
		myMatrix = resizeMatrix;
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		glLineWidth(5);

		// G
		codCol = 6;
		glUniform1i(codColLocation, codCol);
		glDrawArrays(GL_LINES, 80, 10);

		// A
		glDrawArrays(GL_LINES, 90, 6);

		// M
		glDrawArrays(GL_LINES, 96, 8);

		// E
		glDrawArrays(GL_LINES, 104, 8);

		// O
		glDrawArrays(GL_LINES, 112, 8);

		// V
		glDrawArrays(GL_LINES, 120, 4);

		// E
		glDrawArrays(GL_LINES, 124, 8);

		// R
		glDrawArrays(GL_LINES, 132, 10);
	}

	// YOU WIN message
	if (isGameWon) {
		myMatrix = resizeMatrix;
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		codCol = 7;
		glUniform1i(codColLocation, codCol);

		glLineWidth(5);
		// Y
		glDrawArrays(GL_LINES, 142, 6);

		// O
		glDrawArrays(GL_LINES, 148, 8);

		// U
		glDrawArrays(GL_LINES, 156, 6);

		// W
		glDrawArrays(GL_LINES, 162, 8);

		// I
		glDrawArrays(GL_LINES, 170, 2);

		// N
		glDrawArrays(GL_LINES, 172, 6);
	}
	
	
	
	glutSwapBuffers();
	glFlush();
}
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 400);
	glutCreateWindow("Overtake");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(moveCarsByDefault);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	/*glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);*/
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

