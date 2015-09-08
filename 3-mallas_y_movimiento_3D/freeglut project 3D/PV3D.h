/**
@author Jesús Martínez Dotor
*/

#ifndef __PV3D_H
#define __PV3D_H

#include <GL/freeglut.h>

#include <math.h> // pow()

/**
 Clase PuntoVector, que almacena el valor de coordenada X, Y y Z (GLdouble).
 con esta se puede definir tanto un punto, como un vector centrado en el origen.
*/
class PV3D {

private:
	// Coordenadas
	GLdouble _x, _y, _z;
	bool _punto; //indica si es punto o vector

public:
	/** Constructores */
	PV3D() : _x(0.0), _y(0.0), _z(0.0), _punto(true) {}
	PV3D(GLdouble x, GLdouble y, GLdouble z, bool punto) : _x(x), _y(y), _z(z), _punto(punto) {}
	PV3D(const PV3D &p) : _x(p._x), _y(p._y), _z(p._z), _punto(p._punto) {}
	PV3D(PV3D *p) : _x(p->_x), _y(p->_y), _z(p->_z), _punto(p->_punto) {}

	/** Destructor */
	~PV3D() {
	}

	/** Operadores de Comparación */
	bool operator==(PV3D other) {
		 return ( _x == other._x && _y == other._y && _z == other._z);
	}

	bool operator!=(PV3D other) {
		return !operator==(other);
	}

	/**	setters y getters */
	void set_punto(bool p) {
		_punto = p;
	}

	void set_x(GLdouble x) {
		_x = x;
	}

	void set_y(GLdouble y) {
		_y = y;
	}

	void set_z(GLdouble z) {
		_z = z;
	}
	
	bool is_punto() {
		return _punto;
	}

	GLdouble get_x() {
		return _x;
	}

	GLdouble get_y() {
		return _y;
	}

	GLdouble get_z() {
		return _z;
	}


	/*
	 Clona un PV3D a partir de uno pasado como parámetro
	*/
	PV3D* clona()
	{
		return new PV3D(_x, _y, _z, _punto);
	}


	/**************************************
	OPERACIONES "PUNTO - PUNTO"
	***************************************/

	/**
	 Calcula al distancia entre el punto actual (P) y uno pasado como parámetro (Q)
	*/
	GLdouble distanciaPto2Pto(PV3D q) {
		return sqrt( pow( (q._x - _x),2 ) + pow( (q._y - _y),2 ) + pow( (q._z - _z),2 ) ); // raiz((x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2)
	}

	/**
	Calcula el punto medio entre el punto actual (P) y uno pasado como parámetro (Q)
	*/
	PV3D puntoMedio(PV3D q)
	{
		return PV3D((_x + q._x)/2, (_y + q._y)/2, (_z + q._z)/2, true);
	}



	/**************************************
	OPERACIONES "PUNTO - VECTOR"
	***************************************/

	/**
	 Resta a un punto pasado como parámetro (Q) el propio vector (P), (Q-P) 
	 por lo que crea un nuevo vector desde el punto propio al parámetro V=[PQ] (en ese sentido!).
	*/
	PV3D crearVector_RestaPto2Pto(PV3D q) {
		return PV3D((q._x - _x), (q._y - _y), (q._z - _z), false);
	}

	/**
	 Suma un vector pasado como parámetro (V) al punto propio (P),
	 lo que origina un nuevo punto, desplazado mediante el vector (P+V)
	*/
	PV3D sumarVect2Pto(PV3D v) {
		return PV3D((_x + v._x), (_y + v._y), (_z + v._z), true);
	}

	

	/**************************************
	OPERACIONES "VECTOR - VECTOR"
	***************************************/

	/**
	Invierte un vector, por lo que mantiene su dirección y módulo, pero cambia su sentido
	*/
	void invertir()
	{
		_x = -_x;
		_y = -_y;
		_z = -_z;
	}

	/**
	 Suma al vector propio (V), otro vector pasado como parámetro (W) 
	 por lo que crea un nuevo vector, combinación de ambos, que pasa por la bisectriz de ambos.
	*/
	PV3D sumarVect2Vect(PV3D w) {
		return PV3D((w._x + _x), (w._y + _y), (w._z + _z), false);
	}

	/**
	 Resta al vector propio (P), otro vector pasado como parámetro (W) 
	 por lo que crea un nuevo vector, que va desde W a V (en ese sentido!).
	*/
	PV3D restarVect2Vect(PV3D w) {
		return PV3D((_x - w._x), (_y - w._y), (_z - w._z), false);
	}

	/**
	 Multiplica al vector propio (P), multiplicándolo un cierto escalar (K)
	 por lo que crea un nuevo vector, con misma dirección, pero distinto módulo y/o sentido.
	*/
	PV3D multiplicarVect2Escalar(GLdouble k) {
		return PV3D((k * _x), (k * _y), (k * _z), false);
	}

	/**
	 Calcula el módulo del vector propio (V), que es la longitud.
	*/
	GLdouble moduloVect() {
		return sqrt( pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
	}

	/**
	 Normaliza un vector propio (V), 
	 por lo que resulta un vector con misma direccion y sentido, pero modulo 1.
	 Si el módulo del vector es 0, no se puede normalizar, por lo que devuelve el vector (0,0)
	*/
	PV3D normalizarVect() {
		if (moduloVect() != 0)
			return PV3D ( _x/moduloVect(), _y/moduloVect(), _z/moduloVect(), false);
		else
			return PV3D();
	}

	/**
	 Realiza el producto escalar del vector propio (V) con otro pasado como parámetro (W)
	 El resultado de un producto escalar es siempre un valor real.
	*/
	GLdouble productoEscalar(PV3D w) {
		return ((_x * w._x) + (_y * w._y) + (_z * w._z));
	}

	/**
	 Realiza el producto vectorial del vector propio (V) con otro pasado como parámetro (W)
	 El resultado de un producto vectorial es siempre otro vector,
	 cuya dirección es perpendicular a los dos vectores y su sentido sería igual al avance de un sacacorchos al girar de u a v.
	 O la regla de la mano derecha.
	 ver: http://www.vitutor.com/analitica/vectores/producto_vectorial.html 
	*/
	PV3D productoVectorial(PV3D w) {
		return PV3D( ((_y * w._z) - (_z * w._y)), 
					((_z * w._x) - (_x * w._z)), 
					((_x * w._y) - (_y * w._x)), 
					false); 	
	}

	/**
	Calcula la normal al vector, resultante de girar el vector 90º en sentido contrahorario (izquierda)
	*/
	//ESTE MÉTODO (como tal) NO TIENE SENTIDO EN 3D
	/*PV2D vectorNormal() {
		return PV2D ( -_y, _x);
	}*/

}; //PV3D

#endif // __PV3D_H