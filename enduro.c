#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include "bibutil.h"

#define PI 3.14159265

GLfloat fAspect;
GLfloat angle = 45;
GLsizei W, H;

float carPositionX = -8.0f;
float carPositionY = 5.0f;
float carPositionZ = -7.0f;

float carSpeed = 0.0f;
float carAngularSpeed = 2.5f;
float carSpeedMax = 10.0f;
float carAngle = 0.0f;
float carAcceleration = 0.5f;

int isSpeedingUp = 0;
int isBackingUp = 0;
int isGoLeft = 0;
int isGoRight = 0;

float carSidesMax = 5;
float carSidesActions = 0;
float carSidesDescolation = 0.5;

float distanceCamFromCar = 80.0f;
float camPositionX = -8.0f;
float camPositionY = 30.0f;
float camPositionZ = -7.0f;
float camCenterPositionY = 20.0f;

char modeCam = 't';
char modeObj = 't';

OBJ *carObj;
OBJ *trackObj;

GLuint LoadTexture( const char * filename, int width, int height )
{
  GLuint texture;
  unsigned char * data;

  FILE * file;
  file = fopen( filename, "rb" );
  

  if ( file == NULL ) return 0;
  data = (unsigned char *)malloc( width * height * 3 );
  fread( data, width * height * 3, 1, file );
  fclose( file );

  for(int i = 0; i < width * height ; ++i)
  {
    int index = i*3;
    unsigned char B,R;
    B = data[index];
    R = data[index+2];

    data[index] = R;
    data[index+2] = B;
  }
  
  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
  free( data );

  return texture;
}

void drawCar() {
    glPushMatrix();
		glColor3f(1.0f, 1.0f, 0.0f);
		glTranslatef(carPositionX - carSidesActions * sin(carAngle * PI / 180), carPositionY, carPositionZ - carSidesActions * cos(carAngle * PI / 180));
		glRotatef(carAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0, -1.0, 0.0);
		glScalef(10.0f, 10.0f, 10.0f); 
		DesenhaObjeto(carObj);
		//glutSolidCube(10.0);
    glPopMatrix();
}

void drawTrack() {
	glPushMatrix();
		glColor3f(0.96f, 0.96f, 0.96f);
		glTranslatef(0.0, 0.0, 0.0);
		glScalef(1300.0f, 1300.0f, 1300.0f); 
		DesenhaObjeto(trackObj);
	glPopMatrix();
}

void drawGround(){
	glPushMatrix();
		glColor3f(0.0f, 1.28f, 0.0f);
		//LoadTexture("textures/test.bmp",256,256);
		//glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-100000.0, 0.0, 100000.0);
			glTexCoord2f(0.0f, 0.0f);glVertex3f(100000.0, 0.0, 100000.0);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(100000.0, 0.0, -100000.0);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(-100000.0, 0.0, -100000.0);
		glEnd();
		//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawSky(){
	glPushMatrix();
		glColor3f(0.49f, 2.39f, 2.33f);
		//LoadTexture("textures/test.bmp",256,256);
		//glEnable(GL_TEXTURE_2D);

		GLUquadric *qobj = gluNewQuadric();
		//glTranslatef(0.0, 0.0, 0.0);
		//gluQuadricTexture(qobj, GL_TRUE);
		gluSphere(qobj, 10000, 100, 100);
		
		gluDeleteQuadric(qobj);
		//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawObjRef(){
	//Chaleira Azul
	glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(13.0, 5.0, -13.0);
		glutSolidTeapot(5.0f);
	glPopMatrix();

	//Chaleira Vermelha
	glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(-13.0, 5.0, 13.0);
		glutSolidTeapot(5.0f);
	glPopMatrix();
	
	//Donuts Verde
	glPushMatrix();
		glTranslatef(13.0, 5.0, 13.0);
		glColor3f(0.0f, 1.0f, 0.0f);
		glRotatef(-90.0,1.0,0.0,0.0);
		glTranslatef(0.0, 0.0, 0.0);
		glutSolidTorus(3.0,4.0,20.0,40.0);
	glPopMatrix();

	//Donuts Amarelo
	glPushMatrix();
		glTranslatef(-13.0, 5.0, -13.0);
		glColor3f(1.0f, 1.0f, 0.0f);
		glRotatef(-90.0,1.0,0.0,0.0);
		glTranslatef(0.0, 0.0, 0.0);
		glutSolidTorus(3.0,4.0,20.0,40.0);
	glPopMatrix();
}

void drawScene(){
	drawCar();
	drawTrack();
	drawGround();
	drawSky();
}	

// Função callback chamada para fazer o desenho
void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
	glutSwapBuffers();
 }

void loadAllObjects(){
	carObj = CarregaObjeto("./objects/", "car.obj",0);
	trackObj = CarregaObjeto("./objects/", "track.obj",0);
	SetaModoDesenho('t');	// 's' para s�lido
}

// Inicializa parâmetros de rendering
void init(void)
{
	GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0}; 
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};		 // "cor" 
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho" 
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0}; 
	GLint especMaterial = 60;

 	// Especifica que a cor de fundo da janela será preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material 
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	//Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	//Habilita teste de profundidade
	glEnable(GL_DEPTH_TEST);

	camPositionX = carPositionX - distanceCamFromCar * cos(carAngle * PI / 180);
	camPositionZ = carPositionZ + distanceCamFromCar * sin(carAngle * PI / 180);   

	loadAllObjects();
}

// Função usada para especificar o volume de visualização
void configViewMode(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	gluPerspective(angle,fAspect,0.5,100000);
	
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	if (modeCam == 't'){
		float camAnimation = carSpeed/(carSpeedMax*0.7);
		distanceCamFromCar = 70 + (camAnimation * 10.0f);
		camPositionY = 30.0f;
		camCenterPositionY = carPositionY + 15;
	}
	else if (modeCam == 'p'){
		distanceCamFromCar = 0.5f;
		camPositionY = 17;
		camCenterPositionY = 17;
	}

	camPositionX = carPositionX - distanceCamFromCar * cos(carAngle * PI / 180);
	camPositionZ = carPositionZ + distanceCamFromCar * sin(carAngle * PI / 180);
	
	if (modeCam == 'x')
		gluLookAt(carPositionX,700,carPositionZ,carPositionX,carPositionY,carPositionZ,0,0,1);
	else
		gluLookAt(camPositionX,camPositionY,camPositionZ,carPositionX,camCenterPositionY,carPositionZ,0,1,0);
}

void updateSpeed(int value) {

	if (isBackingUp && carSpeed > 0)
		carSpeed -= carAcceleration;
	else if (isSpeedingUp && carSpeed <= carSpeedMax)
		carSpeed += carAcceleration;
	else if (!isSpeedingUp && carSpeed > 0)
		carSpeed -= (2/carSpeed);
	
	//camPositionX = carPositionX - distanceCamFromCar * cos(carAngle * PI / 180);
	//camPositionZ = carPositionZ + distanceCamFromCar * sin(carAngle * PI / 180);

	//if (carSpeed > 0){
		carSidesMax = modeCam == 'p' ? 2 : 5;
		if (isGoLeft){
			carAngle += carAngularSpeed;
			if (carSidesActions > -carSidesMax && carSpeed > 0)
				carSidesActions -= carSidesDescolation;
		}
		else if (!isGoRight){
			if (carSidesActions < 0)
				carSidesActions += carSidesDescolation;
		}

		if (isGoRight){
			carAngle -= carAngularSpeed;
			if (carSidesActions < carSidesMax && carSpeed > 0)
				carSidesActions += carSidesDescolation;
		}
		else if(!isGoLeft){
			if (carSidesActions > 0)
				carSidesActions -= carSidesDescolation;
		}
	//}

	if (carSpeed > 0){
        carPositionX += carSpeed * cos(carAngle * PI / 180);
        carPositionZ -= carSpeed * sin(carAngle * PI / 180);
	}
	
	if (isGoLeft || isGoRight || carSpeed > 0){
		glutPostRedisplay();
		configViewMode();
		glutPostRedisplay();
	}

	glutTimerFunc(1, updateSpeed, 0);
}

// Função callback chamada quando o tamanho da janela é alterado 
void chageWindow(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;
	W = w;
	H = h;
	// Especifica o tamanho da viewport
    glViewport(0, 0, w, h);
 
	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	configViewMode();
}

void freeAllObjects(){
	LiberaObjeto(carObj);
	LiberaObjeto(trackObj);
}

void keyboardEvent(unsigned char key, int x, int y)
{
	switch(key){
		case 27:
			freeAllObjects();
			exit(0);
			break;
		case 'a':
			if (carSidesActions < carSidesMax){
				carSidesActions += carSidesDescolation;
				glutPostRedisplay();
			}
			break;
		case 'd':
			if (carSidesActions > -carSidesMax){
				carSidesActions -= carSidesDescolation;
				glutPostRedisplay();
			}
			break;
		case 'c':
			modeCam = modeCam == 't' ? 'p' : 't';
			configViewMode();
			glutPostRedisplay();
			//distanceCamFromCar = distanceCamFromCar == 80.0f ? 8.0f : 80.0f;
			//camPositionY = camPositionY == 30.0f ? 10 : 30.0f;
			//camCenterPositionY = camPositionY == 30.0f ? camPositionY : carPositionY + 15;
			break;
		case 'x':
			modeCam = modeCam == 'x' ? 't' : 'x';
			configViewMode();
			glutPostRedisplay();
			break;
		case 'z':
			modeObj = modeObj == 't' ? 'w' : 't';
			SetaModoDesenho(modeObj);
			break;
	}
}

void specialEvent(int key, int x, int y) {
	switch(key){
		case GLUT_KEY_LEFT:
			isGoLeft = 1;
			break;
		case GLUT_KEY_RIGHT:
			isGoRight = 1;
			break;
		case GLUT_KEY_DOWN:
			isBackingUp = 1;
			break;
		case GLUT_KEY_UP:
			isSpeedingUp = 1;
			break;
	}
} 

void specialKeyReleased(int key, int x, int y) {
	switch(key){
		case GLUT_KEY_UP:
			isSpeedingUp = 0;
			break;
		case GLUT_KEY_LEFT:
			isGoLeft = 0;
			break;
		case GLUT_KEY_RIGHT:
			isGoRight = 0;
			break;
		case GLUT_KEY_DOWN:
			isBackingUp = 0;
			break;
	}
} 

// Programa Principal 
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
	glutCreateWindow("ENDURO-3D");
	glutDisplayFunc(draw);
    glutReshapeFunc(chageWindow);
	glutKeyboardFunc(keyboardEvent);
	glutSpecialFunc(specialEvent);
	glutSpecialUpFunc(specialKeyReleased);

	glutTimerFunc(1, updateSpeed, 0);
	
	init();
	glutMainLoop();
}
