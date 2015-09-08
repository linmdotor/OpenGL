#ifndef __PUNTO_H
#define __PUNTO_H

#include <GL/freeglut.h>

#include <math.h> // pow()

/**
 Clase Punto, que almacena el valor de coordenada X e Y (GLdouble).

 Operaciones: 
	- distanciaPto2Pto:  Punto, Punto -> GLdouble

@author Jesús Martínez Dotor
*/
class Punto {

private:
	// Coordenadas
	GLdouble _x, _y;

public:
	/** Constructores */
	Punto() : _x(0.0), _y(0.0) {}
	Punto(GLdouble x, GLdouble y) : _x(x), _y(y) {}
	Punto(const Punto &p) : _x(p._x), _y(p._y) {}

	/** Destructor */
	~Punto() {
	}

	/** Operadores de Comparación */
	bool operator==(Punto other) {
		 return ( _x == other._x && _y == other._y );
	}

	bool operator!=(Punto other) {
		return !operator==(other);
	}

	/**	setters y getters */
	void set_x(GLdouble x) {
		_x = x;
	}

	void set_y(GLdouble y) {
		_y = y;
	}
	
	GLdouble get_x() {
		return _x;
	}

	GLdouble get_y() {
		return _y;
	}
	
	/**
	 Calcula al distancia entre el punto actual y uno pasado como parámetro
	*/
	GLdouble distnciaPto2Pto(Punto p) {
		return sqrt( pow( (p._x - _x),2 ) + pow( (p._y - _y),2 ) ); // raiz((x2-x1)^2 + (y2-y1)^2)
	}

}; //Punto

#endif // __PUNTO_H