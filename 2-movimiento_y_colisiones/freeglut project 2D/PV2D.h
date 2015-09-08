/**
@author Lin M. Dotor
*/

#ifndef __PV2D_H
#define __PV2D_H

#include <GL/freeglut.h>

#include <math.h> // pow()

/**
 Clase PuntoVector, que almacena el valor de coordenada X e Y (GLdouble).
 con esta se puede definir tanto un punto, como un vector centrado en el origen.

 Operaciones: 
	- distanciaPto2Pto:  PV2D, PV2D -> GLdouble

@author Lin M. Dotor
*/
class PV2D {

private:
	// Coordenadas
	GLdouble _x, _y;

public:
	/** Constructores */
	PV2D() : _x(0.0), _y(0.0) {}
	PV2D(GLdouble x, GLdouble y) : _x(x), _y(y) {}
	PV2D(const PV2D &p) : _x(p._x), _y(p._y) {}

	/** Destructor */
	~PV2D() {
	}

	/** Operadores de Comparación */
	bool operator==(PV2D other) {
		 return ( _x == other._x && _y == other._y );
	}

	bool operator!=(PV2D other) {
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


	/**************************************
	OPERACIONES "PUNTO - PUNTO"
	***************************************/

	/**
	 Calcula al distancia entre el punto actual (P) y uno pasado como parámetro (Q)
	*/
	GLdouble distanciaPto2Pto(PV2D q) {
		return sqrt( pow( (q._x - _x),2 ) + pow( (q._y - _y),2 ) ); // raiz((x2-x1)^2 + (y2-y1)^2)
	}

	/**
	Calcula el punto medio entre el punto actual (P) y uno pasado como parámetro (Q)
	*/
	PV2D puntoMedio(PV2D q)
	{
		return PV2D((_x + q._x)/2, (_y + q._y)/2);
	}



	/**************************************
	OPERACIONES "PUNTO - VECTOR"
	***************************************/

	/**
	 Resta a un punto pasado como parámetro (Q) el propio vector (P), (Q-P) 
	 por lo que crea un nuevo vector desde el punto propio al parámetro V=[PQ] (en ese sentido!).
	*/
	PV2D crearVector_RestaPto2Pto(PV2D q) {
		return PV2D((q._x - _x), (q._y - _y));
	}

	/**
	 Suma un vector pasado como parámetro (V) al punto propio (P),
	 lo que origina un nuevo punto, desplazado mediante el vector (P+V)
	*/
	PV2D sumarVect2Pto(PV2D v) {
		return PV2D((_x + v._x), (_y + v._y));
	}

	

	/**************************************
	OPERACIONES "VECTOR - VECTOR"
	***************************************/

	/**
	Invierte un vector, por lo que mantiene su direccióny módulo, pero cambia su sentido
	*/
	void invertir()
	{
		_x = -_x;
		_y = -_y;
	}

	/**
	 Suma al vector propio (V), otro vector pasado como parámetro (W) 
	 por lo que crea un nuevo vector, combinación de ambos, que pasa por la bisectriz de ambos.
	*/
	PV2D sumarVect2Vect(PV2D w) {
		return PV2D((w._x + _x), (w._y + _y));
	}

	/**
	 Resta al vector propio (P), otro vector pasado como parámetro (W) 
	 por lo que crea un nuevo vector, que va desde W a V (en ese sentido!).
	*/
	PV2D restarVect2Vect(PV2D w) {
		return PV2D((_x - w._x), (_y - w._y));
	}

	/**
	 Multiplica al vector propio (P), multiplicándolo un cierto escalar (K)
	 por lo que crea un nuevo vector, con misma dirección, pero distinto módulo y/o sentido.
	*/
	PV2D multiplicarVect2Escalar(GLdouble k) {
		return PV2D((k * _x), (k * _y));
	}

	/**
	 Calcula el módulo del vector propio (V), que es la longitud.
	*/
	GLdouble moduloVect() {
		return sqrt( pow(_x, 2) + pow(_y, 2));
	}

	/**
	 Normaliza un vector propio (V), 
	 por lo que resulta un vector con misma direccion y sentido, pero modulo 1.
	 Si el módulo del vector es 0, no se puede normalizar, por lo que devuelve el vector (0,0)
	*/
	PV2D normalizarVect() {
		if (moduloVect() != 0)
			return PV2D ( _x/moduloVect(), _y/moduloVect());
		else
			return PV2D();
	}

	/**
	 Realiza el producto escalar del vector propio (V) con otro pasado como parámetro (W)
	 El resultado de un producto escalar es siempre un valor real.
	*/
	GLdouble productoEscalar(PV2D w) {
		return ((_x * w._x) + (_y * w._y));
	}

	/**
	Calcula la normal al vector, resultante de girar el vector 90º en sentido contrahorario (izquierda)
	*/
	PV2D vectorNormal() {
		return PV2D ( -_y, _x);
	}

}; //PV2D

#endif // __PV2D_H