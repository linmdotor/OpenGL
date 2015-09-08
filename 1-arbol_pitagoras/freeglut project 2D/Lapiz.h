#ifndef __LAPIZ_H
#define __LAPIZ_H

#include <GL/freeglut.h>

#include "Punto.h"

const GLdouble PI = 3.141592653589793;

/**
 Clase Lapiz, que almacena un puntero a un punto y una dirección.

 Operaciones: 
	- moveTo: Lapiz, Punto*, Bool -> 
	- turnTo: Lapiz, GLdouble -> 
	- turn: Lapiz, GLdouble -> 
	- forward: Lapiz, GLdouble, Bool -> 

@author Jesús Martínez Dotor
*/
class Lapiz {

private:
	// Posición
	Punto _pos;

	//Dirección (en GRADOS) [360° = 2PiRad]
	GLdouble _dir;

public:
	/** Constructores */
	Lapiz() : _pos(Punto()), _dir(0.0) {}
	Lapiz(Punto p, GLdouble dir) : _pos(Punto(p.get_x(), p.get_y())), _dir(dir) {}
	Lapiz(const Lapiz &l) : _pos(Punto(l._pos)), _dir(l._dir) {}

	/** Destructor */
	~Lapiz() {
	}

	/**	setters y getters */
	void moveTo(Punto p, bool draw) {
		if (draw)
		{
			glBegin ( GL_LINES ) ;
				glVertex2d( _pos.get_x(), _pos.get_y() );
				glVertex2d( p.get_x(), p.get_y() );
			glEnd () ;
		}

		_pos = Punto(p);
	}

	void turnTo(GLdouble a) {
		_dir = a;
	}
	
	Punto get_pos() {
		return _pos;
	}

	GLdouble get_dir() {
		return _dir;
	}
	
	/**
	 Gira la dirección hacia la izquierda (sentido contrahorario)
	*/
	void turn(GLdouble a) {
		_dir += a;
	}

	/**
	 Avanza la posición del lápiz siguiendo su dirección.
	 El parámetro draw indica si se dibuja el segmento.
	*/
	void forward(GLdouble dist, bool draw) {
		Punto p = Punto(_pos.get_x() + dist*cos(_dir/180*PI),
						_pos.get_y() + dist*sin(_dir/180*PI));
		moveTo(p, draw);
	}

	/**
	 Cambia la dirección del lápiz, que ahora apunta al punto referenciado.
	*/
	void lookAt(Punto p) {
		GLdouble cateto_op = p.get_y() - _pos.get_y();
		GLdouble cateto_cont = p.get_x() - _pos.get_x();
		_dir = atan2(cateto_op, cateto_cont)*180/PI;
	}

}; //Lapiz

#endif // __LAPIZ_H