#define _USE_MATH_DEFINES
#include <windows.h>
#include <gl/gl.h>
#include <glut.h>
#include <iostream>

using namespace std;

struct point
{
	float x;
	float y;
	float z;
	int r;
	int g;
	int b;
};
static GLfloat theta[] = { 0.0, 0.0, 0.0 };

int depth;
float x, y, z, h, side;

//Rysowanie pojedynczej piramidy
void DrawPyramid(float x, float y, float z, float side)
{
	glBegin(GL_POLYGON);

	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x, y, z);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x + side / 2, y - side * sqrt(3) / 2, z + side / 2);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x + side / 2, y - side * sqrt(3) / 2, z - side / 2);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x - side / 2, y - side * sqrt(3) / 2, z - side / 2);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x - side / 2, y - side * sqrt(3) / 2, z + side / 2);
	
	glEnd();

	//Rysowanie podstawy
	glBegin(GL_POLYGON);

	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x + side / 2, y - side * sqrt(3) / 2, z + side / 2);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x + side / 2, y - side * sqrt(3) / 2, z - side / 2);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x - side / 2, y - side * sqrt(3) / 2, z - side / 2);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x - side / 2, y - side * sqrt(3) / 2, z + side / 2);

	glEnd();
}

//Funckja rekurencyjna
void DrawSierpinskiPyramid(float x, float y, float z, float side, int depth)
{		
	if(depth > 1)
	{
		side = side / 2;

		DrawSierpinskiPyramid(x, y, z, side, depth - 1);
		DrawSierpinskiPyramid(x + side / 2, y - side * sqrt(3) / 2, z + side / 2, side, depth - 1);
		DrawSierpinskiPyramid(x + side / 2, y - side * sqrt(3) / 2, z - side / 2, side, depth - 1);
		DrawSierpinskiPyramid(x - side / 2, y - side * sqrt(3) / 2, z - side / 2, side, depth - 1);
		DrawSierpinskiPyramid(x - side / 2, y - side * sqrt(3) / 2, z + side / 2, side, depth - 1);
	}
	else
	{
		DrawPyramid(x, y, z, side);
	}
}

//Obkrêcanie
void spinPiramid()
{
	theta[0] -= 0.5;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.5;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.5;
	if (theta[2] > 360.0) theta[2] -= 360.0;

	glutPostRedisplay();
}

//Osie wspó³rzêdnych
void Axes()
{
	point  x_min = { -5.0, 0.0, 0.0 };
	point  x_max = { 5.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	point  y_min = { 0.0, -5.0, 0.0 };
	point  y_max = { 0.0,  5.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	point  z_min = { 0.0, 0.0, -5.0 };
	point  z_max = { 0.0, 0.0,  5.0 };
	//  pocz¹tek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3f(x_min.x, x_min.y, x_min.z);
	glVertex3f(x_max.x, x_max.y, x_max.z);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3f(y_min.x, y_min.y, y_min.z);
	glVertex3f(y_max.x, y_max.y, y_max.z);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3f(z_min.x, z_min.y, z_min.z);
	glVertex3f(z_max.x, z_max.y, z_max.z);
	glEnd();
}

//Funkcja rysuj¹ca
void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	Axes();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);
	y = side * sqrt(3) / 2 * 2 / 3;
	DrawSierpinskiPyramid(x, y, z, side, depth);
	glFlush();
	glutSwapBuffers();
}

//Funkcja ustalaj¹ca stan renderowania
void MyInit()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//Funkcja skaluj¹ca
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	GLfloat AspectRatio;
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();           
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;    
	if (horizontal <= vertical)
		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else
		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main()
{
	cout << "Podaj glebokosc: ";
	cin >> depth;
	cout << "Podaj dlugosc boku: ";
	cin >> side;
	
	h = side * sqrt(3) / 2;
	cout << h << endl;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(300, 300);
	glutCreateWindow("PIRAMIDA");
	glutIdleFunc(spinPiramid);
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);   
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}