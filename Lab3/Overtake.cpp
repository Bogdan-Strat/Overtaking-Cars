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

int codCol, stopGame = false;
float PI = 3.141592, angle = 0;
float xCar2 = 0; float yCar2 = 0; 
float width = 400, height = 200;
float iCar1 = 0, stepCar1 = 0.0025;
float iCar2 = 0, stepCar2 = 0.0025;
glm::mat4 
	myMatrix, resizeMatrix, matrTranslCar1, matrTranslCar2;

void draw() {
	//glutLeaveMainLoop();
	//glutDestroyWindow(glutGetWindow());
	//glutCreateWindow("Game Over");
}

void moveCarsByDefault(void) {
	if (stopGame == false) {
		iCar1 += stepCar1;
		iCar2 += stepCar2;
	}
	
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key) {
	case 'l':
		angle += 0.2f;
		break;
	case 'r':
		angle -= 0.2f;
		break;
	}
	if (key == 27)
		exit(0);
}
void processSpecialKeys(int key, int xx, int yy) 
{
	int len;
	if (stopGame == false) {
		switch (key)
		{
		case GLUT_KEY_LEFT:
			xCar2 -= 5;
			break;
		case GLUT_KEY_RIGHT:
			xCar2 += 5;
			break;
		case GLUT_KEY_UP:
			//stopGame = true;
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

		// car 1
		75.0f, 30.0f, 0.0f, 1.0f,
		75.0f, 80.0f, 0.0f, 1.0f,
		155.0f, 80.0f, 0.0f, 1.0f,
		155.0f, 30.0f, 0.0f, 1.0f,

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

		// car3



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
	// Desenare puncte din colturi si axe
			/*codCol = 0;
			glUniform1i(codColLocation, codCol);
			glPointSize(10.0);
			glDrawArrays(GL_POINTS, 0, 4);
			glDrawArrays(GL_LINES, 4, 4);*/

	// Matricea pentru elementele care isi schimba pozitia
	// car 1
	//if (stopGame == false) {
		myMatrix = resizeMatrix * matrTranslCar1;
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		codCol = 1;
		glUniform1i(codColLocation, codCol);
		glDrawArrays(GL_POLYGON, 12, 4);
	//}
	

	// car 2 
	myMatrix = resizeMatrix * matrTranslCar2;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 16, 4);
	codCol = 4;
	glUniform1i(codColLocation, codCol);

	//rear window
	glDrawArrays(GL_POLYGON, 20, 4);

	// windscreen
	glDrawArrays(GL_POLYGON, 24, 4);

	// top window
	glDrawArrays(GL_POLYGON, 28, 4);

	// down window
	glDrawArrays(GL_POLYGON, 32, 4);

	// top headlight
	codCol = 5;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 36, 4);

	// down headlight
	glDrawArrays(GL_POLYGON, 40, 4);

	// top rear light
	codCol = 6;
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 44, 4);

	// down rear light
	glDrawArrays(GL_POLYGON, 48, 4);

			//myMatrix = resizeMatrix * matrTransl * matrRot;
			//glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
			//// Desenare patrat si punct mobil
			//codCol = 1;
			//glUniform1i(codColLocation, codCol);
			//glDrawArrays(GL_POLYGON, 8, 4);
			//codCol = 2;
			//glUniform1i(codColLocation, codCol);
			//glEnable(GL_POINT_SMOOTH);
			//glDrawArrays(GL_POINTS, 12, 1);
			//glDisable(GL_POINT_SMOOTH);


	
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
	glutSpecialFunc(processSpecialKeys);
	/*glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);*/
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

