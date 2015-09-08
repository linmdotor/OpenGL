/**
@author Lin M. Dotor
*/

#ifndef __ESCENA_H
#define __ESCENA_H

#include <GL/freeglut.h>

#include "Pelota.h"
#include "Obstaculo.h"
#include "Circulo.h"
#include "CirculoConMascara.h"
#include "Triangulo.h"
#include "TrianguloConMascara.h"

#define num_obs 10

/**
 Clase Escena, que contiene los obstaculos y la pelota
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
	- step: Escena ->

@author Lin M. Dotor
*/
class Escena {

private:
	// Elementos de la escena
	Pelota _pelota;
	Pelota _pelota_extra[20];
	int num_pelotas;
	Obstaculo* _obstaculos[num_obs];

	//Puntero a la función display(), para el zoomProgresivo
	void (*display)(void);

private:
	//Dimensiones AVE
	GLdouble _xLeft, _xRight, _yBot, _yTop;

	//Dimensiones PV2D
	int *_xInit, *_yInit;
	int *_CLIENT_WIDTH, *_CLIENT_HEIGHT;
	
public:
	bool _pintar_normales;
	bool _pintar_mascaras;
	bool _usar_mascaras;

	/** Constructores */
	Escena(GLdouble xLeft, GLdouble xRight, GLdouble yBot, GLdouble yTop, int *xInit, int *yInit, int *width, int *height, void (*disp)(void)) :
		_xLeft(xLeft), _xRight(xRight), _yBot(yBot), _yTop(yTop),  
		_xInit(xInit), _yInit(yInit), _CLIENT_WIDTH(width), _CLIENT_HEIGHT(height), 
		display (disp), _pintar_normales(false), _pintar_mascaras(false), _usar_mascaras(true), 
		_pelota(Pelota(PV2D(100.0, 200.0), 10.0, PV2D(3.0, -3.0))){ //generar_aleatorio(-5.0, 5.0), generar_aleatorio(-5.0, 5.0)

			//limites de la escena (triangulos)
		PV2D v[3];
		//Triangulo Abajo
		v[0] = PV2D(xRight+50, yBot);		
		v[1] = PV2D(xLeft-50, yBot);
		v[2] = PV2D((xRight+xLeft)/2, yBot-50);
		_obstaculos[0] = new TrianguloConMascara(v, _pelota.get_radio());
		_obstaculos[0]->setColor(RGBColor(0.0, 0.0, 1.0));
		//Triangulo Arriba
		v[0] = PV2D(xLeft-50, yTop);
		v[1] = PV2D(xRight+50, yTop);
		v[2] = PV2D((xRight+xLeft)/2, yTop+50);
		_obstaculos[1] = new TrianguloConMascara(v, _pelota.get_radio());
		_obstaculos[1]->setColor(RGBColor(0.0, 0.0, 1.0));
		//Triangulo Izquierda
		v[0] = PV2D(xLeft, yBot-50);
		v[1] = PV2D(xLeft, yTop+50);
		v[2] = PV2D(xLeft-50, (yBot+yTop)/2);
		_obstaculos[2] = new TrianguloConMascara(v, _pelota.get_radio());
		_obstaculos[2]->setColor(RGBColor(0.0, 0.0, 1.0));
		//Triangulo Derecha
		v[0] = PV2D(xRight, yTop+50);
		v[1] = PV2D(xRight, yBot-50);
		v[2] = PV2D(xRight+50, (yBot+yTop)/2);
		_obstaculos[3] = new TrianguloConMascara(v, _pelota.get_radio());
		_obstaculos[3]->setColor(RGBColor(0.0, 0.0, 1.0));


			//obstaculos de la escena

		v[0] = PV2D(250.0, 50.0);
		v[1] = PV2D(150.0, 100.0);
		v[2] = PV2D(50.0, 0.0);
		_obstaculos[4] = new TrianguloConMascara(v, _pelota.get_radio());
		_obstaculos[4]->setColor(RGBColor(1.0, 1.0, 0.0));

		v[0] = PV2D(425.0, 121.0);
		v[1] = PV2D(355.0, 165.0);
		v[2] = PV2D(360.0, 105.0);
		_obstaculos[5] = new TrianguloConMascara(v, _pelota.get_radio());
		_obstaculos[5]->setColor(RGBColor(1.0, 1.0, 0.0));

		v[0] = PV2D(300.0, 145.0);
		v[1] = PV2D(285.0, 160.0);
		v[2] = PV2D(270.0, 145.0);
		_obstaculos[6] = new TrianguloConMascara(v, _pelota.get_radio());
		_obstaculos[6]->setColor(RGBColor(1.0, 1.0, 0.0));

		_obstaculos[7] = new CirculoConMascara(PV2D(200.0, 200.0), 50.0, _pelota.get_radio());
		_obstaculos[8] = new CirculoConMascara(PV2D(400.0, 50.0), 25.0, _pelota.get_radio());
		_obstaculos[9] = new CirculoConMascara(PV2D(400.0, 200.0), 30.0, _pelota.get_radio());
		_obstaculos[7]->setColor(RGBColor(1.0, 1.0, 0.0));
		_obstaculos[8]->setColor(RGBColor(1.0, 1.0, 0.0));
		_obstaculos[9]->setColor(RGBColor(1.0, 1.0, 0.0));
	}


	
	/** Destructor */
	~Escena() {
	}

	/**	setters y getters */
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
		drawMargin();

		for (int i=0; i<num_obs; i++)
			_obstaculos[i]->draw(_pintar_normales, _pintar_mascaras);

		_pelota.draw(_pintar_normales);
		for(int i=0; i<num_pelotas; i++)
			_pelota_extra[i].draw(_pintar_normales);
	}

	
	//----------EMBALDOSAR----------
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

	/**  ----------STEP----------
	Método que avanza un paso en la animación, por lo que mueve los objetos.
	1º Comprueba la colisión con todos los obstaculos.
	2º Avanza
	3º Rebota
	A este step se le podrá llamar por el callback KEY o callback TIMER
	*/
	void step(Pelota &pelota)
	{
		PV2D normal_min;
		GLdouble t_hit_min = 2;

		for (int i=0; i<num_obs ; i++)
		{
			PV2D normal;
			GLdouble t_hit;

			if(_obstaculos[i]->interseccion(pelota.get_centro(), pelota.get_v_mov(), pelota.get_v_mov_1(), normal, t_hit, _usar_mascaras))
			{
				//cout << "posible colision con obstaculo " << i << " en el t: " << t_hit << "   es (<= 1)? " << (t_hit <= 1) << endl;
				if(t_hit > 0 && t_hit <= 1+_epsilon) //ALEATORIAMENTE, DECIDE QUE 1 NO ESTÁ ENTRE EL RANGO, POR ESO AÑADIMOS EPSILON
				{
					//cout << "COLISION!!!! con obstaculo " << i << " en el t: " << t_hit << endl;
					if(t_hit_min > t_hit)
					{
						t_hit_min = t_hit;
						normal_min = normal;
					}
				}
			}
		}

		if(t_hit_min == 2) //no ha habido colisión
		{
			pelota.avanza(1.0);
		}
		else //hubo colisión
		{
			pelota.avanza(t_hit_min);
			pelota.rebota(normal_min);
		}
	}

	void step()
	{
		step(_pelota);

		for(int i=0; i<num_pelotas; i++)
			step(_pelota_extra[i]);
	}

	void nuevaPelota()
	{
		if(num_pelotas<20)
		{
			_pelota_extra[num_pelotas] = Pelota(PV2D(generar_aleatorio(25.0, 125.0), generar_aleatorio(125.0, 225.0)), 10.0, 
				PV2D(generar_aleatorio(-5.0, 5.0), generar_aleatorio(-5.0, 5.0)));
			//_pelota_extra[num_pelotas].setColor(RGBColor(generar_aleatorio(0.0, 1.0), generar_aleatorio(0.0, 1.0), generar_aleatorio(0.0, 1.0)));
			num_pelotas++;
		}
	}

	void quitaPelota()
	{
		if(num_pelotas>0)
			num_pelotas--;
	}

	/**
	 Genera un aleatorio GLdouble entre 2 límites
	*/
	GLdouble generar_aleatorio(GLdouble min, GLdouble max) {
		return (((GLdouble) rand()*(max-min))/ (GLdouble)RAND_MAX + min);
	}

}; //Escena

#endif // __ESCENA_H