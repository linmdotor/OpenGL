/**
@author Jesús Martínez Dotor
*/

#ifndef __PINTARVECT_H
#define __PINTARVECT_H

#include <GL/freeglut.h>

#include "Lapiz.h"
#include "PV2D.h"

/**
 Clase PintarVector

@author Jesús Martínez Dotor
*/
class PintarVect {
public:	
	/**
	Dibuja un vector
	*/
	static void draw(PV2D origen, PV2D v, GLdouble redim) {
		
		Lapiz lap = Lapiz(origen, 0.0);
		
		v = PV2D(v.get_x()*redim, v.get_y()*redim); //aumentamos o diminuimos el vector en función de la cantidad deseada

		v = v.sumarVect2Pto(origen);
		lap.lookAt(v);
		lap.moveTo(v, true);
		lap.turn(150);
		lap.forward(1.0, true);
		lap.turn(180);
		lap.forward(1.0, true);
		lap.turn(-120);
		lap.forward(1.0, true);
	}


}; //PINTARVECT

#endif // __PINTARVECT_H