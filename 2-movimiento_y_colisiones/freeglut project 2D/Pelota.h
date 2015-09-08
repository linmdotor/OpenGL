/**
@author Jesús Martínez Dotor
*/

#ifndef __PELOTA_H
#define __PELOTA_H

#include <GL/freeglut.h>

#include "PV2D.h"
#include "RGBColor.h"
#include "Poligono.h"
#include "PintarVect.h"

/**
 Clase Pelota, que almacena sus posición y el polígono con que se pinta.
 Contiene:
	- centro -> posición del centro de la pelota
	- radio -> longitud del radio
	- v_mov -> vector que expresa su movimiento (el módulo es su velocidad)

	- Poligono -> poligono regular de n lados que se emplea para representarla.

 Operaciones: 
	- draw:  Cuadrado -> 
	- seleccionar: Cuadrado, PV2D, GLdouble -> Bool

@author Jesús Martínez Dotor
*/
class Pelota {

	Poligono _poligono;

	PV2D _centro;
	GLdouble _radio;
	PV2D _v_mov; //vector del movimiento
	PV2D _v_mov_1; //unitario del movimiento

	public:
	/** Constructores */
	Pelota() :
		_centro(PV2D(100.0, 100.0)), _radio(10.0), _v_mov(PV2D(10.0, 10.0)),
		_poligono(Poligono(20, PV2D(100.0, 100.0), 10.0)), 
		_v_mov_1(PV2D(10.0, 10.0).normalizarVect()) {}

	Pelota(PV2D centro, GLdouble radio, PV2D v_mov) :
		_centro(centro), _radio(radio), _v_mov(v_mov),
		_poligono(Poligono(20, centro, radio)), 
		_v_mov_1(v_mov.normalizarVect()) {}
	
	/** Destructor */
	~Pelota() {
	}

	/**	setters y getters */

	PV2D get_centro() {
		return _centro;
	}

	PV2D get_v_mov() {
		return _v_mov;
	}

	PV2D get_v_mov_1() {
		return _v_mov_1;
	}

	GLdouble get_radio() {
		return _radio;
	}

	/**
	 Dibuja la pelota
	*/
	void draw(bool pintar_vector) {
		_poligono.draw();
		if(pintar_vector)
			PintarVect::draw(_centro, _v_mov, 1.0);
	}

	/**
	Avanza una cierta cantidad (t) la pelota (debe estar entre 0 (sin incluir) y 1 (incluido))
	*/
	void avanza(GLdouble t)
	{
		_centro = _centro.sumarVect2Pto(_v_mov.multiplicarVect2Escalar(t));
		_poligono.mover(_v_mov.multiplicarVect2Escalar(t));
	}

	/**
	Modifica los valores del vector de la pelota, para rebotar, respecto a una cierta normal
	*/
	void rebota(PV2D n)
	{
		GLdouble proyeccion = _v_mov.productoEscalar(n);
		_v_mov = _v_mov.restarVect2Vect(n.multiplicarVect2Escalar(2.0).multiplicarVect2Escalar(proyeccion));
		_v_mov_1 = _v_mov.normalizarVect(); //unitario del movimiento
	}

	void setColor(RGBColor c)
	{
		_poligono.set_colorrelleno(c);
	}

}; //Pelota

#endif // __PELOTA_H