/**
@author Jesús Martínez Dotor
*/

#ifndef __CILINDRO_H
#define __CILINDRO_H

#include <GL/freeglut.h>

#include "PV3D.h"
#include "ObjetoCuadrico.h"

/**
 Clase Cilindro. 
*/
class Cilindro : public ObjetoCuadrico{

protected:
	GLdouble _radio_base;
	GLdouble _radio_tapa;
	GLdouble _altura;
	GLint _num_meridianos; //longitudinales, de polo a polo
	GLint _num_paralelos; //latitudinales, que rodean la esfera

public:
	/** Constructores */
	Cilindro() :
		_radio_base(10.0), _radio_tapa(10.0),
		_altura(50.0),
		_num_meridianos(20), _num_paralelos(2) {}

	Cilindro(GLdouble r_base, GLdouble r_top, GLdouble height, GLint slices, GLint stacks, bool pintar_caras, RGBColor color_caras, bool pintar_lineas, RGBColor color_lineas) :
		_radio_base(r_base), _radio_tapa(r_top),
		_altura(height),
		_num_meridianos(slices), _num_paralelos(stacks) {
		_color = color_caras;
		_colorlineas = color_lineas;
		_pintar_caras = pintar_caras;
		_pintar_lineas = pintar_lineas;
		}
	
	/** Destructor */
	~Cilindro() {
	}

	/**
	 Dibuja el Cilindro
	*/
	void draw() {
		/* 
		MODOS DE PINTADO:
		GLU_FILL
			Quadrics are rendered with polygon primitives. The polygons are drawn in a counterclockwise fashion with respect to their normals (as defined with gluQuadricOrientation).

		GLU_LINE
			Quadrics are rendered as a set of lines.

		GLU_POINT
			Quadrics are rendered as a set of points

		GLU_SILHOUETTE
			Quadrics are rendered as a set of lines, except that edges separating coplanar faces will not be drawn.
		*/

		if(_pintar_caras)
		{
			gluQuadricDrawStyle(_quadric, GLU_FILL);
			_color.changeColor();
			gluCylinder (_quadric, _radio_base, _radio_tapa, _altura, _num_meridianos, _num_paralelos);
		}
		if(_pintar_lineas)
		{
			gluQuadricDrawStyle(_quadric, GLU_LINE);
			_colorlineas.changeColor();
			gluCylinder (_quadric, _radio_base, _radio_tapa, _altura, _num_meridianos, _num_paralelos);
		}

	}

}; //Cilindro

#endif // __CILINDRO_H