#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <dirent.h> 
#include <string.h>
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
float carSpeedMax = 15.0f;
float carAngle = 180.0f;
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

typedef struct PositionCar {
	float carPositionX;
	float carPositionY;
	float carPositionZ;
	float carAngle;
} PositionCar;

typedef struct ControllCar {
	int nPosition;
	PositionCar * position;
	OBJ *carObj;
	int time;
} ControllCar;


char pathTrajectories[] = "./trajectories/";

OBJ *carObj;
OBJ *trackObj;

int nCompetitors = 0;
ControllCar* listCompetitors;

FILE *fileTrajectory = NULL;
int recordNewTrajectory = 0;

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

void drawCompetitors() {
	for (int i = 0; i < nCompetitors; i++){
		glPushMatrix();
			glColor3f(1.0f, 1.0f, 0.0f);
			glTranslatef(listCompetitors[i].position[listCompetitors[i].time].carPositionX, listCompetitors[i].position[listCompetitors[i].time].carPositionY, listCompetitors[i].position[listCompetitors[i].time].carPositionZ);
			glRotatef(listCompetitors[i].position[listCompetitors[i].time].carAngle, 0.0f, 1.0f, 0.0f);
			glRotatef(90, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0, -1.0, 0.0);
			glScalef(10.0f, 10.0f, 10.0f); 
			DesenhaObjeto(listCompetitors[i].carObj);
			//glutSolidCube(10.0);
    	glPopMatrix();
	}
}

void drawTrack() {
	glPushMatrix();
		glColor3f(1.64f, 1.13f, 0.55f);
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

void drawArrive(){
	glPushMatrix();
		glColor3f(255.0f, 255.0f, 255.0f);
		//LoadTexture("textures/test.bmp",256,256);
		//glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-60.0f, 8.0, 120.0f);
			glTexCoord2f(0.0f, 0.0f);glVertex3f(120.0f, 8.0, 120.0f);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(120.0f, 8.0, -150.0f);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(-60.0f, 8.0, -150.0f);
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
	drawCompetitors();
	drawTrack();
	drawArrive();
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
	for (int i = 0; i < nCompetitors; i++)
		listCompetitors[i].carObj = CarregaObjeto("./objects/", "car.obj",0);

	SetaModoDesenho(modeObj);
}

// Inicializa parâmetros de rendering
void init(void)
{
	srand( (unsigned)time(NULL) );
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

	nCompetitors = 0;
	struct dirent *dir;
	DIR *d = opendir(pathTrajectories);
	
	while ((dir = readdir(d)) != NULL)
		if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
			nCompetitors++;
	
	listCompetitors = (ControllCar*) calloc(nCompetitors, sizeof(ControllCar));
	d = opendir(pathTrajectories);

	int index = 0;
	while ((dir = readdir(d)) != NULL){
		if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
			continue;
		
		char fileDir[100];
		listCompetitors[index].carObj = NULL;
		
		sprintf(fileDir, "%s%s", pathTrajectories, dir->d_name);
		printf("%s\n",  fileDir);
		
		FILE * filePositions = fopen(fileDir, "r");
		char c;
		while ((c = fgetc(filePositions)) != EOF)
			if (c == '\n')
				listCompetitors[index].nPosition++;
		listCompetitors[index].nPosition++;
		listCompetitors[index].position = (PositionCar*) calloc(listCompetitors[index].nPosition, sizeof(PositionCar));

		fclose(filePositions);

		filePositions = fopen(fileDir, "r");
		char * line = NULL;
		size_t len = 0;
		int indexPosition = 0;
		while ((getline(&line, &len, filePositions)) != -1) {
			printf("%s", line);
			float x, y, z, angle;
			sscanf(line, "%f;%f;%f;%f", &x, &y, &z, &angle); 
			listCompetitors[index].position[indexPosition].carPositionX = x;
			listCompetitors[index].position[indexPosition].carPositionY = y; 
			listCompetitors[index].position[indexPosition].carPositionZ = z;
			listCompetitors[index].position[indexPosition].carAngle = angle;
			printf("%f %f %f %f\n", listCompetitors[index].position[indexPosition].carPositionX, listCompetitors[index].position[indexPosition].carPositionY, listCompetitors[index].position[indexPosition].carPositionZ);
			indexPosition++;
		}
		index ++;
	}

	closedir(d);
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

		camPositionX = carPositionX - distanceCamFromCar * cos(carAngle * PI / 180);
		camPositionZ = carPositionZ + distanceCamFromCar * sin(carAngle * PI / 180);

		gluLookAt(camPositionX,camPositionY,camPositionZ,carPositionX,camCenterPositionY,carPositionZ,0,1,0);
	}
	else if (modeCam == 'p'){
		distanceCamFromCar = -10.0f;
		camPositionY = 17;
		camCenterPositionY = 17;

		camPositionX = carPositionX - distanceCamFromCar * cos(carAngle * PI / 180);
		camPositionZ = carPositionZ + distanceCamFromCar * sin(carAngle * PI / 180);

		float targetX = carPositionX + 30 * cos(carAngle * PI / 180);
		float targetZ = carPositionZ - 30 * sin(carAngle * PI / 180);

		gluLookAt(camPositionX,camPositionY,camPositionZ,targetX,camCenterPositionY,targetZ,0,1,0);
	}
	else if (modeCam == 'x')
		gluLookAt(carPositionX,700,carPositionZ,carPositionX,carPositionY,carPositionZ,0,0,1);
}

int isCollision() {
    int size = 30;
    float minX1 = carPositionX;
    float maxX1 = carPositionX + size;
    float minY1 = carPositionY;
    float maxY1 = carPositionY + size;
    float minZ1 = carPositionZ;
    float maxZ1 = carPositionZ + size;

	for (int i = 0; i < nCompetitors; i++){
		float minX2 = listCompetitors[i].position[listCompetitors[i].time].carPositionX;
		float maxX2 = listCompetitors[i].position[listCompetitors[i].time].carPositionX + size;
		float minY2 = listCompetitors[i].position[listCompetitors[i].time].carPositionY;
		float maxY2 = listCompetitors[i].position[listCompetitors[i].time].carPositionY + size;
		float minZ2 = listCompetitors[i].position[listCompetitors[i].time].carPositionZ;
		float maxZ2 = listCompetitors[i].position[listCompetitors[i].time].carPositionZ + size;

		// Verificando se há sobreposição em cada eixo
		if (maxX1 < minX2 || minX1 > maxX2) continue; // Sem sobreposição no eixo X
		if (maxY1 < minY2 || minY1 > maxY2) continue; // Sem sobreposição no eixo Y
		if (maxZ1 < minZ2 || minZ1 > maxZ2) continue; // Sem sobreposição no eixo Z

		// Se passou pelos testes acima, os cubos estão colidindo
		return 1;
	}
	return 0;
}


void updateSpeed(int value) {

	if (isBackingUp && carSpeed > 0)
		carSpeed -= carAcceleration;
	else if (isSpeedingUp && carSpeed <= carSpeedMax)
		carSpeed += carAcceleration;
	else if (!isSpeedingUp && carSpeed > 0)
		carSpeed -= (2/carSpeed);

	carSidesMax = modeCam == 'p' ? 0 : 5;

	if (carSpeed > 0){
		if (isGoLeft){
			carAngle += carAngularSpeed;
			if (carSidesActions > -carSidesMax && carSpeed > 0)
				carSidesActions -= carSidesDescolation;
		}
		else if (!isGoRight)
			if (carSidesActions < 0)
				carSidesActions += carSidesDescolation;

		if (isGoRight){
			carAngle -= carAngularSpeed;
			if (carSidesActions < carSidesMax && carSpeed > 0)
				carSidesActions += carSidesDescolation;
		}
		else if(!isGoLeft)
			if (carSidesActions > 0)
				carSidesActions -= carSidesDescolation;

		if (carSpeed > 0){
			carPositionX += carSpeed * cos(carAngle * PI / 180);
			carPositionZ -= carSpeed * sin(carAngle * PI / 180);
		}
	}

	if (recordNewTrajectory){
		if (fileTrajectory == NULL){
			int id = rand();
			char fileDir[100];
			sprintf(fileDir, "%s%d.dat", pathTrajectories, id);
			fileTrajectory = fopen(fileDir, "w");

			for (int i = 0; i < nCompetitors; i++)
				listCompetitors[i].time=0;
		}
		fprintf(fileTrajectory, "%f;%f;%f;%f\n", carPositionX, carPositionY, carPositionZ, carAngle);
	}
	else{
		if (fileTrajectory != NULL)
			fclose(fileTrajectory);
		fileTrajectory = NULL;
	}

	glutPostRedisplay();
	
	if (isCollision())
		carSpeed = 0.0;

	if (isGoLeft || isGoRight || carSpeed > 0){
		configViewMode();
		glutPostRedisplay();
	}

	for (int i = 0; i < nCompetitors; i++){
		listCompetitors[i].time++;
		if (listCompetitors[i].time > listCompetitors[i].nPosition)
			listCompetitors[i].time = 0;
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
	for (int i = 0; i < nCompetitors; i++)
		LiberaObjeto(listCompetitors[i].carObj);
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
			configViewMode();
			glutPostRedisplay();
			break;
		case 'r':
			recordNewTrajectory = recordNewTrajectory ? 0 : 1;
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
