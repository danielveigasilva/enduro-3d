#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>

GLfloat fAspect;
GLfloat angle = 45;
GLfloat rotate_all_obj_x, rotate_all_obj_y = 0;
GLfloat lookAtEye[3] = {0.0, 30.0, 200.0};
GLfloat lookAtCenter[3] = {0.0, 30.0, 0.0};
GLfloat lookAtUnitVector[3] = {0.0, 0.0, 0.0};
GLfloat lookAtSpeedCam = 10;
GLfloat lookAtCenterCam = 0.07;
int mode = 0;
float alfa;
float delta;

// Função callback chamada para fazer o desenho
void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Chaleira Azul
	glPushMatrix();
		glRotatef(rotate_all_obj_x,1.0,0.0,0.0);
		glRotatef(rotate_all_obj_y,0.0,1.0,0.0);
		glColor3f(1.0f, 1.0f, 0.0f);
		glTranslatef(100.0, 33.0, 13.0);
		glRotatef(180,0.0,1.0,0.0);
		glTranslatef(0.0, 0.0, 0.0);
		glutSolidTeapot(20.0f);
	glPopMatrix();

	//Chaleira Vermelha
	glPushMatrix();
		glRotatef(rotate_all_obj_x,1.0,0.0,0.0);
		glRotatef(rotate_all_obj_y,0.0,1.0,0.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(-100.0, 33.0, 13.0);
		glutSolidTeapot(20.0f);
	glPopMatrix();

	//Cone Branco Esq
	glPushMatrix();
		glRotatef(rotate_all_obj_x,1.0,0.0,0.0);
		glRotatef(rotate_all_obj_y,0.0,1.0,0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(-50.0, 20.0, -80.0);
		glRotatef(-90,1.0,0.0,0.0);
		glTranslatef(0.0, 0.0, 0.0);
		glutSolidCone(20.0, 40.5, 50, 50);
	glPopMatrix();

	//Cone Branco Esq
	glPushMatrix();
		glRotatef(rotate_all_obj_x,1.0,0.0,0.0);
		glRotatef(rotate_all_obj_y,0.0,1.0,0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(50.0, 20.0, -80.0);
		glRotatef(-90,1.0,0.0,0.0);
		glTranslatef(0.0, 0.0, 0.0);
		glutSolidCone(20.0, 40.5, 50, 50);
	glPopMatrix();
	
	//Base Azul
	glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
			//glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(-500.0, 20.0, 500.0);
			glVertex3f(500.0, 20.0, 500.0);
			glVertex3f(500.0, 20.0, -500.0);
			glVertex3f(-500.0, 20.0, -500.0);
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
}

// Função usada para especificar o volume de visualização
void configViewMode(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	if (mode != 0)
		glOrtho(-100, 100, -100, 100, 30, 600);
	else
    	gluPerspective(angle,fAspect,0.5,500);
	
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	// Especifica posição do observador e do alvo

	for (int i = 0; i < 3; i++)
		lookAtCenter[i] -= lookAtEye[i];

	lookAtCenter[0] = lookAtCenter[0] * cos(alfa) + lookAtCenter[2] * sin(alfa);
	lookAtCenter[1] = lookAtCenter[1];
	lookAtCenter[2] = -lookAtCenter[0] * sin(alfa) + lookAtCenter[2] * cos(alfa);
	alfa = 0;

	for (int i = 0; i < 3; i++)
		lookAtCenter[i] += lookAtEye[i];

	for (int i = 0; i < 3; i++)
		lookAtUnitVector[i] = lookAtEye[i] - lookAtCenter[i];

	float length = sqrt(pow(lookAtUnitVector[0],2) + pow(lookAtUnitVector[1],2) + pow(lookAtUnitVector[2],2));
	
	for (int i = 0; i < 3; i ++){
		lookAtUnitVector[i] = length != 0 ? lookAtUnitVector[i]/length : lookAtUnitVector[i];	
		lookAtEye[i] += lookAtUnitVector[i] * delta;
		lookAtCenter[i] += lookAtUnitVector[i] * delta;
	}
	delta = 0;

	gluLookAt(lookAtEye[0],lookAtEye[1],lookAtEye[2],lookAtCenter[0],lookAtCenter[1],lookAtCenter[2],0,1,0);
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

// Função callback chamada para gerenciar eventos do mouse
void mouseEvent(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)  // Zoom-in
			if (angle >= 10) angle -= 5;

	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN)  // Zoom-out
			if (angle <= 130) angle += 5;

	configViewMode();
	glutPostRedisplay();
}

void keyboardEvent(unsigned char key, int x, int y)
{
	switch(key){
		case 27:
			exit(0);
			break;
		case 'p':
			mode = 0;
			break;
		case 'o':
			mode = 1;
			break;
	}
	configViewMode();
	glutPostRedisplay();
}

void specialEvent(int key, int x, int y) {
	switch(key){
		case GLUT_KEY_LEFT:
			alfa += lookAtCenterCam;
			//lookAtUnitVector[0] = -1;
			break;
		case GLUT_KEY_RIGHT:
			alfa -= lookAtCenterCam;
			//lookAtUnitVector[0] = 1;
			break;
		case GLUT_KEY_DOWN:
			delta += lookAtSpeedCam;
			//lookAtUnitVector[2] = 1;
			break;
		case GLUT_KEY_UP:
			delta -= lookAtSpeedCam;
			//lookAtUnitVector[2] = -1;
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
	glutMouseFunc(mouseEvent);
	glutKeyboardFunc(keyboardEvent);
	glutSpecialFunc(specialEvent);
	Inicializa();
	glutMainLoop();
}
