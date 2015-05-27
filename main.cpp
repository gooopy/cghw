// osx
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#include <GLUT/GLUT.h>



// window
#include <windows.h>	   // Standard header for MS Windows applications
#include <GL/gl.h>		   // Open Graphics Library (OpenGL) header
#include <GL/glut.h>	   // The GL Utility Toolkit (GLUT) Header


#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

using namespace std;

typedef struct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;

} material;

material PlaneMaterial = {{0.6,0.6,0.6,0.6},
						  {0.0,0.0,0.0,0.0},
						  {0.0,0.0,0.0,0.0},
						  0};

void setMaterial(material M);


/* DEFINE */


const float PI = 3.14159265;
const float UNITANGLE = 5;
const float COSTHETA = cos(UNITANGLE * PI / 180);
const float SINTHETA = sin(UNITANGLE * PI / 180);

const int NUM_OF_THINGS = 50;
const int NUM_OF_SHAPES = 4;
const int SIZE_OF_WORLD = 250;		// Window size -- 500x500;
									// WORLD -- 500x500 plane

const int SPHERE = 1;
const int TEAPOT = 2;
const int TORUS = 3;
const int CUBE = 4;
const int CONE = 5;




/* Global Variables */

bool BIRDSEYEVIEW = false;
bool SELECTMODE = false;
bool LIGHTMOVING = false;

float cameraX = 0;
float cameraY = 0;
float cameraZ = 0;

float lightX =0;
float lightY = 50;
float lightZ = 250;

int cameraDirection = 0;
float directionX = 0;
float directionZ = 1;

int mouseX, mouseY;
//int winH = 500;

bool tempColor = 1;



/* class Definition */

class Thing {
public:
	int shape;		// shape type
	GLfloat x, y, z;		// x, z -- position (y = 0)
	GLfloat color[3];	// color (random generation)

	material mat;

	GLfloat savedColor[3];
	float angle;		// angle
	float velocity;	// how fast rotation is
	float size;

	float acc;
	float savedAcc;

	bool isSelected;

	Thing();
	void put();
	void setColor(GLfloat, GLfloat, GLfloat);
	void setColorDefault();
	double distance(double, double);
	void updateMaterial();

	void avatarSet(int, int);
};

Thing::Thing() {

	x = (rand() % 5000)/10 - 250;
	y = 0;
	z = (rand() % 5000)/10 - 250;
	shape = (rand() % NUM_OF_SHAPES) +1;

	angle = 0;
	velocity = (rand() % 100)/10  + 1;

	acc = (rand() % 50)/10 + 10;
	savedAcc = acc;

	size = (rand() % 100)/10 + 5;

	isSelected = false;


	color[0] = (float)(rand()%5000 + 500) / 10000;
	color[1] = (float)(rand()%5000 + 500) / 10000;
	color[2] = (float)(rand()%5000 + 500) / 10000;

	material temp = {{color[0], color[1], color[2], 1.0},
							 {color[0], color[1], color[2], 1.0},
							 {color[0], color[1], color[2], 1.0},
							 8.2};

	mat = temp;

	for(int i=0; i<3; i++){
		savedColor[i] = color[i];
	}





}

void Thing::updateMaterial() {
	material temp = {{color[0], color[1], color[2], 1.0},
		  {color[0], color[1], color[2], 1.0},
		  {color[0], color[1], color[2], 1.0},
		  8.2};

	mat = temp;

}

void Thing::put() {
	/*
	 * GENERATION and PUT the Things
	 */


	setMaterial(mat);

	glPushMatrix();
	glColor3f(color[0], color[1], color[2]);





	glTranslatef(x, y, z);

	//
	// Rotation, but not yet individual rotation (velocity)
	//
	glRotatef(angle, 0,1,0);
	angle = angle + velocity;





	// yet individual size ( all the same now )
	if(shape == SPHERE) {
		glutSolidSphere(size,10,10);
	} else if(shape == TEAPOT) {
		glutSolidTeapot(size);
	} else if(shape == TORUS) {
		glutSolidTorus(size/2, size, 10, 10);
	} else if(shape == CUBE) {
		glutSolidCube(size);
	} else if(shape == CONE) {
		glutSolidCone(size, size, 5, 5);
	}

	glPopMatrix();
}

void Thing::setColor(GLfloat i, GLfloat j, GLfloat k) {
	color[0] = i;
	color[1] = j;
	color[2] = k;
}

void Thing::setColorDefault() {
	color[0] = savedColor[0];
	color[1] = savedColor[1];
	color[2] = savedColor[2];
}



double Thing::distance(double a, double b) {
	return sqrt((float)(a-x)*(a-x) + (b-z)*(b-z));
}

void Thing::avatarSet(int a, int b) {
	x = a;
	z = b;
}

Thing* ThingPointer[NUM_OF_THINGS];
Thing* isSelected;
bool hasSelected = false;






void setMaterial(material M)
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, M.specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, M.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, M.diffuse);
	//glMaterialf(GL_FRONT, GL_SHININESS, M.shininess);
}


/* init */
void init(void)
{
	// setup opengl state
	glClearColor(0,0,0,0.0);
	//glColor3f(0.9,0.9,0.9);


	for(int i=0; i<NUM_OF_THINGS; i++) {
		ThingPointer[i] = new Thing();
	}



	// ThingPointer[0] is Avatar
	ThingPointer[0]->avatarSet(cameraX, cameraZ);
	ThingPointer[0]->shape = TEAPOT;
	ThingPointer[0]->setColor(1,1,1);
	ThingPointer[0]->size = 10;
	ThingPointer[0]->velocity = 0;
	ThingPointer[0]->y = 30;
	ThingPointer[0]->angle = -90;
	ThingPointer[0]->updateMaterial();


	//ThingPointer[NUM_OF_THINGS - 1] is light
	ThingPointer[NUM_OF_THINGS - 1]->avatarSet(lightX, lightZ);
	ThingPointer[NUM_OF_THINGS - 1]->shape = SPHERE;
	ThingPointer[NUM_OF_THINGS - 1]->setColor(1,1,1);
	ThingPointer[NUM_OF_THINGS - 1]->size = 10;
	ThingPointer[NUM_OF_THINGS - 1]->velocity = 0;
	ThingPointer[NUM_OF_THINGS - 1]->y = lightY;
	ThingPointer[NUM_OF_THINGS - 1]->updateMaterial();


	/* 100% white light */
	GLfloat light_ambient[]={0.3, 0.3, 0.3, 0.5};
	GLfloat light_diffuse[]={0.5, 0.5, 0.5, 0.5};
	GLfloat light_specular[]={0.5, 0.5, 0.5, 0.5};

	/* set up ambient, diffuse, and specular components for light 0 */
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	/* set up ambient, diffuse, and specular components for light 0 */
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);


	glClearColor (0.0, 0.0, 0.0, 1.0);	// World Color is Black like 'SPACE'
	glShadeModel (GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);






}

/* Display Function */ // this function will be recall again and again ALWAYS (please don't forget)
void display()
{
	static float rot=0;
	static float rotL=0;
	static float position[4] = {0,2,0,1}; /* point light at (0,2,0) */
	static float position2[4] = {0, 0, 0, 1};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Setup the view of the world */
	glMatrixMode(GL_PROJECTION);           // Switch to projection mode (P)
	glLoadIdentity();                      // P <- I

	if(BIRDSEYEVIEW) {
		glOrtho(-252,252,-252,252,1.0,2000.0);
		//glFrustum(-250,250,-250,250,1.0,2000.0);
		//gluPerspective(14.03624347, 1.0, 999.9, 3000.0); // P <- P
	} else {
		gluPerspective(30.0, 1.0, 1.0, 3000.0);
	}



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(BIRDSEYEVIEW) {
		gluLookAt(0, 1000, 0, 0,0,0, 0,0,1);    // Bird's eye View
	} else {
		gluLookAt(cameraX,cameraY,cameraZ, cameraX+directionX,0,cameraZ+directionZ, 0,1,0);

	}



	// GENERATE a ground PLANE (y < 0)
	// plane y=0 / plane x=100, x=-100 / plane z=100, z=-100
	//
	setMaterial(PlaneMaterial);


	glBegin(GL_POLYGON);
		glColor3f(0.8,0.8,0.8);
		glVertex3f(-SIZE_OF_WORLD,0,SIZE_OF_WORLD);
		glVertex3f(-SIZE_OF_WORLD,0,-SIZE_OF_WORLD);
		glVertex3f(SIZE_OF_WORLD,0,-SIZE_OF_WORLD);
		glVertex3f(SIZE_OF_WORLD,0,SIZE_OF_WORLD);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(-SIZE_OF_WORLD,0,SIZE_OF_WORLD);
		glVertex3f(-SIZE_OF_WORLD,-SIZE_OF_WORLD,SIZE_OF_WORLD);
		glVertex3f(SIZE_OF_WORLD,-SIZE_OF_WORLD,SIZE_OF_WORLD);
		glVertex3f(SIZE_OF_WORLD,0,SIZE_OF_WORLD);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(SIZE_OF_WORLD,0,SIZE_OF_WORLD);
		glVertex3f(SIZE_OF_WORLD,-SIZE_OF_WORLD,SIZE_OF_WORLD);
		glVertex3f(SIZE_OF_WORLD,-SIZE_OF_WORLD,-SIZE_OF_WORLD);
		glVertex3f(SIZE_OF_WORLD,0,-SIZE_OF_WORLD);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(-SIZE_OF_WORLD, 0,SIZE_OF_WORLD);
		glVertex3f(-SIZE_OF_WORLD,-SIZE_OF_WORLD,SIZE_OF_WORLD);
		glVertex3f(-SIZE_OF_WORLD,-SIZE_OF_WORLD,-SIZE_OF_WORLD);
		glVertex3f(-SIZE_OF_WORLD,0,-SIZE_OF_WORLD);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(-SIZE_OF_WORLD,0,-SIZE_OF_WORLD);
		glVertex3f(-SIZE_OF_WORLD,-SIZE_OF_WORLD,-SIZE_OF_WORLD);
		glVertex3f(SIZE_OF_WORLD,-SIZE_OF_WORLD,-SIZE_OF_WORLD);
		glVertex3f(SIZE_OF_WORLD,0,-SIZE_OF_WORLD);
	glEnd();




	/*
	 *  put()
	 */

	// Avatar and Light -- updating their position to camera and lighting position
	ThingPointer[0]->avatarSet(cameraX, cameraZ);
	ThingPointer[NUM_OF_THINGS -1]->avatarSet(lightX, lightZ);

	for(int i=0; i<NUM_OF_THINGS; i++) {
		ThingPointer[i]->put();
	}

	for(int i=1; i<NUM_OF_THINGS-1; i+=4) {
		ThingPointer[i]->y += ThingPointer[i]->acc/2;
		ThingPointer[i]->acc -= 2;

		if(ThingPointer[i]->y < 0) ThingPointer[i]->acc = ThingPointer[i]->savedAcc;
	}


	glLightfv(GL_LIGHT0, GL_POSITION, position);





	/*
	 *   Light
	 */
	glPushMatrix();
	glTranslatef(lightX, lightY, lightZ);
	glLightfv(GL_LIGHT1, GL_POSITION, position2); /* set light defined above */
	glPopMatrix();




	//

	if(!SELECTMODE & isSelected!=NULL){
		isSelected->setColorDefault();
		isSelected->updateMaterial();
		isSelected = NULL;
	}

	glutSwapBuffers();


}



// SETUP KEYBOARD
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

		case ' ':		// Switch Bird's eye view
			BIRDSEYEVIEW = !BIRDSEYEVIEW;	// toggle
			if(!BIRDSEYEVIEW & SELECTMODE) SELECTMODE = false;
			break;

		case 's':
			if(BIRDSEYEVIEW) SELECTMODE = !SELECTMODE;
			if(LIGHTMOVING) LIGHTMOVING = !LIGHTMOVING;
			break;

		case 'l':
			LIGHTMOVING = !LIGHTMOVING;
			break;

		case 'd':
			if(LIGHTMOVING) {
				lightX = 0;
				lightZ = 0;
			} else if(SELECTMODE) {
				isSelected->x =0;
				isSelected->z =0;
			} else {
				cameraX = 0;
				cameraZ = 0;
			}
			break;

		case 27:			// 'ESC' -- quit
			exit(0);

		default:
			break;
	}
}

// SETUP KEYBOARD
void arrow(int key, int x, int y)
{
	switch (key)
	{


		case GLUT_KEY_LEFT:		// left key
			if(LIGHTMOVING) {
				lightX += 10;
			} else {

				if (SELECTMODE) {
					if (hasSelected) {
						isSelected->x += 10;
					}
				} else {
					/* Camera Moving to the left*/

					float tempX = directionX;
					float tempZ = directionZ;

					directionX = tempX * COSTHETA + tempZ * SINTHETA;
					directionZ = tempZ * COSTHETA - tempX * SINTHETA;

					ThingPointer[0]->angle += UNITANGLE;

				}
			}
			break;

		case GLUT_KEY_RIGHT:		// right key
			if(LIGHTMOVING) {
				lightX -= 10;
			} else {

				if (SELECTMODE) {
					if (hasSelected) {
						isSelected->x -= 10;
					}
				} else {
					/* Camera Moving to the right*/

					float tempX = directionX;
					float tempZ = directionZ;

					directionX = tempX * COSTHETA - tempZ * SINTHETA;
					directionZ = tempX * SINTHETA + tempZ * COSTHETA;

					ThingPointer[0]->angle -= UNITANGLE;
				}
			}

			break;

		case GLUT_KEY_UP:		// up key
			if(LIGHTMOVING) {
				lightZ += 10;
			} else {
				if (SELECTMODE) {
					if (isSelected != NULL) {
						isSelected->z += 10;
					}
				} else {
					/* camera Moving */
					cameraX += directionX * 3;
					cameraZ += directionZ * 3;
				}
			}


			break;

		case GLUT_KEY_DOWN:		//down key
			if(LIGHTMOVING) {
				lightZ -= 10;
			} else {
				if (SELECTMODE) {
					if (isSelected != NULL) {
						isSelected->z -= 10;
					}
				} else {
					/* camera Moving */
					cameraX -= directionX * 3;
					cameraZ -= directionZ * 3;
				}
			}
			break;


		default:
			break;
	}
}

// Idle call back
void idle()
{
	glutPostRedisplay();
}




// MousePress: Called when mouse is pressed.  Most likely the mouseMotion func will
// be called immediately afterwards if the mouse button is still down.
// NOTE: a single click will cause this function to be called twice
// once for GLUT_DOWN and once for GLUT_UP
void mousePress(GLint button, GLint state, GLint x, GLint y)
{


	if( button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && SELECTMODE) {
		mouseX = 250 - x;
		mouseY = 250 - y;





		double findMin = 100000;

		for (int i = 0; i < NUM_OF_THINGS -1; i++) {

			double temp = ThingPointer[i]->distance(mouseX, mouseY);
			if(findMin > temp) {
				if(temp < ThingPointer[i]->size){

					if(hasSelected){
						isSelected->setColorDefault();
						isSelected->updateMaterial();
					}

					isSelected = ThingPointer[i];
					hasSelected = true;
				}
			}
		}

		if(isSelected != NULL) {
			isSelected->setColor(0,0,0);
			isSelected->updateMaterial();
		}
	}



}



int main(int argc, char** argv)
{
	srand(time(0));


	glutInit(&argc, argv);   // not necessary unless on Unix
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (500, 500);
	glutCreateWindow ("3D WORLD");
	init();

	//glutReshapeFunc (reshape);       // register respace (anytime window changes)
	glutKeyboardFunc (keyboard);     // register keyboard (anytime keypressed)
	glutSpecialFunc(arrow);

	glutMouseFunc (mousePress);      // register mouse press funct

	glutDisplayFunc (display);       // register display function
	glutIdleFunc (idle);             // reister idle function

	glutMainLoop();
	return 0;
}