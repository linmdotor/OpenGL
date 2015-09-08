/**
@author Lin M. Dotor
*/

#ifndef __DISCOPARCIAL_H
#define __DISCOPARCIAL_H

#include <GL/freeglut.h>

#include "PV3D.h"
#include "ObjetoCuadrico.h"

/**
 Clase DiscoParcial. 
*/
class DiscoParcial : public ObjetoCuadrico{

protected:
	GLdouble _radio_dentro;
	GLdouble _radio_fuera;
	GLint _num_meridianos; //longitudinales, de polo a polo
	GLint _num_anillos; //latitudinales, que rodean la esfera
	GLdouble _angulo_inicio;
	GLdouble _angulo_fin;

public:
	/** Constructores */
	DiscoParcial() :
		_radio_dentro(5.0), _radio_fuera(15.0),
		_num_meridianos(20), _num_anillos(20),
		_angulo_inicio(0.0), _angulo_fin(180.0) {}

	DiscoParcial(GLdouble r_inner, GLdouble r_outer, GLint slices, GLint rings, GLdouble startAngle, GLdouble sweepAngle, bool pintar_caras, RGBColor color_caras, bool pintar_lineas, RGBColor color_lineas) :
		_radio_dentro(r_inner), _radio_fuera(r_outer),
		_num_meridianos(slices), _num_anillos(rings),
		_angulo_inicio(startAngle), _angulo_fin(sweepAngle) {
		_color = color_caras;
		_colorlineas = color_lineas;
		_pintar_caras = pintar_caras;
		_pintar_lineas = pintar_lineas;
		}
	
	/** Destructor */
	~DiscoParcial() {
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
			gluPartialDisk (_quadric, _radio_dentro, _radio_fuera, _num_meridianos, _num_anillos, _angulo_inicio, _angulo_fin);
		}
		if(_pintar_lineas)
		{
			gluQuadricDrawStyle(_quadric, GLU_LINE);
			_colorlineas.changeColor();
			gluPartialDisk (_quadric, _radio_dentro, _radio_fuera, _num_meridianos, _num_anillos, _angulo_inicio, _angulo_fin);
		}

	}

}; //DiscoParcial

#endif // __DISCOPARCIAL_H