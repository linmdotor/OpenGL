/**
@author Lin M. Dotor
*/

#ifndef __OBSTACULO_H
#define __OBSTACULO_H

#include <GL/freeglut.h>

#include "PV2D.h"
#include "Pelota.h"

#define _epsilon 0.0000001


/**
 Clase Obtaculo, que sirve de interfaz para todos los obstaculos de la escena.
 Operaciones: 
	- intersección: Obstaculo, Pelota -> bool(colision), PV2D(normal), GLdouble(tHit)

@author Lin M. Dotor
*/

class Obstaculo {

public:	

	virtual bool interseccion(PV2D p, PV2D v, PV2D v_1, PV2D &n, GLdouble &hit, bool usar_mascaras) = 0;
	virtual void draw(bool pintar_normales, bool pintar_mascaras) = 0;

	virtual void setColor(RGBColor c) = 0;
	virtual void setColorLinea(RGBColor c) = 0;

}; //Obstaculo

#endif // __OBSTACULO_H