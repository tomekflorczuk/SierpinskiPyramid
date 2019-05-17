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

static GLfloat r = 15.0;
static GLfloat delta_r = 0.0;
static GLfloat r_old = 0.0;

static GLfloat thetax = 0.0;
static GLfloat thetay = 0.0;
static GLfloat pix2angleh;
static GLfloat pix2anglev;
static GLint status = 0;
static int x_pos_old, y_pos_old = 0;
static int delta_x, delta_y = 0;
static GLfloat viewer[] = { 0.0, 0.0, 15.0 };

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
	//glNormal3fv();
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
	if (depth > 1)
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

void Axes()
{
	point  x_min = { -5.0, 0.0, 0.0 };
	point  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point  y_min = { 0.0, -5.0, 0.0 };
	point  y_max = { 0.0,  5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point  z_min = { 0.0, 0.0, -5.0 };
	point  z_max = { 0.0, 0.0,  5.0 };
	//  pocz?tek i koniec obrazu osi y
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

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;               
		y_pos_old = y;
		status = 1;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		r_old = y;
		status = 2;
	}
	else status = 0;          
}

void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;     
	delta_y = y - y_pos_old;
	delta_r = y - r_old;
	x_pos_old = x;            
	y_pos_old = y;
	r_old = y;
	glutPostRedisplay();     
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	Axes();
	if (status == 1)        
	{
		thetax += delta_x*pix2angleh*0.1;
		thetay += delta_y*pix2anglev*0.1;
	}                                  
	if (status == 2)
	{
		r += delta_r * pix2anglev;
	}
	
	viewer[0] = r * cos(thetax) * cos(thetay);
	viewer[1] = r * sin(thetay);
	viewer[2] = r * sin(thetax) * cos(thetay);

	y = side * sqrt(3) / 2 * 2 / 3;
	DrawSierpinskiPyramid(x, y, z, side, depth);
	glFlush();
	glutSwapBuffers();
}

void MyInit()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angleh = 360.0 / (float)horizontal;
	pix2anglev = 360.0 / (float)vertical;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1.0, 1.0, 30.0);
	
	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

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
	glutCreateWindow("Rzutowanie perspektywiczne");
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}