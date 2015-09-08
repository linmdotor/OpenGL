/**
@author Jesús Martínez Dotor
*/

#ifndef __ESFERA_H
#define __ESFERA_H

#include <GL/freeglut.h>

#include "PV3D.h"
#include "ObjetoCuadrico.h"

/**
 Clase Esfera. 
*/
class Esfera : public ObjetoCuadrico{

private:
	GLdouble _radio;
	GLint _num_meridianos; //longitudinales, de polo a polo
	GLint _num_paralelos; //latitudinales, que rodean la esfera

public:
	/** Constructores */
	Esfera() :
		_radio(10.0), 
		_num_meridianos(20), _num_paralelos(20) {}

	Esfera(GLdouble r, GLint slices, GLint stacks, bool pintar_caras, RGBColor color_caras, bool pintar_lineas, RGBColor color_lineas ) :
		_radio(r), 
		_num_meridianos(slices), _num_paralelos(stacks) {
		_color = color_caras;
		_colorlineas = color_lineas;
		_pintar_caras = pintar_caras;
		_pintar_lineas = pintar_lineas;
		}
	
	/** Destructor */
	~Esfera() {
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
			gluSphere (_quadric, _radio, _num_meridianos, _num_paralelos);
		}
		if(_pintar_lineas)
		{
			gluQuadricDrawStyle(_quadric, GLU_LINE);
			_colorlineas.changeColor();
			gluSphere (_quadric, _radio, _num_meridianos, _num_paralelos);
		}

	}

}; //Esfera

#endif // __ESFERA_H