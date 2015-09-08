#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <GL/freeglut.h>
//#include <GL/glut.h>

#include <iostream>
using namespace std;

#include "Planeta.h"
#include "Satelite.h"
#include "Espiral.h"
#include "Panel.h"

// Freeglut parameters
// Flag telling us to keep processing events
// bool continue_in_main_loop= true; //(**)

// Viewport size
int WIDTH= 500, HEIGHT= 500;

// Viewing frustum parameters
GLdouble xRight=500, xLeft=-xRight, yTop=500, yBot=-yTop, N=-1000, F=10000;

// Camera parameters
GLdouble eyeX=100.0, eyeY=100.0, eyeZ=100.0;
GLdouble lookX=0.0, lookY=0.0, lookZ=0.0;
GLdouble upX=0, upY=1, upZ=0;

Planeta _planeta;
Satelite _satelite;
Espiral _espiral;
Panel _panel[4];

GLdouble _angulo_giro_sistema = 0.0; //sistema = planeta + satelite
GLdouble _posiX_sistema = 0.0;
GLdouble _posiZ_sistema = 0.0;
GLdouble _t = 0.0;

bool caras_planeta = true;
bool lineas_planeta = true;
bool caras_satelite = true;
bool lineas_satelite = true;
bool caras_espiral = false;
bool lineas_espiral = true;

void initGL() {	 		 
	glClearColor(0.6f,0.7f,0.8f,1.0);
    glEnable(GL_LIGHTING);    

    // Light0
    glEnable(GL_LIGHT0);
    GLfloat d[]={1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, d);
    GLfloat a[]={0.3f,0.3f,0.3f,1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, a);
	GLfloat p[]={25.0, 25.0, 0.0, 1.0};	 
	glLightfv(GL_LIGHT0, GL_POSITION, p);

	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.1f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);

	glEnable(GL_CULL_FACE); //oculta las caras traseras

	// buildSceneObjects();


	// Camera set up
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);

	// Frustum set up
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();     
	glOrtho(xLeft, xRight, yBot, yTop, N, F);

	// Viewport set up
    glViewport(0, 0, WIDTH, HEIGHT);        
 }

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

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

	//mueve el eje de coordenadas como considera oportuno
	glMatrixMode(GL_MODELVIEW); //se activa la pila de matrices
	
	glPushMatrix(); //se duplica la cima de la pila
	//se aplican las transformaciones sucesivas, aplicandose la ultima matriz que hayamos aplicado (glTranslatef, glRotatef, glScalef)
		glTranslatef(_posiX_sistema, 0.0, _posiZ_sistema); //desplaza el sistema del origen
		glPushMatrix(); 
			glRotatef(_angulo_giro_sistema, 0.0, 1.0, 0.0);	//gira el sistema sobre si mismo
			glPushMatrix(); 	
				glTranslatef(2*radio_planeta, 0.0, 0.0);
				glRotatef(90.0, 0.0, 0.0, 1.0);
					_satelite.draw(caras_satelite, lineas_satelite);
					glRotatef(3*_angulo_giro_sistema, 0.0, 1.0, 0.0);
					glPushMatrix(); 
						_panel[0].draw(caras_satelite, lineas_satelite);
						glRotatef(90.0, 0.0, 1.0, 0.0);
						_panel[0].draw(caras_satelite, lineas_satelite);
						glRotatef(90.0, 0.0, 1.0, 0.0);
						_panel[0].draw(caras_satelite, lineas_satelite);
						glRotatef(90.0, 0.0, 1.0, 0.0);
						_panel[0].draw(caras_satelite, lineas_satelite);
					glPopMatrix();
			glPopMatrix(); 
		glPopMatrix(); 

		glPushMatrix();
			glRotatef(_angulo_giro_sistema, 0.0, -1.0, 0.0);
			glPushMatrix();
				glRotatef(90.0, 1.0, 0.0, 0.0);
					_planeta.draw(caras_planeta, lineas_planeta); //relleno: GL_FILL GLU_LINE
			glPopMatrix();
		glPopMatrix();
	glPopMatrix(); //se elimina la cima duplicada
	
	_espiral.draw(caras_espiral, lineas_espiral);

	glFlush();
	glutSwapBuffers();
}


void resize(int newWidth, int newHeight) {
	WIDTH= newWidth;
	HEIGHT= newHeight;
	GLdouble RatioViewPort= (float)WIDTH/(float)HEIGHT;
	glViewport (0, 0, WIDTH, HEIGHT) ;
   
	GLdouble SVAWidth= xRight-xLeft;
	GLdouble SVAHeight= yTop-yBot;
	GLdouble SVARatio= SVAWidth/SVAHeight;
	if (SVARatio >= RatioViewPort) {		 
		GLdouble newHeight= SVAWidth/RatioViewPort;
		GLdouble yMiddle= ( yBot+yTop )/2.0;
		yTop= yMiddle + newHeight/2.0;
		yBot= yMiddle - newHeight/2.0;
    }
	else {      
		GLdouble newWidth= SVAHeight*RatioViewPort;
		GLdouble xMiddle= ( xLeft+xRight )/2.0;
		xRight= xMiddle + newWidth/2.0;
		xLeft=  xMiddle - newWidth/2.0;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();   
	glOrtho(xLeft, xRight, yBot, yTop, N, F);
}



	//----------ACTUALIZAR MATRIZ AVE----------
	void actualizarAVE(GLdouble xL, GLdouble xR, GLdouble yB, GLdouble yT, GLdouble n, GLdouble f) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();   
		glOrtho(xL, xR, yB, yT, n, f);
	}

	//----------TRASLACIÓN PV----------
	void trasladar(GLdouble x, GLdouble y)
	{
		xLeft += x; xRight += x;
		yTop += y;  yBot += y;

		actualizarAVE(xLeft, xRight, yBot, yTop, N, F);		
	}

	void trasladarHorizontal(GLdouble x)
	{
		trasladar(x, 0);
	}

	void trasladarVertical(GLdouble y)
	{
		trasladar(0, y);
	}



	//----------ACTUALIZAR MATRIZ CAMARA----------
	void actualizarCAM(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble lookX, GLdouble lookY, GLdouble lookZ, GLdouble upX, GLdouble upY, GLdouble upZ) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);	
	}

	//----------TRASLACIÓN CAM----------
	void trasladar(GLdouble x, GLdouble y, GLdouble z)
	{
		eyeX += x;
		eyeY += y;
		eyeZ += z;

		actualizarCAM(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);		
	}

	void trasladarX(GLdouble x)
	{
		trasladar(x, 0, 0);
	}

	void trasladarY(GLdouble y)
	{
		trasladar(0, y, 0);
	}

	void trasladarZ(GLdouble z)
	{
		trasladar(0, 0, z);
	}
	

	//----------ZOOM PV----------
	void zoom(GLdouble factorEscala) //MIRAR SI AL HACER ZOOM SE TIENE QUE MODIFICAR N o F
	{	
		GLdouble centroX = (xRight + xLeft)/2;
		GLdouble centroY = (yTop + yBot)/2;

		GLdouble nuevoAncho = (xRight - xLeft)/factorEscala;
		GLdouble nuevoAlto = (yTop - yBot)/factorEscala;

		//aplicamos el zoom
		xLeft = centroX - nuevoAncho/2;
		xRight = centroX + nuevoAncho/2;
		yBot = centroY - nuevoAlto/2;
		yTop = centroY + nuevoAlto/2;

		actualizarAVE(xLeft, xRight, yBot, yTop, N, F);
	}

	void zoomPercent(GLdouble percent)
	{
		zoom(percent/100);
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



	void modificarPosicionSistema(GLdouble t)
	{
		PV3D desp; //desplazamiento de la esfera y satelite en "t"
		GLdouble m[16]; //matriz de deplazamiento

		_espiral.calculaMatrizTransformacion(m, t);
		desp = _espiral.aplicaMatrizAPunto(m, PV3D());
		_posiX_sistema = desp.get_x();
		_posiZ_sistema = desp.get_z();
	}

	void key(unsigned char key, int x, int y){
	bool need_redisplay = true;
	switch (key) {
		case 27:  /* Escape key */
			//continue_in_main_loop = false; // (**)
			//Freeglut's sentence for stopping glut's main loop (*)
			glutLeaveMainLoop (); 
			break;

		case 'w' :
			trasladarVertical( (yTop - yBot)/10);
			break ;

		case 's' :
			trasladarVertical( -(yTop - yBot)/10);
			break ;
		case 'a' :
			trasladarHorizontal(-(xRight - xLeft)/10 );
			break ;

		case 'd' :
			trasladarHorizontal((xRight - xLeft)/10 );
			break ;

		case 'i' :
			trasladarY(5.0);
			break ;

		case 'k' :
			trasladarY(-5.0);
			break ;
		case 'j' :
			trasladarX(-5.0);
			break ;

		case 'l' :
			trasladarX(5.0);
			break ;

		case 'y' :
			trasladarZ(-5.0);
			break ;

		case 'h' :
			trasladarZ(5.0);
			break ;

		case 'o' :
			_angulo_giro_sistema = _angulo_giro_sistema + 2.5;
			if(_t >= 1.0)
			{
				_t = _t - 1.0;			
				modificarPosicionSistema(_t);
			}
			break ;

		case 'p' :
			_angulo_giro_sistema = _angulo_giro_sistema - 2.5;
			_t = _t + 1.0;
			modificarPosicionSistema(_t);
			break ;

		case 'r' :
			if(lineas_planeta)
				lineas_planeta = false;
			else
				lineas_planeta = true;
			break ;

		case 't' :
			if(caras_planeta)
				caras_planeta = false;
			else
				caras_planeta = true;
			break ;

		case 'f' :
			if(lineas_satelite)
				lineas_satelite = false;
			else
				lineas_satelite = true;
			break ;

		case 'g' :
			if(caras_satelite)
				caras_satelite = false;
			else
				caras_satelite = true;
			break ;

		case 'v' :
			if(lineas_espiral)
				lineas_espiral = false;
			else
				lineas_espiral = true;
			break ;

		case 'b' :
			if(caras_espiral)
				caras_espiral = false;
			else
				caras_espiral = true;
			break ;
			
		default:
			need_redisplay = false;
			break;
	}

	if (need_redisplay)
		glutPostRedisplay();
}


//----------mouseWheel----------
void mouseWheel(int button, int state, int x, int y)
{
	if (state > 0) // Zoom IN
    {
		zoomPercent(125);
    }
    else // Zoom OUT
    {
		zoomPercent(80);
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
	glutMouseWheelFunc(mouseWheel); //añadidas por mí para controlar eventos del ratón

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
