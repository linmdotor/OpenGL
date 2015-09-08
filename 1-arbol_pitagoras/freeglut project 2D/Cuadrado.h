#ifndef __CUADRADO_H
#define __CUADRADO_H

#include <GL/freeglut.h>

#include "Punto.h"
#include "RGBColor.h"
#include "Lapiz.h"

/**
 Clase Cuadrado, que almacena sus 4 vértices (GLdouble) y un color.
 El orden de los mismos es el siguiente:
	- p[0] -> inferior derecho
	- el resto se recorren en sentido antihorario, partiendo de p[1]

 Operaciones: 
	- draw:  Cuadrado -> 
	- seleccionar: Cuadrado, Punto, GLdouble -> Bool

@author Lin M. Dotor
*/
class Cuadrado {

private:
	// Vértices
	Punto _p[4];

	//Color
	RGBColor _colorrelleno;
	RGBColor _colorlinea;
	RGBColor _colorseleccionado;
	bool _relleno;
	bool _linea;
	bool _seleccionado;	

public:
	/** Constructores */
	Cuadrado() {
		_p[0] = Punto(100.0, 0.0);
		_p[1] = Punto(100.0, 100.0);
		_p[2] = Punto(0.0, 100.0);
		_p[3] = Punto(0.0, 0.0);
		_relleno = true;
		_linea = true;
		_colorrelleno = RGBColor(1.0, 1.0, 1.0);
		_colorseleccionado = RGBColor(1.0, 0.0, 1.0);
		_seleccionado = false;
	}

	Cuadrado(Punto centro, GLdouble lado) {
		_p[0] = Punto(centro.get_x()+(lado/2), centro.get_y()-(lado/2));
		_p[1] = Punto(centro.get_x()+(lado/2), centro.get_y()+(lado/2));
		_p[2] = Punto(centro.get_x()-(lado/2), centro.get_y()+(lado/2));
		_p[3] = Punto(centro.get_x()-(lado/2), centro.get_y()-(lado/2));
		_relleno = true;
		_linea = true;
		_colorrelleno = RGBColor(1.0, 1.0, 1.0);
		_colorseleccionado = RGBColor(1.0, 0.0, 1.0);
		_seleccionado = false;
	}

	Cuadrado(Punto p1, Punto p2, Punto p3, Punto p4) {
		_p[0] = p1;
		_p[1] = p2;
		_p[2] = p3;
		_p[3] = p4;
		_relleno = true;
		_linea = true;
		_colorrelleno = RGBColor(1.0, 1.0, 1.0);
		_colorseleccionado = RGBColor(1.0, 0.0, 1.0);
		_seleccionado = false;
	}
	
	Cuadrado(const Cuadrado &c) {
		_p[0] = c._p[0];
		_p[1] = c._p[1];
		_p[2] = c._p[2];
		_p[3] = c._p[3];
		_relleno = c._relleno;
		_linea = c._linea;
		_colorrelleno = c._colorrelleno;
		_colorlinea = c._colorlinea;
		_colorseleccionado = c._colorseleccionado;
		_seleccionado = c._seleccionado;
	}

	/** Destructor */
	~Cuadrado() {
	}

	/**	setters y getters */
	void set_colorrelleno(RGBColor c) {
		_colorrelleno = c;
	}

	RGBColor get_colorrelleno() {
		return _colorrelleno;
	}

	void set_colorlinea(RGBColor c) {
		_colorlinea = c;
	}

	void set_relleno(bool r) {
		_relleno = r;
	}

	void set_linea(bool l) {
		_linea = l;
	}

	Punto get_p(int i) {
		return _p[i];
	}

	void set_seleccionado(bool l) {
		_seleccionado = l;
	}

	/**
	 Dibuja el cuadrado
	*/
	void draw() {
		
		if(_seleccionado)
			_colorseleccionado.changeColor();
		else
			_colorrelleno.changeColor();

		if(_relleno)
		{	
			glBegin ( GL_QUADS ) ;
				glVertex2d( _p[0].get_x(), _p[0].get_y() );
				glVertex2d( _p[1].get_x(), _p[1].get_y() );
				glVertex2d( _p[2].get_x(), _p[2].get_y() );
				glVertex2d( _p[3].get_x(), _p[3].get_y() );
			glEnd () ;
		}

		_colorlinea.changeColor();
			
		if(_linea)
		{	
			glBegin ( GL_LINE_LOOP ) ;
				glVertex2d( _p[0].get_x(), _p[0].get_y() );
				glVertex2d( _p[1].get_x(), _p[1].get_y() );
				glVertex2d( _p[2].get_x(), _p[2].get_y() );
				glVertex2d( _p[3].get_x(), _p[3].get_y() );
			glEnd () ;
		}
	}

	Cuadrado crearCuadrado(Lapiz lap, GLdouble lado) {
		Punto p[4];

		for (int i=0; i<4; i++)
		{
			p[i] = lap.get_pos();
			lap.forward(lado, false);		
			lap.turn(90);
		}
		
		return Cuadrado(p[0], p[1], p[2], p[3]);
	}

	/**
	 Crea un nuevo cuadrado a partir del cuadrado actual, con un ángulo determinado.
	 "Devuelve" los 2 nuevos cuadrados, modificando las referencias originales.
	*/
	void anidar(Cuadrado &cuadnuevo1, Cuadrado &cuadnuevo2, GLdouble ang) {
		GLdouble lado = Punto(_p[0].get_x(), _p[0].get_y()).distnciaPto2Pto(Punto(_p[1].get_x(), _p[1].get_y())); //calculamos el lado original
		GLdouble lado_aux1 = cos(ang/180*PI)*lado;
		GLdouble lado_aux2 = sin(ang/180*PI)*lado;
		Lapiz lap = Lapiz(Punto(_p[0].get_x(), _p[0].get_y()), 0.0); //se crea un lapiz en el 0.0

		lap.lookAt(Punto(_p[1].get_x(), _p[1].get_y())); //lo colocamos mirando al p1
		lap.forward(lado, false); //avanza hasta p1

		lap.turn(-ang);

		cuadnuevo1 = crearCuadrado(lap, lado_aux1);

		lap.turn(90);
		lap.forward(lado_aux1, false);

		cuadnuevo2 = crearCuadrado(lap, lado_aux2);
	}

	/**
	 Determina si algún vertice del cuadrado está "cerca" del punto dado
	*/
	bool seleccionar(Punto p, GLdouble dist) {
		bool encontrado = false;
		for(int i=0; i<4 ; i++)
		{
			if(_p[i].distnciaPto2Pto(p) <= dist)
				encontrado = true;
		}
		return encontrado;
	}

}; //Cuadrado

#endif // __CUADRADO_H