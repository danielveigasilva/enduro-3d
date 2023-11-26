#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>

#define PI 3.14159265

GLfloat fAspect;
GLfloat angle = 45;
GLfloat lookAtEye[3] = {0.0, 5.0, 200.0};
GLfloat lookAtUnitVector[3] = {0.0, 0.0, 0.0};
GLfloat lookAtSpeedCam = 10;
GLfloat lookAtCenterCam = 0.07;

float carPositionX = -8.0f; // Posição inicial do carro no eixo X
float carPositionY = 0.0f; // Posição inicial do carro no eixo X
float carPositionZ = -7.0f; // Posição inicial do carro no eixo Z
float carSpeed = 0.0f; // Velocidade do carro
float carSpeedMax = 5.0f;
float carAngle = 0.0f; // Ângulo de rotação do carro
float carAcceleration = 0.5f;
int isSpeedingUp = 0;
int isBraking = 0;
float carSidesMax = 25;
float carSidesActions = 0;
float carSidesDescolation = 5;

float distanceCamFromCar = 70.0f;
float camPositionX = -8.0f;
float camPositionY = 30.0f;
float camPositionZ = -7.0f;

void drawCar() {
    glColor3f(0.0f, 1.0f, 0.0f); // Cor verde para o carro
    glPushMatrix();
		glTranslatef(carPositionX - carSidesActions * sin(carAngle * PI / 180), carPositionY, carPositionZ - carSidesActions * cos(carAngle * PI / 180));
		glRotatef(carAngle, 0.0f, 1.0f, 0.0f); // Rotação do carro conforme o ângulo
		glutSolidCube(10.0);
    glPopMatrix();
}

// Função callback chamada para fazer o desenho
void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawCar();

	glBegin(GL_LINES);
        glVertex3f(carPositionX, 5, carPositionZ);
        glVertex3f(carPositionX + 20 * cos(carAngle * PI / 180), 5, carPositionZ - 20 * sin(carAngle * PI / 180));
    glEnd();

	glBegin(GL_LINES);
        glVertex3f(carPositionX, 5, carPositionZ);
        glVertex3f(carPositionX - 20 * sin(carAngle * PI / 180), 5, carPositionZ - 20 * cos(carAngle * PI / 180));
    glEnd();
	
	//Base Azul
	glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
			glVertex3f(-500.0, 0.0, 500.0);
			glVertex3f(500.0, 0.0, 500.0);
			glVertex3f(500.0, 0.0, -500.0);
			glVertex3f(-500.0, 0.0, -500.0);
		glEnd();
	glPopMatrix();

	// Executa os comandos OpenGL
	glutSwapBuffers();
 }

// Inicializa parâmetros de rendering
void Inicializa(void)
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
}

// Função usada para especificar o volume de visualização
void configViewMode(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	gluPerspective(angle,fAspect,0.5,500);
	
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	
	gluLookAt(camPositionX,camPositionY,camPositionZ,carPositionX,carPositionY + 15,carPositionZ,0,1,0);
	//gluLookAt(0,400,0,0,0,0,0,0,1);
}

 void update(int value) {

	if (isBraking)
		carSpeed = 0.1 * carSpeed;

	if (isSpeedingUp && carSpeed <= carSpeedMax)
		carSpeed += carAcceleration;
	else if (!isSpeedingUp && carSpeed > 0)
		carSpeed -= (0.5/carSpeed);
	
	camPositionX = carPositionX - distanceCamFromCar * cos(carAngle * PI / 180);
	camPositionZ = carPositionZ + distanceCamFromCar * sin(carAngle * PI / 180);

	if (carSpeed > 0){

        carPositionX += carSpeed * cos(carAngle * PI / 180);
        carPositionZ -= carSpeed * sin(carAngle * PI / 180);

		configViewMode();
		glutPostRedisplay();
	}
	glutTimerFunc(10, update, 0);
}

// Função callback chamada quando o tamanho da janela é alterado 
void chageWindow(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica o tamanho da viewport
    glViewport(0, 0, w, h);
 
	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	configViewMode();
}

void keyboardEvent(unsigned char key, int x, int y)
{
	switch(key){
		case 27:
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
	}
}

void specialEvent(int key, int x, int y) {
	switch(key){
		//glVertex3f(carPositionX - 20 * sin(carAngle * PI / 180), 5, carPositionZ - 20 * cos(carAngle * PI / 180));
		case GLUT_KEY_LEFT:
			/*
			if (carSidesActions < carSidesMax){
				carSidesActions += carSidesDescolation;
				carPositionX -= carSidesDescolation * sin(carAngle * PI / 180);
				carPositionZ -= carSidesDescolation * cos(carAngle * PI / 180);
			}*/
			carAngle += 1.0f;
			break;
		case GLUT_KEY_RIGHT:
			/*
			if (carSidesActions > -carSidesMax){
				carSidesActions -= carSidesDescolation;
				carPositionX += carSidesDescolation * sin(carAngle * PI / 180);
				carPositionZ += carSidesDescolation * cos(carAngle * PI / 180);
			}*/
			carAngle -= 1.0f;
			break;
		case GLUT_KEY_DOWN:
			isBraking = 1;
			break;
		case GLUT_KEY_UP:
			isSpeedingUp = 1;
			break;
	}
	configViewMode();
	glutPostRedisplay();
} 


void specialKeyReleased(int key, int x, int y) {
	switch(key){
		case GLUT_KEY_UP:
			isSpeedingUp = 0;
			break;
		case GLUT_KEY_LEFT:
			//carGoLeft = 0;
			break;
		case GLUT_KEY_RIGHT:
			//carGoRight = 0;
			break;
		case GLUT_KEY_DOWN:
			isBraking = 0;
			break;
	}
	configViewMode();
	glutPostRedisplay();
} 

// Programa Principal 
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
	glutCreateWindow("Navegacao");
	glutDisplayFunc(draw);
    glutReshapeFunc(chageWindow);
	glutKeyboardFunc(keyboardEvent);
	glutSpecialFunc(specialEvent);
	glutSpecialUpFunc(specialKeyReleased);
	glutTimerFunc(25, update, 0);
	Inicializa();
	glutMainLoop();
}
