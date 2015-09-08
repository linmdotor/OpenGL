/**
@author Lin M. Dotor
*/

#ifndef __POLIGONO_H
#define __POLIGONO_H

#include <GL/freeglut.h>

#include "PV2D.h"
#include "RGBColor.h"
#include "Lapiz.h"

#include <iostream>
using namespace std;

/**
 Clase Poligono, que almacena sus N vértices (GLdouble), radio y un color.
 El orden de los mismos es el siguiente:
	- p[0] -> el de la derecha
	- el resto se recorren en sentido antihorario, partiendo de p[1]

 Operaciones: 
	- draw:  Poligono ->
	- construir: Poligono ->

@author Lin M. Dotor
*/
class Poligono {

private:
	// Vértices (máx 20 (icoságono))
	PV2D _p[20];
	int _num_lados;

	//Color
	RGBColor _colorrelleno;
	RGBColor _colorlinea;
	bool _relleno;
	bool _linea;

public:
	/** Constructores */
	Poligono() {
		_num_lados = 6;

		construirPoligono(_num_lados, PV2D(50.0, 50.0), 50.0, _p);

		_relleno = true;
		_linea = true;
		_colorrelleno = RGBColor(0.0, 1.0, 0.0);
	}

	Poligono(int num_lados, PV2D centro, GLdouble radio) {
		
		construirPoligono(num_lados, centro, radio, _p);

		_num_lados = num_lados;
		_relleno = true;
		_linea = true;
		_colorrelleno = RGBColor(0.0, 1.0, 0.0);
	}

	Poligono(int num_lados, PV2D p[]) {
		for(int i=0; i<num_lados; i++)
			_p[i] = p[i];

		_num_lados = num_lados;
		_relleno = true;
		_linea = true;
		_colorrelleno = RGBColor(0.0, 1.0, 0.0);
	}
	
	Poligono(const Poligono &c) {
		for(int i=0; i<c._num_lados; i++)
			_p[i] = c._p[i];

		_num_lados = c._num_lados;
		_relleno = c._relleno;
		_linea = c._linea;
		_colorrelleno = c._colorrelleno;
		_colorlinea = c._colorlinea;
	}

	/** Destructor */
	~Poligono() {
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

	PV2D get_p(int i) {
		return _p[i];
	}

	int get_num_lados() {
		return _num_lados;
	}

	/**
	Construye un nuevo polígono a partir de los parámetros de entrada, mediante al utilización de un lápiz.
	Almacena los distintos puntos en el vector que es pasado como parámetro.
	*/
	void construirPoligono(int num_lados, PV2D centro, GLdouble radio, PV2D p[])
	{
		Lapiz lap = Lapiz(centro, 0.0);
		GLdouble alpha = 360.0/(GLdouble)num_lados; //alpha es el angulo entre cada lado.
		GLdouble lado = 2*sin((alpha/2.0)/180*PI)*radio;

		lap.forward(radio, false);
		lap.turn(180-((180-alpha)/2));

		for (int i=0; i<num_lados; i++)
		{
			p[i] = lap.get_pos();
			lap.forward(lado, false);		
			lap.turn(alpha);
		}
	}

	/**
	Mueve el polígono un determinado "vector", desplazando todos los puntos
	*/
	void mover(PV2D vector)
	{
		for(int i=0; i<_num_lados; i++)
			_p[i] = _p[i].sumarVect2Pto(vector);
	}

	/**
	 Dibuja el poligono
	*/
	void draw() {
		
		_colorrelleno.changeColor();

		if(_relleno)
		{	
			glBegin ( GL_POLYGON ) ;
				for (int i = 0; i<_num_lados; i++)
					glVertex2d( _p[i].get_x(), _p[i].get_y() );
			glEnd () ;
		}

		_colorlinea.changeColor();
			
		if(_linea)
		{	
			glBegin ( GL_LINE_LOOP ) ;
				for (int i = 0; i<_num_lados; i++)
					glVertex2d( _p[i].get_x(), _p[i].get_y() );
			glEnd () ;
		}
	}

}; //Poligono

#endif // __POLIGONO_H