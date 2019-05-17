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
static GLfloat viewer[] = { 12.0, 7.0, 12.0 };

int depth;
GLfloat x, y, z, h, side;

typedef GLfloat vec[3];

//Iloczyn wektorowy
vec* VectorProduct(vec vec1, vec vec2)
{
	vec result;
	result[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	result[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	result[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
	return &result;
}

//Rysowanie pojedynczej piramidy
void DrawPyramid(GLfloat x, GLfloat y, GLfloat z, GLfloat side)
{
	glBegin(GL_TRIANGLES);

	vec vec1 = { x - (x + side / 2), y - (y - side * sqrt(3) / 2), z - (z + side / 2) };
	vec vec2 = { x - (x + side / 2), y - (y - side * sqrt(3) / 2), z - (z - side / 2) };
	vec* vec3 = VectorProduct(vec1, vec2);
	glNormal3f(*vec3[0], *vec3[1], *vec3[2]);

	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x, y, z);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x + side / 2, y - side * sqrt(3) / 2, z + side / 2);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x + side / 2, y - side * sqrt(3) / 2, z - side / 2);
	
	glEnd();

	glBegin(GL_TRIANGLES);
	
	vec1[2] = z - (z - side / 2);
	vec2[0] = x - (x - side / 2);
	vec3 = VectorProduct(vec1, vec2);
	glNormal3f(*vec3[0], *vec3[1], *vec3[2]);

	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x, y, z);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x + side / 2, y - side * sqrt(3) / 2, z - side / 2);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x - side / 2, y - side * sqrt(3) / 2, z - side / 2);

	glEnd();

	glBegin(GL_TRIANGLES);

	vec1[0] = x - (x - side / 2);
	vec2[2] = z - (z + side / 2);
	vec3 = VectorProduct(vec1, vec2);
	//NormalizeVector(*vec3);
	glNormal3f(*vec3[0], *vec3[1], *vec3[2]);

	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x, y, z);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x - side / 2, y - side * sqrt(3) / 2, z - side / 2);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x - side / 2, y - side * sqrt(3) / 2, z + side / 2);

	glEnd();

	glBegin(GL_TRIANGLES);

	vec1[2] = z - (z + side / 2);
	vec2[0] = x - (x + side / 2);
	vec3 = VectorProduct(vec1, vec2);
	glNormal3f(*vec3[0], *vec3[1], *vec3[2]);

	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x, y, z);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x - side / 2, y - side * sqrt(3) / 2, z + side / 2);
	glColor3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	glVertex3f(x + side / 2, y - side * sqrt(3) / 2, z + side / 2);

	glEnd();

	//Rysowanie podstawy
	glBegin(GL_POLYGON);

	vec1[0] = x + side / 2 - (x + side / 2);
	vec1[1] = y - side * sqrt(3) / 2 - (y - side * sqrt(3) / 2);
	vec1[2] = z + side / 2 - (z - side / 2);
	vec2[0] = x + side / 2 - (x - side / 2);
	vec2[1] = vec1[1];
	vec2[2] = z + side / 2 - (z - side / 2);
	vec3 = VectorProduct(vec1, vec2);
	glNormal3f(*vec3[0], *vec3[1], *vec3[2]);

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
void DrawSierpinskiPyramid(GLfloat x, GLfloat y, GLfloat z, GLfloat side, int depth)
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
		r_old = y;
		status = 1;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;
		y_pos_old = y;
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
		r += delta_r * pix2anglev;
	}
	if (status == 2)
	{
		thetax += delta_x*pix2angleh*0.1;
		thetay += delta_y*pix2anglev*0.1;
		r += delta_r * pix2anglev;
	}

	GLfloat lightposition[4];

	lightposition[0] = r * cos(thetax) * cos(thetay);
	lightposition[1] = r * sin(thetay);
	lightposition[2] = r * sin(thetax) * cos(thetay);
	lightposition[3] = 1.0;

	if(status == 1) glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
	else if (status == 2) glLightfv(GL_LIGHT1, GL_POSITION, lightposition);

	y = side * sqrt(3) / 2 * 2 / 3;
	DrawSierpinskiPyramid(x, y, z, side, depth);
	glFlush();
	glutSwapBuffers();
}

void MyInit()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	GLfloat mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 20.0 };

	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse1[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat light_diffuse2[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light_specular2[] = { 0.7, 0.7, 1.0, 1.0 };
	GLfloat light_position1[] = { -10.0, 15.0, -10.0 };
	GLfloat light_position2[] = { 10.0, 15.0, 10.0 };	
	GLfloat att_constant = 1.0;
	GLfloat att_linear = 0.05;
	GLfloat att_quadratic = 0.001;

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
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
	glutCreateWindow("Oswietlenie");
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}