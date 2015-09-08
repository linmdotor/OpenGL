/**
@author Jesús Martínez Dotor
*/

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <GL/freeglut.h>
//#include <GL/glut.h>

#include <iostream>
using namespace std;

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Escena.h"

// Freeglut parameters
// Flag telling us to keep processing events
// bool continue_in_main_loop= true; //(**)

void display(void);

// Viewport size
int xInit= 0, yInit= 0;
int WIDTH= 500, HEIGHT= 250;

// Número de columnas del embaldosado
int nCols = 1;

Escena _escena = Escena(0.0, 500.0, 0.0, 250.0, &xInit, &yInit, &WIDTH, &HEIGHT, display);

//----------initGl----------
void intitGL() {
	glClearColor(1.0,1.0,1.0,1.0);
	glColor3f(1.0,0.0,0.0); 

	glPointSize(4.0);
	glLineWidth(2.0);

	// Viewport
    glViewport(xInit, yInit, WIDTH, HEIGHT);
    
	// Model transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	_escena.actualizarAVE(_escena.get_xLeft(), _escena.get_xRight(), _escena.get_yBot(), _escena.get_yTop());
 }

//----------resize----------
void resize(int newWidth, int newHeight){
	//Resize Viewport
	WIDTH= newWidth;
	HEIGHT= newHeight;
	GLdouble RatioViewPort= (float)WIDTH/(float)HEIGHT;
	glViewport ( xInit, yInit, WIDTH, HEIGHT ) ;
  
	//Resize Scene Visible Area 
	//Se actualiza el área visible de la escena
	//para que su ratio coincida con ratioViewPort
	GLdouble SVAWidth= _escena.get_xRight() - _escena.get_xLeft();
	GLdouble SVAHeight= _escena.get_yTop() - _escena.get_yBot();
	GLdouble SVARatio= SVAWidth/SVAHeight;
	if (SVARatio >= RatioViewPort) {
		// Increase SVAHeight
		GLdouble newHeight= SVAWidth/RatioViewPort;
		GLdouble yMiddle= ( _escena.get_yBot() + _escena.get_yTop() )/2.0;
		_escena.set_yTop( yMiddle + newHeight/2.0);
		_escena.set_yBot( yMiddle - newHeight/2.0);
		}
	else {
		//Increase SVAWidth
		GLdouble newWidth= SVAHeight*RatioViewPort;
		GLdouble xMiddle= ( _escena.get_xLeft() + _escena.get_xRight() )/2.0;
		_escena.set_xRight( xMiddle + newWidth/2.0);
		_escena.set_xLeft(  xMiddle - newWidth/2.0);
	}

	_escena.actualizarAVE(_escena.get_xLeft(), _escena.get_xRight(), _escena.get_yBot(), _escena.get_yTop());
}

//----------key----------
void key(unsigned char key, int x, int y) {
 
  bool need_redisplay = true;

  switch (key) {
  case 27:  /* Escape key */
    //continue_in_main_loop = false; // (**)
	glutLeaveMainLoop (); //Freeglut's sentence for stopping glut's main loop (*)
    break;

  case 'w' :
	_escena.trasladarVertical( (_escena.get_yTop() - _escena.get_yBot())/10);
    break ;

  case 's' :
    _escena.trasladarVertical( -(_escena.get_yTop() - _escena.get_yBot())/10);
    break ;
  case 'a' :
    _escena.trasladarHorizontal(-(_escena.get_xRight() - _escena.get_xLeft())/10 );
    break ;

  case 'd' :
    _escena.trasladarHorizontal((_escena.get_xRight() - _escena.get_xLeft())/10 );
    break ;

  case '-' :
	//zoomPercent(50, (xRight+xLeft)/2, (yTop+yBot)/2);
	_escena.zoomPercent(50);
    break ;

  case '+' :
	//zoomPercent(200, (xRight+xLeft)/2, (yTop+yBot)/2);
	_escena.zoomPercent(200);
    break ;

  case '/' :
	_escena.zoomProgresivo(0.5, 1.5);
    break ;

  case '*' :
	_escena.zoomProgresivo(2.0, 1.5);
    break ;

  case 'e' :
	_escena.anidar();
    break ;

  case 'q' :
	_escena.desanidar();
    break ;

  case 'z' :
	cout << "Introduzca el numero de columnas a embaldosar: " ;
	cin >> nCols;
    break ;

  default:
    need_redisplay = false;
    break;

  }//switch

  if (need_redisplay)
    glutPostRedisplay();
}

//----------display----------
void display(void) {
  glClear( GL_COLOR_BUFFER_BIT );

  // Scene rendering
  _escena.embaldosar(nCols);

  glFlush();
  glutSwapBuffers();
}

//----------mouse----------
void mouse(int button, int state, int x, int y)
{
	//se transforma en función del nCols
	x = (x%(WIDTH/nCols))*nCols;
	y = (y%(HEIGHT/nCols))*nCols;

	bool need_redisplay = true;

	switch(button) {
	case GLUT_LEFT_BUTTON:
		printf(" LEFT ");
		if (state == GLUT_DOWN) {
			printf("DOWN\n");
			printf("PV (%d, %d) - AVE (%f, %f)\n", x, y, _escena.PVtoAVE_X(x), _escena.PVtoAVE_Y(y));
		}
		else if (state == GLUT_UP) {
			printf("UP\n");
			printf("PV (%d, %d) - AVE (%f, %f)\n", x, y, _escena.PVtoAVE_X(x), _escena.PVtoAVE_Y(y));
			_escena.seleccionar(x, y);
		}
		break;

	case GLUT_RIGHT_BUTTON:
		printf(" RIGHT ");
		if (state == GLUT_DOWN) {
			printf("DOWN\n");
			printf("PV (%d, %d) - AVE (%f, %f)\n", x, y, _escena.PVtoAVE_X(x), _escena.PVtoAVE_Y(y));
		}
		else  if (state == GLUT_UP) {
			printf("UP\n");
			printf("PV (%d, %d) - AVE (%f, %f)\n", x, y, _escena.PVtoAVE_X(x), _escena.PVtoAVE_Y(y));

			//pinta el primer cuadrado
			cout << "- Introduce el lado del cuadrado: ";
			GLdouble lado;
			cin >> lado;
			_escena.set_lado(lado);
			cout << "- Introduce el angulo derecho del arbol: ";
			GLdouble angulo;
			cin >> angulo;
			_escena.set_anguloderecho(angulo);
			Cuadrado raiz = Cuadrado(Punto(_escena.PVtoAVE_X(x),_escena.PVtoAVE_Y(y)), lado);
			raiz.set_colorrelleno(RGBColor(0.3, 0.2, 0.1));
			raiz.set_linea(false);
			_escena.set_arbol(Arbol(raiz, angulo));
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

//----------mouseWheel----------
void mouseWheel(int button, int state, int x, int y)
{
	if (state > 0) // Zoom IN
    {
		//zoomPercent(125, x, y);
		_escena.zoomPercent(125);
    }
    else // Zoom OUT
    {
		//zoomPercent(80, x, y);
		_escena.zoomPercent(80);
    }

    glutPostRedisplay();
}








int main(int argc, char *argv[]) {
/* initialize random seed: */
  srand (time(NULL));

  cout<< "Starting console..." << endl;
  cout<< endl << "TECLAS: a, q, w, e, a, s, d, z, +, -, *, /, mouse wheel, right and left click" << endl;

  int my_window; //my window's identifier

  cout << endl << "Introduzca el numero de columnas a embaldosar: " ;
  cin >> nCols;

  //Initialization
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition (140, 140);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
  glutInit(&argc, argv);

  //Window construction
  my_window = glutCreateWindow( "Freeglut 2D-project" );
    
  //Callback registration
  glutReshapeFunc(resize);
  glutKeyboardFunc(key);
  glutDisplayFunc(display);
  glutMouseFunc(mouse); //añadidas por mí para controlar eventos del ratón
  glutMouseWheelFunc(mouseWheel);

  //OpenGL basic setting
  intitGL();


  // Freeglut's main loop can be stopped executing (**)
  //while ( continue_in_main_loop )
  //  glutMainLoopEvent();

  // Classic glut's main loop can be stopped after X-closing the window,
  // using the following freeglut's setting (*)
  glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION ) ;
    
  // Classic glut's main loop can be stopped in freeglut using (*)
  glutMainLoop(); 
  
  // We would never reach this point using classic glut
  system("PAUSE"); 
   
  return 0;
}

