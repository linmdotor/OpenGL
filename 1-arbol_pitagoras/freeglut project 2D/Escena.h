#ifndef __ESCENA_H
#define __ESCENA_H

#include <GL/freeglut.h>

#include "Arbol.h"
#include "Cuadrado.h"

/**
 Clase Escena, que contiene un árbol de pitágoras
 y las propiedades de la misma.

 Operaciones: 
	- trasladar: Escena, GLdouble, GLdouble -> 
	- trasladarHorizontal: Escena, GLdouble -> 
	- trasladarVertical: Escena, GLdouble -> 
	- zoom: Escena, GLdouble -> 
	- zoomPercent: Escena, GLdouble -> 
	- draw: Escena -> 
	- anidar: Escena ->
	- PVtoAVE_X: Escena, GLdouble -> GLdouble 
	- PVtoAVE_Y: Escena, GLdouble -> GLdouble 
	- AVEtoPV_X: Escena, GLdouble -> GLdouble 
	- AVEtoPV_Y: Escena, GLdouble -> GLdouble 
	- dibujarChar: Escena, char, void *, GLdouble, GLdouble ->  
	- dibujarString: Escena, char *, void *, GLdouble, GLdouble -> 
	- dibujarInt: Escena, int, void *, GLdouble, GLdouble -> 

@author Jesús Martínez Dotor
*/
class Escena {

private:
	//Propiedades del árbol
	GLdouble _anguloderecho;
	GLdouble _lado;

	// Arbol
	Arbol _arbol;

	//Puntero a la función display(), para el zoomProgresivo
	void (*display)(void);

private:
	//Copia del Cuadrado seleccionado del árbol
	Cuadrado* _seleccionado;

	//Dimensiones AVE
	GLdouble _xLeft, _xRight, _yBot, _yTop;

	//Dimensiones PV
	int *_xInit, *_yInit;
	int *_CLIENT_WIDTH, *_CLIENT_HEIGHT;
	
public:
	/** Constructores */
	/*Escena() : _arbol(Arbol()), _anguloderecho(45.0), _lado(50.0), _seleccionado(NULL),
				_xLeft(0.0), _xRight(500.0), _yBot(0.0), _yTop(250.0), 
				_xInit(0), _yInit(0), _CLIENT_WIDTH(500), _CLIENT_HEIGHT(250) {}*/
	Escena(GLdouble xLeft, GLdouble xRight, GLdouble yBot, GLdouble yTop, int *xInit, int *yInit, int *width, int *height, void (*disp)(void)) :
		_arbol(Arbol()), _anguloderecho(45.0), _lado(50.0), _seleccionado(NULL),
		_xLeft(xLeft), _xRight(xRight), _yBot(yBot), _yTop(yTop),  
		_xInit(xInit), _yInit(yInit), _CLIENT_WIDTH(width), _CLIENT_HEIGHT(height), 
		display (disp) {}
	
	/** Destructor */
	~Escena() {
	}

	/**	setters y getters */
	void set_anguloderecho(GLdouble angulo) {
		_anguloderecho = angulo;
	}

	void set_lado(GLdouble lado) {
		_lado = lado;
	}

	void set_arbol(Arbol arbol) {
		_arbol = arbol;
	}

	GLdouble get_xLeft() {
		return _xLeft;
	}

	void set_xLeft(GLdouble xLeft) {
		_xLeft = xLeft;
	}

	GLdouble get_xRight() {
		return _xRight;
	}

	void set_xRight(GLdouble xRight) {
		_xRight = xRight;
	}

	GLdouble get_yBot() {
		return _yBot;
	}

	void set_yBot(GLdouble yBot) {
		_yBot = yBot;
	}

	GLdouble get_yTop() {
		return _yTop;
	}

	void set_yTop(GLdouble yTop) {
		_yTop = yTop;
	}


	//----------ACTUALIZAR MATRIZ AVE----------
	void actualizarAVE(GLdouble xLeft, GLdouble xRight, GLdouble yBot, GLdouble yTop) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(xLeft, xRight, yBot, yTop);
	}

	//----------TRASLACIÓN----------
	void trasladar(GLdouble x, GLdouble y)
	{
		_xLeft += x; _xRight += x;
		_yTop += y;  _yBot += y;

		actualizarAVE(_xLeft, _xRight, _yBot, _yTop);		
	}

	void trasladarHorizontal(GLdouble x)
	{
		trasladar(x, 0);
	}

	void trasladarVertical(GLdouble y)
	{
		trasladar(0, y);
	}

	//----------ZOOM----------
	void zoom(GLdouble factorEscala)
	{	
		GLdouble centroX = (_xRight + _xLeft)/2;
		GLdouble centroY = (_yTop + _yBot)/2;

		GLdouble nuevoAncho = (_xRight - _xLeft)/factorEscala;
		GLdouble nuevoAlto = (_yTop - _yBot)/factorEscala;

		//aplicamos el zoom
		_xLeft = centroX - nuevoAncho/2;
		_xRight = centroX + nuevoAncho/2;
		_yBot = centroY - nuevoAlto/2;
		_yTop = centroY + nuevoAlto/2;

		actualizarAVE(_xLeft, _xRight, _yBot, _yTop);
	}

	void zoomPercent(GLdouble percent)
	{
		zoom(percent/100);
	}

	void zoomProgresivo(GLdouble factorEscala, int duracion)
	{	
		int Iter_per_sec = 18; // Se toma 18, por que es el mínimo nº de fotogramas por segundo que la vista
								//puede ver como movimiento continuo
		int tiempoIncr = 1000/Iter_per_sec;
		int nIter = duracion*Iter_per_sec; 		

		GLdouble ancho = (_xRight - _xLeft);
		GLdouble alto = (_yTop - _yBot);
		
		GLdouble centroX = (_xRight + _xLeft)/2;
		GLdouble centroY = (_yTop + _yBot)/2;	

		GLdouble factorIncr= (factorEscala-1)/(GLdouble)nIter;
		for (int i=0; i<=nIter; i++)
		{
			GLdouble fAux= 1 + factorIncr*i;
			GLdouble anchoNew = ancho/fAux; 
			GLdouble altoNew= alto/fAux;
		
			//aplicamos el zoom
			_xLeft = centroX - anchoNew/2;
			_xRight = centroX + anchoNew/2;
			_yBot = centroY - altoNew/2;
			_yTop = centroY + altoNew/2;

			actualizarAVE(_xLeft, _xRight, _yBot, _yTop);
			display(); //glutPostRedisplay(); no funciona!
			Sleep(tiempoIncr);
		}
	}

	//----------DRAW----------
	void draw()
	{	
		drawAxes();
		_arbol.draw();
		drawMargin();
	}

	//----------ANIDAR----------
	void anidar()
	{
		_arbol.anidar();
	}

	//----------DESANIDAR----------
	void desanidar()
	{
		_arbol.desanidar();
	}

	//----------SELECCIONAR----------
	void seleccionar(int x, int y)
	{
		_arbol.seleccionar(_seleccionado, Punto(PVtoAVE_X(x), PVtoAVE_Y(y)), (_xRight - _xLeft)/50);
	}

	//----------SELECCIONAR----------
	void embaldosar(int nCol){
		GLdouble AVEratio = (_xRight - _xLeft) / (_yTop - _yBot);
		GLdouble ancho = (GLdouble)*_CLIENT_WIDTH / (GLdouble)nCol;
		//La altura de cada puerto se calcula proporcionalmente
		GLdouble alto = ancho/AVEratio;

		for(GLint c=0; c<=nCol; c++) 
		{
			GLdouble altoActual = 0;
			while((altoActual + alto) <= *_CLIENT_HEIGHT +8) // se deja un margen de +8 para el borde de la ventana
			{
				glViewport((GLint)(c*ancho), (GLint)altoActual, (GLint)ancho, (GLint)alto);
				draw(); //dibujar la escena
				altoActual += alto;
			}//while
		}//for
	}

	//----------TRANFORMAR AVEtoPV----------
	GLdouble PVtoAVE_X(GLdouble x)
	{
		GLdouble escalaancho = *_CLIENT_WIDTH/(_xRight-_xLeft);
		return ((x-*_xInit)/escalaancho)+_xLeft;
	}

	GLdouble PVtoAVE_Y(GLdouble y)
	{
		GLdouble escalaalto = *_CLIENT_HEIGHT/(_yTop-_yBot);
		return -((y-*_yInit)/escalaalto)+_yBot+(_yTop-_yBot);
	}

	int AVEtoPV_X(GLdouble x)
	{
		GLdouble escalaancho = *_CLIENT_WIDTH/(_xRight-_xLeft);
		return (GLint)(escalaancho*(x-_xLeft))+*_xInit;
	}

	int AVEtoPV_Y(GLdouble y)
	{
		GLdouble escalaalto = *_CLIENT_HEIGHT/(_yTop-_yBot);
		return (-(GLint)(escalaalto*(y-(_yTop-_yBot)-_yBot)))+*_yInit;
	}


	//----------ESCRIBIR TEXTO----------
	void dibujarChar(char c, void *fuente, GLdouble x, GLdouble y )
	{
		glRasterPos2d (x, y);
		glutBitmapCharacter(fuente, c); 
	}

	void dibujarString (char *s, void *fuente, GLdouble x, GLdouble y) //ver si declararlo como void inline drawString
	{ 
		glRasterPos2d (x, y);

		for (unsigned int i=0; i<strlen(s); i++) 
			glutBitmapCharacter(fuente, s[i]); 
	} 

	void dibujarInt (int n, void *fuente, GLdouble x, GLdouble y)
	{
		char buffer [10];
		itoa (n,buffer,10);
		dibujarString(buffer, fuente, x, y);
	}

	//----------DIBUJAR MARGEN----------
	void drawMargin() {
	  glColor3f(0.0,0.0,0.0);

	  glBegin ( GL_LINE_LOOP ) ; 
		glVertex2d( _xLeft, _yTop );
		glVertex2d( _xRight, _yTop );
		glVertex2d( _xRight, _yBot );
		glVertex2d( _xLeft, _yBot );
	  glEnd () ; 
	}

	//----------DIBUJAR EJES DE COORDENADAS----------
	void drawAxes()
	{
		GLdouble longitudEjeX = 2.0*500, 
				longitudEjeY = 2.0*250;
		GLdouble centroEjeX = 0.0, 
				centroEjeY = 0.0;

		glColor3f(0.5,0.5,0.5);

		//ejeX
	  glBegin ( GL_LINES ) ; 
		glVertex2d( centroEjeX-longitudEjeX, centroEjeY );
		glVertex2d( centroEjeX+longitudEjeX, centroEjeY );
	  glEnd () ; 

	  //ejeY
	  glBegin ( GL_LINES ) ; 
		glVertex2d( centroEjeX, centroEjeY-longitudEjeY );
		glVertex2d( centroEjeX, centroEjeY+longitudEjeY );
	  glEnd () ;
  
	  //lineas ejeX
	  for(int i = (int)(centroEjeX-longitudEjeX); i <= (int)(centroEjeX+longitudEjeX); i += 100)
	  {
		  glBegin ( GL_LINES ) ; 
			glVertex2d( i, centroEjeY+5 );
			glVertex2d( i, centroEjeY-5 );
		  glEnd () ;

		  dibujarInt(i, GLUT_BITMAP_HELVETICA_10, i, centroEjeY+5);
	  }

	  //lineas ejeY
	  for(int i = (int)(centroEjeY-longitudEjeY); i <= (int)(centroEjeY+longitudEjeY); i += 100)
	  {
		  glBegin ( GL_LINES ) ; 
			glVertex2d( centroEjeY+5, i );
			glVertex2d( centroEjeY-5, i );
		  glEnd () ;

		  dibujarInt(i, GLUT_BITMAP_HELVETICA_10, centroEjeX+5, i);
	  }
	}

}; //Escena

#endif // __ESCENA_H