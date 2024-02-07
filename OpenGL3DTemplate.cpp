#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <ctime>

#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") 


#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
#define M_PI 3.14159265358979323846

bool animate = false;
bool soundActive = false;
bool returnSound = true;


bool keyL = false; 
bool keyR = false;  
bool keyF = false;  
bool keyB = false;  
float treeRotationAngle = 0.0f;
bool col = false;

int gameTime = 0;
bool flag30Reached = false;

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};
class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};
Camera camera;
void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}


int keyTimerInterval = 50;
float playerSpeed = 200.0;

float playerX = 0.0f;
float playerZ = 0.0f;


void movePlayer(float dx, float dz) {

	float newX = playerX + dx;
	float newZ = playerZ + dz;

	float minX = -4.65f;
	float maxX = 3.0f;
	float minZ = -5.05f;
	float maxZ = 2.75f;

	if (newX >= minX && newX <= maxX && newZ >= minZ && newZ <= maxZ) {
		playerX = newX;
		playerZ = newZ;
		glutPostRedisplay();
	}
	printf("Player X: %f\n", playerX);
	printf("Player Z: %f\n", playerZ);

	glutPostRedisplay();
}

bool isCol() {
	float minX = -4.599998;
	float maxX = -3.699999;
	float minZ = 0.0;
	float maxZ = 0.0;
	return (playerX >= minX && playerX <= maxX) && (playerZ >= minZ && playerZ <= maxZ);
}
float Move = 0.0f;
void drawHuman() {
	glPushMatrix();
	glTranslatef(playerX, 0.0f, playerZ);

		if (keyF) {
			if (Move = 0) {
				Move = 90.0f;
			}
			keyF = false;

		}
		if (keyB) {
			if (Move = 0) {
				Move = 90.0f;
			}			
			keyB = false;
		}
		
		if (keyL) {
			if (Move = 90) {
				Move = -90;

			}
			else {
				Move = 0;
			}
			keyL = false;

		}
		if (keyR) {
			if (Move = 90) {
				Move = -90;
			}
			else {
				Move = 0;

			}
			keyR = false;

		}

		glRotatef(Move, 0.0f, 1.0f, 0.0f);

		glPushMatrix();
		glTranslated(0.0, 0.8, 0.0);
		glColor3f(0.96, 0.83, 0.63);  
		glutSolidSphere(0.2, 20, 20);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.0, 0.5, 0.0);
		glColor3f(0.0, 0.0, 1.0);  
		glutSolidCube(0.2);
		glPopMatrix();

    
		glPushMatrix();
		glTranslated(-0.07, 0.25, 0.0);
		glColor3f(0.0, 0.0, 0.0);  
		glutSolidCube(0.1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.07, 0.25, 0.0);
		glColor3f(0.0, 0.0, 0.0);  
		glutSolidCube(0.1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.18, 0.65, 0.0);
		glColor3f(0.96, 0.83, 0.63);  
		glutSolidCube(0.1);
		glPopMatrix();

    
		glPushMatrix();
		glTranslated(0.18, 0.65, 0.0);
		glColor3f(0.96, 0.83, 0.63);  
		glutSolidCube(0.1);
		glPopMatrix();
	glPopMatrix();

}

void drawApple() {
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);  
	glutSolidSphere(0.04, 20, 20);
	glPopMatrix();
}

void drawTree() {
	glPushMatrix();
	glTranslated(0.0, 0.48, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.18, 0.0);
	glColor3f(0.4, 0.2, 0.0);
	glutSolidCube(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.45, 0.2);
	drawApple();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.15, 0.45, -0.1);
	drawApple();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.15, 0.45, -0.1);
	drawApple();
	glPopMatrix();
}


void drawFerrisWheel() {
	glColor3f(0.5f, 0.5f, 0.5f);
	GLUquadricObj* baseCylinder = gluNewQuadric();
	gluCylinder(baseCylinder, 0.5, 0.5, 0.1, 30, 30);
	gluDeleteQuadric(baseCylinder);

	glColor3f(0.7f, 0.7f, 0.7f); 
	const float wheelRadius = 2.0f;
	const int numSpokes = 12;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.1f);
	for (int i = 0; i <= numSpokes; ++i) {
		float angle = (2.0f * M_PI * i) / numSpokes;
		glVertex3f(wheelRadius * cos(angle), wheelRadius * sin(angle), 0.1f);
	}
	glEnd();

	glColor3f(0.6f, 0.6f, 0.6f); 
	const float spokeLength = 3.5f;
	const float spokeWidth = 0.05f;
	for (int i = 0; i < numSpokes; ++i) {
		glPushMatrix();
		glRotatef((360.0f / numSpokes) * i, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, wheelRadius, 0.1f);
		glScalef(spokeWidth, spokeLength, spokeWidth);
		glutSolidCube(1.0f);
		glPopMatrix();
	}

	glColor3f(0.6f, 0.4f, 0.2f); 
	const int numCabins = 12;
	const float cabinRadius = 0.6f;
	const float cabinDistance = 3.5f; 

	for (int i = 0; i < numCabins; ++i) {
		float angle = 360.0f / numCabins * i;
		float x = cabinDistance * cos(angle * M_PI / 180.0f);
		float y = cabinDistance * sin(angle * M_PI / 180.0f);

		glPushMatrix();
		glTranslatef(x, y, 0.1f);
		glutSolidCube(0.8);
		glPopMatrix();
	}

	glColor3f(0.4f, 0.2f, 0.8f);
	const float legHeight = 1.5f;
	const float legRadius = 0.1f;
	const int numLegs = 3;
	for (int i = 0; i < numLegs; ++i) {
		glPushMatrix();
		glRotatef(120.0f * i, 0.0f, 0.0f, 1.0f); 
		glTranslatef(0.0f, 0.0f, -legHeight * 0.5f);
		GLUquadricObj* legCylinder = gluNewQuadric();
		gluCylinder(legCylinder, legRadius, legRadius, legHeight, 20, 20);
		gluDeleteQuadric(legCylinder);
		glPopMatrix();
	}
}

void drawGermanFlag(float x, float y, float z, float width, float height, float rotationAngleG) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rotationAngleG, 0.0f, 1.0f, 0.0f); 
	glScalef(width, height, 1.0);

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.5, 0.17, 0.0);
	glVertex3f(0.5, 0.17, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.16, 0.0);
	glVertex3f(0.5, -0.16, 0.0);
	glVertex3f(0.5, 0.17, 0.0);
	glVertex3f(-0.5, 0.17, 0.0);
	glEnd();

	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.49, 0.0);
	glVertex3f(0.5, -0.49, 0.0);
	glVertex3f(0.5, -0.16, 0.0);
	glVertex3f(-0.5, -0.16, 0.0);
	glEnd();

	glPopMatrix();
}

GLfloat carPosition = -1.0f; 
GLfloat carSpeed = 0.01f;
GLfloat stairRadius = 0.1f; 

int stairSteps = 20;  

void drawCylinderStair() {
	glColor3f(0.0f, 0.0f, 1.0f); 

	for (int i = 0; i <= stairSteps; ++i) {
		float theta = static_cast<float>(i) / static_cast<float>(stairSteps) * 2.0f * 3.1415926535f;
		float x = stairRadius * cos(theta);
		float y = 0.1f * i / static_cast<float>(stairSteps) - 0.17f; 
		float z = stairRadius * sin(theta);

		glBegin(GL_QUADS);
		glVertex3f(x, y, z);
		glVertex3f(x, y + 0.1f, z);

		theta = static_cast<float>(i + 1) / static_cast<float>(stairSteps) * 2.0f * 3.1415926535f;
		x = stairRadius * cos(theta);
		y = 0.1f * (i + 1) / static_cast<float>(stairSteps) - 0.17f; 
		z = stairRadius * sin(theta);

		glVertex3f(x, y + 0.1f, z);
		glVertex3f(x, y, z);
		glEnd();
	}
}

void drawRollerCoaster() {
	glPushMatrix();

	GLfloat wallY = 0.5f;

	glColor3f(0.58f, 0.0f, 0.83f);
	glLineWidth(5.0f);

	glBegin(GL_LINE_STRIP);
	for (float t = -1.0f; t <= 1.0f; t += 0.1f) {
		float x = t;
		float y = wallY + sin(t) + 0.5f;
		float z = (t * t); 
		glVertex3f(x, y, z);
	}
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f); 
	glPushMatrix();
	glTranslatef(carPosition, wallY + sin(carPosition) + 0.5f, carPosition * carPosition);
	glutSolidCube(0.1f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(carPosition, wallY + sin(carPosition) + 0.5f, carPosition * carPosition);
	drawCylinderStair();
	glPopMatrix();

	glPopMatrix();
}



float rocketHeight = 0.0f;
float chairHeight = 0.25f;
float stickHeight = 1.25f;
float chairAnimationSpeed = 0.01f;  

void drawStairs() {
	glPushMatrix();
	glColor3f(0.0f, 0.56f, 1.0f);
	glTranslatef(-1.5f, rocketHeight, -1.5f);
	glScalef(1.0f, 0.25f, 1.0f);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void drawRocketStick() {
	glPushMatrix();
	glTranslatef(-1.5f, rocketHeight + 0.25f, -1.5f);
	GLUquadricObj* rocketStick = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  
	glColor3f(0.0f, 0.56f, 1.0f);
	gluCylinder(rocketStick, 0.1, 0.1, stickHeight, 20, 20);
	gluDeleteQuadric(rocketStick);

	glColor3f(1.0f, 1.0f, 1.0f);  
	glTranslatef(0.0f, 0.0f, stickHeight);
	glutSolidSphere(0.15, 20, 20); 

	glPushMatrix();
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.1f);
	glutSolidCone(0.1, 0.4, 20, 20); 
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);  
	glTranslatef(0.0f, 0.0f, 0.1f); 
	glutSolidCone(0.1, 0.4, 20, 20);  
	glPopMatrix();

	glPopMatrix();
}

void drawChairs() {
	glPushMatrix();
	glTranslatef(-1.5f, rocketHeight + chairHeight, -1.5f);

	if (animate) {
		static float chairAnimation = 0.0f;
		chairAnimation += chairAnimationSpeed;

		if (chairAnimation > 4.0f || chairAnimation < 0.0f) {
			chairAnimationSpeed = -chairAnimationSpeed;
		}

		glTranslatef(0.0f, chairHeight * chairAnimation, 0.0f);
	}
	glutSolidCube(0.3f);  
	glPopMatrix();
}

void animateCar(int value) {
	carPosition += carSpeed;

	if (carPosition >= 1.0f || carPosition <= -1.0f) {
		carSpeed = -carSpeed;
	}
	glutPostRedisplay();

	if (animate) {
		glutTimerFunc(16, animateCar, 0);
	}
}


void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;
	float stepSize = 0.1f;
	if (!isCol()) {
		if (!flag30Reached) {
			switch (key) {
			case 'w':
				camera.moveY(d);
				break;
			case 's':
				camera.moveY(-d);
				break;
			case 'a':
				camera.moveX(d);
				break;
			case 'd':
				camera.moveX(-d);
				break;
			case 'q':
				camera.moveZ(d);
				break;
			case 'e':
				camera.moveZ(-d);
				break;
			case 'v':
				movePlayer(stepSize, 0);
				keyR = true;
				break;
			case 'c':
				movePlayer(-stepSize, 0);
				keyL = true;
				break;
			case 'b':
				movePlayer(0, stepSize);
				keyF = true;
				break;
			case 'n':
				movePlayer(0, -stepSize);
				keyB = true;
				break;
			case 'z':
				animate = true;
				PlaySound(TEXT("animate.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
				glutTimerFunc(0, animateCar, 0);
				break;

			case 'x':
				animate = false;
				PlaySound(TEXT("noanimate.wav"), NULL, SND_ASYNC | SND_FILENAME);
				Sleep(2000);
				PlaySound(TEXT("background.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
				break;
			case 'p':
				camera.eye = Vector3f(0.0f, 0.6f, 2.5f);
				camera.center = Vector3f(0.0f, 0.0f, 0.0f);
				camera.up = Vector3f(0.0f, 1.0f, 0.0f);
				camera.look();
				break;
			case 'o':
				camera.eye = Vector3f(1.9f, 0.2f, 0.0f);
				camera.center = Vector3f(0.0f, 0.0f, 0.0f);
				camera.up = Vector3f(0.0f, 1.0f, 0.0f);
				camera.look();
				break;
			case 'i':
				camera.eye = Vector3f(0.0f, 6.0f, 0.0f);
				camera.center = Vector3f(0.0f, 0.0f, 0.0f);
				camera.up = Vector3f(-1.0f, 0.0f, 0.0f);
				camera.look();
				break;
			case GLUT_KEY_ESCAPE:
				exit(EXIT_SUCCESS);
			}
		}
	}
	else {
		PlaySound(TEXT("Goal.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	glutPostRedisplay();
}

void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}
float rotationAngleW = 0.0f; 

GLfloat wallColor2[3] = { 1.0f, 0.6f, 0.4f };
GLfloat wallColors[5][3] = {
	{1.0f, 0.6f, 0.4f},  
	{1.0f, 1.0f, 0.5f},  
	{1.0f, 0.8f, 0.6f},  
	{1.0f, 0.4f, 0.2f},   
	{0.1f, 0.1f, 0.3f}
};
int currentColorIndex = 0;
void updateWallColors() {
	wallColor2[0] = wallColors[currentColorIndex][0];
	wallColor2[1] = wallColors[currentColorIndex][1];
	wallColor2[2] = wallColors[currentColorIndex][2];

	currentColorIndex = (currentColorIndex + 1) % 5;
}


void mainLoop(int value) {
	static time_t lastUpdateTime = 0;
	time_t currentTime = time(nullptr);

	if (currentTime - lastUpdateTime >= 5) {
		updateWallColors();
		lastUpdateTime = currentTime;
	}

	if (animate) {
		treeRotationAngle += 1.0f;
	}
	
	glutTimerFunc(keyTimerInterval, mainLoop, 0);
	glutPostRedisplay();
}


void updateTimer(int value) {
	gameTime++;
	if (gameTime == 30) {
		flag30Reached = true;
	
			if (!isCol()) {
				printf("Playing loss.wav\n");
				PlaySound(TEXT("loss.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
			}
		
	}
	if (gameTime < 30) {
		glutTimerFunc(1000, updateTimer, 0);
	}
}

void drawTimer() {
	int remainingTime = 30 - gameTime;

	glRasterPos2f(-0.15f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'T');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'i');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'm');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'e');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'r');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + remainingTime / 10);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + remainingTime % 10);

	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);

	float wallX = 0.0f;  
	float wallY = 0.25f; 
	float wallZ = -2.0f; 

	glTranslatef(wallX, wallY, wallZ);
	glScalef(0.01f, 0.01f, 0.01f); 
	glutSolidCube(1.0);

	glPopMatrix();
}


float stickSwingAngle = 0.0f; 
float shipSwingAngle = 0.0f;
float swingDirection = 1.0f;   
const float cubeHeight = 0.25f; 
const float shipSize = 0.4f;     
const float circleRadius = 1.0f;

void drawCube() {
	glutSolidCube(shipSize);
}

void drawCH() {
	glPushMatrix();
	glColor3f(0.7, 0.4, 0.1); 
	glTranslatef(0.0f, -1.0, 0.0f);
	glBegin(GL_POLYGON);
	glVertex2f(-0.5, -0.2);
	glVertex2f(0.5, -0.2);
	glVertex2f(0.4, 0.0);
	glVertex2f(-0.4, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-0.2, 0.0);
	glVertex2f(0.2, 0.0);
	glVertex2f(0.2, 0.2);
	glVertex2f(-0.2, 0.2);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(0.0, 0.2);
	glVertex2f(0.0, 0.4);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.2);
	glVertex2f(0.1, 0.4);
	glVertex2f(-0.1, 0.4);
	glEnd();
	glPopMatrix();
}
void drawStick() {
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, cubeHeight / 2.0f, 0.0f);

	glBegin(GL_LINES);
	glVertex3f(0.0f, -cubeHeight / 2.0f, 0.0f); 
	glVertex3f(0.0f, -1.25f, 0.0f); 
	glEnd();

	glPopMatrix();
}
void drawShip() {
	glPushMatrix();
	glTranslatef(1.0, 0.0, 1.5);

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 0.5f, 1.0f);
		drawCube();
		glPopMatrix();

	
		glPushMatrix();
		glTranslatef(0.0f, 1.5f, 0.0f);  
		glRotatef(shipSwingAngle, 0.0f, 0.0f, 1.0f);  
		drawStick();
		drawCH();
		glColor3f(0.0f, 0.5f, 1.0f);
		glutSolidCube(shipSize * 0.7); 
		glPopMatrix();
	glPopMatrix();

}

void updateSwing(int value) {
	if (animate) {
		shipSwingAngle += 0.5f * swingDirection;
		if (shipSwingAngle > 30.0f || shipSwingAngle < -30.0f) {
			swingDirection = -swingDirection;
		}
	}
	
	glutPostRedisplay(); 
	glutTimerFunc(30, updateSwing, 0);  
}

void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslated(0.4, -0.1, 0.6);  
	glRotated(0, 1, 0, 0);       
	glScaled(0.5, 0.5, 0.5);
	drawHuman();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.5, -0.08, 0.0);
	glRotatef(treeRotationAngle, 0.0f, 1.0f, 0.0f);
	glScaled(1.0, 1.0, 1.0);
	drawTree();
	glPopMatrix();

	drawGermanFlag(-1.99, 0.25, 0.0, 0.5, 0.5, 90.0);
	drawGermanFlag(0.0, 0.25, -1.99, 0.5, 0.5, 0.0);
	drawGermanFlag(1.99, 0.25, 0.0, 0.5, 0.5, 90.0);


	glPushMatrix();
	glColor3f(0.0, 0.4, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-2.0f, 0.0f, -2.0f);
	glVertex3f(2.0f, 0.0f, -2.0f);
	glVertex3f(2.0f, 0.0f, 2.0f);
	glVertex3f(-2.0f, 0.0f, 2.0f);
	glEnd();
	glPopMatrix();

	//1
	glPushMatrix();
	glColor3fv(wallColor2);

	glBegin(GL_QUADS);
	glVertex3f(-2.0f, 0.5f, -2.0f);
	glVertex3f(-2.0f, 2.0f, -2.0f);
	glVertex3f(-2.0f, 2.0f, 2.0f);
	glVertex3f(-2.0f, 0.5f, 2.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.2, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-2.0f, 0.0f, -2.0f);
	glVertex3f(-2.0f, 0.5f, -2.0f);
	glVertex3f(-2.0f, 0.5f, 2.0f);
	glVertex3f(-2.0f, 0.0f, 2.0f);
	glEnd();
	glPopMatrix();



	//2

	glPushMatrix();
	glColor3fv(wallColor2);

	glBegin(GL_QUADS);
	glVertex3f(-2.0f, 0.5f, -2.0f);
	glVertex3f(-2.0f, 2.0f, -2.0f);
	glVertex3f(2.0f, 2.0f, -2.0f);
	glVertex3f(2.0f, 0.5f, -2.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.2, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-2.0f, 0.0f, -2.0f);
	glVertex3f(-2.0f, 0.5f, -2.0f);
	glVertex3f(2.0f, 0.5f, -2.0f);
	glVertex3f(2.0f, 0.0f, -2.0f);
	glEnd();
	glPopMatrix();


	//3
	glPushMatrix();
	glColor3fv(wallColor2);

	glBegin(GL_QUADS);
	glVertex3f(2.0f, 0.5f, 2.0f);
	glVertex3f(2.0f, 2.0f, 2.0f);
	glVertex3f(2.0f, 2.0f, -2.0f);
	glVertex3f(2.0f, 0.5f, -2.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.2, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(2.0f, 0.0f, 2.0f);
	glVertex3f(2.0f, 0.5f, 2.0f);
	glVertex3f(2.0f, 0.5f, -2.0f);
	glVertex3f(2.0f, 0.0f, -2.0f);
	glEnd();
	glPopMatrix();

	if (!isCol()) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);

		glTranslated(-1.9, 0.05, 0.6);
		glScaled(0.1, 1.0, 0.1);
		glRotatef(rotationAngleW, 0.0, 1.0, 0.0);
		glutSolidCube(1.0);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);

		glTranslated(-1.6, 0.05, 0.6);
		glScaled(0.1, 1.0, 0.1);
		glRotatef(rotationAngleW, 0.0, 1.0, 0.0);
		glutSolidCube(1.0);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);

		glTranslated(-1.75, 0, 0.6);
		glScaled(0.3, 0.05, 0.1);
		glutSolidCube(1.0);
		glPopMatrix();
	}
	if (isCol() && !flag30Reached) {		
		glColor3f(0.0f, 0.0f, 1.0f);
		glRasterPos2f(-0.2, 0.0); 
		const char* winMessage = "You Win!";
		for (int i = 0; winMessage[i] != '\0'; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, winMessage[i]);
		}
	}
	else if (flag30Reached) {
		if (!isCol()) {
			glColor3f(1.0f, 0.0f, 0.0f);
			glRasterPos2f(-0.2, 0.0);
			const char* winMessage = "Game Over!";
			for (int i = 0; winMessage[i] != '\0'; i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, winMessage[i]);
			}
		}
	}

	glColor3f(0.0f, 1.0f, 0.0f);
	drawTimer();

	rotationAngleW += 1.0f;
	drawRollerCoaster();
	glutSwapBuffers();

	glPushMatrix();
	glTranslated(1,1, -1.75);

	glRotatef(treeRotationAngle, 0.0f, 0.0f, 1.0f);
	glScaled(0.2, 0.2, 0.2);
	drawFerrisWheel();
	glPopMatrix();

	drawStairs();
	drawRocketStick();
	drawChairs();
	drawShip();

	glFlush();


}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Amusment Park");
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	PlaySound(TEXT("background.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	
	glShadeModel(GL_SMOOTH);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutDisplayFunc(Display);

	glutTimerFunc(keyTimerInterval, mainLoop, 0);
	glutTimerFunc(1000, updateTimer, 0);
	glutTimerFunc(30, updateSwing, 0);  

	glutPostRedisplay();
	glutMainLoop();
}





