#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <GL/freeglut.h>
//#include <GL/glut.h>

#include <iostream>
using namespace std;

#include "Camara.h"
#include "MunecoNieve.h"
#include "Suelo.h"
#include "TextureLoader.h"

#define TEXTURA_WOOD 1
#define TEXTURA_SNOW 2

#define PICK_TOL 10 //tolerancia en pixels del picking
#define PICK_BUFFER_SIZE 1024 //tamaño del buffer de picking

#define RENDER 1 //modo de solo renderizado
#define SELECT 2 //modo de seleccion



// Freeglut parameters
// Flag telling us to keep processing events
// bool continue_in_main_loop= true; //(**)

bool isLeftKeyPressed=false, isRightKeyPressed=false, 
	isDownKeyPressed=false, isUpKeyPressed=false;

// Viewport size
int WIDTH= 500, HEIGHT= 500;

// Rotacion respecto a los ejes mundiales
GLfloat _angle_x=0.0, _angle_y=0.0, _angle_z=0.0;


Camara* cam;
MunecoNieve* muneco;
Suelo* suelo;


bool _luz_ambiente_on = true;
bool _luz_remota_on = true;
bool _focos_ojos_on = true;

bool _niebla_on = false;

GLuint pickBuffer[PICK_BUFFER_SIZE]; // picking buffer 
int mode = RENDER; // GL_RENDER or GL_SELECT 
int cursorX,cursorY;
GLint global_hits;

Cilindro cil;
//¿por qué cuando se crean los objetos dentro de initGL (llamo al constructor) y no tengo un ciclindro, me pinta los objetos con las caras invertidas???????
//es como que no llamara al constructor del ObjetoCuadrico


//---------- initGL ----------
void initGL() {	 		 
	glClearColor(0.6f,0.7f,0.8f,1.0);
    glEnable(GL_LIGHTING);  

	// creación de la cámara
	cam = new Camara();

    // LUZ AMBIENTE
	GLfloat amb[] = {0.2, 0.2, 0.2, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb); //0.2 en todo por defecto

	//LUZ DIRECCIONAL en 45º con XZ
    glEnable(GL_LIGHT0);
    GLfloat d[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, d);
    GLfloat a[] = {0.2, 0.2, 0.2, 0.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, a);
	GLfloat p[] = {1.0, 1.0, 1.0, 0.0};	 //el 4º cero indica direccional
	glLightfv(GL_LIGHT0, GL_POSITION, p);


	//activa las TEXTURAS
	unsigned int width=700, height=700;
	unsigned char * imagen = loadBMPRaw("wood.bmp", width, height);
	// Create one OpenGL texture
	GLuint textureID_wood;
	glGenTextures(TEXTURA_WOOD, &textureID_wood);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID_wood);

	// trilinear filtering.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //tiling (GL_CLAMP para clamping)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //tiling
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//puede ser NEAREST o 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	//glGenerateMipmap(GL_TEXTURE_2D);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //el color de la textura se multiplica por el del pixel (GL_MODULATE)

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagen);
			
	delete[] imagen;
	

	//COLOR y MATERIALES
	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.1f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT); //iluminacion plana (para cada cara)
	//glEnable(GL_CULL_FACE); //oculta las caras traseras


	// ----------- NIEBLA
	// http://www.glprogramming.com/red/chapter06.html
	//glEnable(GL_FOG); 
	glFogf(GL_FOG_DENSITY, 0.001f);
	glFogi(GL_FOG_START, 0);
	glFogi(GL_FOG_END, 10000);
	GLfloat fog_color[4] = {0.9, 0.9, 0.9, 1.0};
	glFogfv(GL_FOG_COLOR, fog_color);
	glFogi(GL_FOG_MODE, GL_EXP2); //GL_LINEAR


	//------------ PICKING 
	// http://www.lighthouse3d.com/opengl/picking/
	// http://web.engr.oregonstate.edu/~mjb/cs553/Handouts/Picking/picking.pdf

	//glLoadName(1); //nombre del objeto para el picking
	// NO se puede poner un LoadName entre un glBegin y un glEnd
	//void glLoadName(GLunit name); //This function replaces the top of the stack with name. It is the same as calling 
									//glPopName();
									//glPushName(name);
	//otra forma de asignar unnombre es: 
	//glPushName(int);
		//drawObject();
	//glPopName();


	// buildSceneObjects();
	muneco = new MunecoNieve();
	suelo = new Suelo();
	muneco->get_mT()->traslada(new PV3D(100.0, 0.0, 0.0, false));

	// Viewport set up
    glViewport(0, 0, WIDTH, HEIGHT);        
 }

void interuptorNiebla() {
	if(_niebla_on)
		glDisable(GL_FOG);
	else
		glEnable(GL_FOG); 
	_niebla_on = ! _niebla_on;
}

// INICIA EL PICKING
void startPicking(int cursorX, int cursorY) {

	GLint viewport[4];
	/* setup the picking buffer: */ 
	glSelectBuffer(PICK_BUFFER_SIZE, pickBuffer);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glRenderMode(GL_SELECT);

	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(cursorX, viewport[3]-cursorY, 5, 5, viewport);
	gluPerspective(25, 1.0, 1, 10000);
	glMatrixMode(GL_MODELVIEW);
}

//procesa los clicks del picking
void processHits(GLint hits, GLuint buffer[])
{
   unsigned int i, j, numberOfNames;
   GLuint names, *ptr, minZ,*ptrNames;

   printf ("hits = %d\n", hits);
   ptr = (GLuint *) buffer;
   minZ = 0xffffffff;
   for (i = 0; i < hits; i++) {	
      names = *ptr;
	  ptr++;
	  if (*ptr < minZ) {
		  numberOfNames = names;
		  minZ = *ptr;
		  ptrNames = ptr+2;
	  }
	  
	  ptr += names+2;
	}
	if (numberOfNames > 0) {
		//activa-desactiva la niebla
		interuptorNiebla();

		printf ("Has pinchado sobre:  ");
		ptr = ptrNames;
		for (j = 0; j < numberOfNames; j++,ptr++) { 
			printf ("%d ", *ptr);
		}
	}
	else
		printf("No has pinchado sobre nada con nombre asignado");
	printf ("\n");
   
}

// DETIENE EL PICKING
void stopPicking() {

	// restoring the original projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	
	// returning to normal rendering mode
	global_hits = glRenderMode(GL_RENDER);
	
	// if there are hits process them
	if (global_hits != 0)
		processHits(global_hits, pickBuffer);

	mode = RENDER;
}

//----------EVENTO display----------
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

	//inicia el picking si está en modo selección
	if (mode == SELECT) {
		startPicking(cursorX, cursorY);
	}

	GLfloat p[] = {1.0, 1.0, 1.0, 0.0};	 //actualiza la posición de la luz
	glLightfv(GL_LIGHT0, GL_POSITION, p);

	glMatrixMode(GL_MODELVIEW); //se activa la pila de matrices

	glPushMatrix(); //se duplica la cima de la pila
	//se aplican las transformaciones sucesivas, aplicandose la ultima matriz que hayamos aplicado (glTranslatef, glRotatef, glScalef)

	glRotatef(_angle_x, 1.0, 0.0, 0.0); //rota respecto a los ejes mundiales
	glRotatef(_angle_y, 0.0, 1.0, 0.0);
	glRotatef(_angle_z, 0.0, 0.0, 1.0);

		// Drawing axes
		glBegin( GL_LINES );
			glColor3f(1.0,0.0,0.0); 
			glVertex3f(0, 0, 0);
			glVertex3f(1000, 0, 0);	     
	 
			glColor3f(0.0,1.0,0.0); 
			glVertex3f(0, 0, 0);
			glVertex3f(0, 1000, 0);	 
	 
			glColor3f(0.0,0.0,1.0); 
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 1000);	     
		glEnd();

		muneco->draw();			
		suelo->draw();

	glPopMatrix();


	if (mode == SELECT) 
		stopPicking();
	else
		glutSwapBuffers();
}

	
//----------REDIMENSIONAR AVE----------
void resize(int newWidth, int newHeight) {
	WIDTH= newWidth;
	HEIGHT= newHeight;
	GLdouble RatioViewPort= (float)WIDTH/(float)HEIGHT;
	glViewport (0, 0, WIDTH, HEIGHT) ;
   
	GLdouble SVAWidth= cam->get_xRight()-cam->get_xLeft();
	GLdouble SVAHeight= cam->get_yTop()-cam->get_yBot();
	GLdouble SVARatio= SVAWidth/SVAHeight;
	if (SVARatio >= RatioViewPort) {		 
		GLdouble newHeight= SVAWidth/RatioViewPort;
		GLdouble yMiddle= ( cam->get_yBot()+cam->get_yTop() )/2.0;
		cam->set_yTop(yMiddle + newHeight/2.0);
		cam->set_yBot(yMiddle - newHeight/2.0);
	}
	else {      
		GLdouble newWidth= SVAHeight*RatioViewPort;
		GLdouble xMiddle= ( cam->get_xLeft()+cam->get_xRight() )/2.0;
		cam->set_xRight(xMiddle + newWidth/2.0);
		cam->set_xLeft(xMiddle - newWidth/2.0);
	}

	cam->actualizarAVE();
}

//----------MODIFICAR LUZ AMBIENTE----------
void interruptorLuzAmbiente() {
	GLfloat amb[4];

	if(_luz_ambiente_on)
	{
		amb[0] = 0.0; amb[1] = 0.0; amb[2] = 0.0; amb[3] = 1.0;
	}
	else
	{
		amb[0] = 0.2; amb[1] = 0.2; amb[2] = 0.2; amb[3] = 1.0;
	}

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	_luz_ambiente_on = ! _luz_ambiente_on;
}

//----------MODIFICAR LUZ REMOTA----------
void interruptorLuzRemota() {
	if(_luz_remota_on)
		glDisable(GL_LIGHT0);
	else
		glEnable(GL_LIGHT0);
	_luz_remota_on = ! _luz_remota_on;
}

//----------MODIFICAR FOCOS DE LOS OJOS----------
void interruptorLuzOjos() {
	if(_focos_ojos_on)
	{
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	}
	else
	{
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}
	_focos_ojos_on = ! _focos_ojos_on;
}

	//----------ESCRIBIR TEXTO----------
	void dibujarChar(char c, void *fuente, GLdouble x, GLdouble y, GLdouble z)
	{
		glRasterPos3d (x, y, z);
		glutBitmapCharacter(fuente, c); 
	}

	void dibujarString (char *s, void *fuente, GLdouble x, GLdouble y, GLdouble z) //ver si declararlo como void inline drawString
	{ 
		glRasterPos3d (x, y, z);

		for (unsigned int i=0; i<strlen(s); i++) 
			glutBitmapCharacter(fuente, s[i]); 
	} 

	void dibujarInt (int n, void *fuente, GLdouble x, GLdouble y, GLdouble z)
	{
		char buffer [10];
		itoa (n,buffer,10);
		dibujarString(buffer, fuente, x, y, z);
	}


//----------EVENTO key----------
void specialKey(int key, int x, int y)
{	
	bool need_redisplay = true;
	switch (key) {
		case GLUT_KEY_LEFT:
			isLeftKeyPressed = true;
			if (!isRightKeyPressed) {
				cam->moverPVIzquierda();
			}
			break;

		case GLUT_KEY_RIGHT:
			isRightKeyPressed = true;
			if (!isLeftKeyPressed) {
				cam->moverPVDerecha();
			}
			break;

		case GLUT_KEY_UP:
			isUpKeyPressed = true;
			if (!isDownKeyPressed) {
				cam->moverPVArriba();
			}
			break;

		case GLUT_KEY_DOWN:
			isDownKeyPressed = true;
			if (!isUpKeyPressed) {
				cam->moverPVAbajo();
			}
			break;

		default:
			need_redisplay = false;
			break;
	}

	if (need_redisplay)
		glutPostRedisplay();
}

void handleSpecialKeyReleased(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			isLeftKeyPressed = false;
			break;

		case GLUT_KEY_RIGHT:
			isRightKeyPressed = false;
			break;

		case GLUT_KEY_UP:
			isUpKeyPressed = false;
			break;

		case GLUT_KEY_DOWN:
			isDownKeyPressed = false;
			break;
	}
}

void key(unsigned char key, int x, int y){
	bool need_redisplay = true;
	switch (key) {
		case 27:  /* Escape key */
			//continue_in_main_loop = false; // (**)
			//Freeglut's sentence for stopping glut's main loop (*)
			glutLeaveMainLoop (); 
			break;

		case 'a' :
			_angle_x = _angle_x + 10.0;
			break ;

		case 'z' :
			_angle_x = _angle_x - 10.0;
			break ;

		case 's' :
			_angle_y = _angle_y + 10.0;
			break ;

		case 'x' :
			_angle_y = _angle_y - 10.0;
			break ;

		case 'd' :
			_angle_z = _angle_z + 10.0;
			break ;

		case 'c' :
			_angle_z = _angle_z - 10.0;
			break ;
		
		case 'o' :
			cam->setPerspectiva(false);
			break ;

		case 'p' :
			cam->setPerspectiva(true);
			break ;

		case '1' : 
			cam->girarEnEje(10.0, cam->X);
			break ;

		case '2' : 
			cam->girarEnEje(10.0, cam->Y);
			break ;

		case '3' : 
			cam->girarEnEje(10.0, cam->Z);
			break ;

		case '4' : 
			cam->vistaLateral();
			break ;

		case '5' : 
			cam->vistaFrontal();
			break ;

		case '6' : 
			cam->vistaCenital();
			break ;

		case '7' : 
			cam->vistaEsquina();
			break ;
		
		case 'g' :
			cam->trasladar(0.0, 10.0, 0.0);
			break ;

		case 'f' :
			cam->trasladar(0.0, -10.0, 0.0);
			break ;

		case 'r' :
			cam->trasladar(-10.0, 0.0, 0.0);
			break ;

		case 't' :
			cam->trasladar(10.0, 0.0, 0.0);
			break ;

		case 'v' :
			cam->trasladar(0.0, 0.0, -10.0);
			break ;

		case 'b' :
			cam->trasladar(0.0, 0.0, 10.0);
			break ;

		case 'u' :
			cam->roll(10.0);
			break ;

		case 'i' :
			cam->roll(-10.0);
			break ;

		case 'j' :
			cam->yaw(10.0);
			break ;

		case 'k' :
			cam->yaw(-10.0);
			break ;

		case 'n' :
			cam->pitch(10.0);
			break ;

		case 'm' :
			cam->pitch(-10.0);
			break ;

		case 'q' :
			interruptorLuzAmbiente();
			break ;

		case 'w' :
			interruptorLuzRemota();
			break ;

		case 'e' :
			interruptorLuzOjos();
			break ;

		case 'W' :
			muneco->avanza(10.0);
			break ;

		case 'S' :
			muneco->avanza(-10.0);
			break ;

		case 'A' :
			muneco->set_rotacion_angulo(muneco->get_rotacion_angulo() + 10.0);
			break ;

		case 'D' :
			muneco->set_rotacion_angulo(muneco->get_rotacion_angulo() - 10.0);
			break ;
			
		default:
			need_redisplay = false;
			break;
	}

	if (need_redisplay)
		glutPostRedisplay();
}


//----------mouse----------
void mouse(int button, int state, int x, int y)
{
	bool need_redisplay = true;

	switch(button) {
	case GLUT_LEFT_BUTTON:
		printf(" LEFT ");
		if (state == GLUT_DOWN) {
			printf("DOWN\n");
			
				//guardamos la posición del click y ponemos el modo SELECCIONAR como ACTIVADO
				cursorX = x;
				cursorY = y;
				mode = SELECT;

		}
		else if (state == GLUT_UP) {
			printf("UP\n");
			

		}
		break;

	case GLUT_RIGHT_BUTTON:
		printf(" RIGHT ");
		if (state == GLUT_DOWN) {
			printf("DOWN\n");
			

		}
		else  if (state == GLUT_UP) {
			printf("UP\n");
			

		}
		break;

    default:
		need_redisplay = false;
		break;

  }//switch

  if (need_redisplay)
	glutPostRedisplay();

  fflush(stdout);              // Force output to stdout

}

//----------EVENTO mouseWheel----------
void mouseWheel(int button, int state, int x, int y)
{
	if (state > 0) // Zoom IN
    {
		cam->zoomPercent(125);
    }
    else // Zoom OUT
    {
		cam->zoomPercent(80);
    }

    glutPostRedisplay();
}



int main(int argc, char *argv[]){
	cout<< "Starting console..." << endl;

	int my_window; // my window's identifier

	// Initialization
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition (140, 140);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	// Window construction
	my_window = glutCreateWindow("Freeglut 3D-project");
    
	// Callback registration
	glutReshapeFunc(resize);
	glutKeyboardFunc(key);
	glutDisplayFunc(display);
	glutMouseFunc(mouse); //eventos del ratón
	glutMouseWheelFunc(mouseWheel); 
	glutSpecialFunc(specialKey); //teclas especiales
	glutSpecialUpFunc(handleSpecialKeyReleased);

	// OpenGL basic setting
	initGL();

	// Freeglut's main loop can be stopped executing (**)
	// while (continue_in_main_loop) glutMainLoopEvent();

	// Classic glut's main loop can be stopped after X-closing the window,
	// using the following freeglut's setting (*)
	glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION) ;
    
	// Classic glut's main loop can be stopped in freeglut using (*)
	glutMainLoop(); 
  
	// We would never reach this point using classic glut
	system("PAUSE"); 
   
	return 0;
}
