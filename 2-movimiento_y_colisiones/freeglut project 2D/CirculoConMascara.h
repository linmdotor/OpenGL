/**
@author Jesús Martínez Dotor
*/

#ifndef __CIRCULOCONMASCARA_H
#define __CIRCULOCONMASCARA_H

#include <GL/freeglut.h>

#include "Circulo.h"
#include "PV2D.h"
#include "Obstaculo.h"
#include "Poligono.h"
#include "Pelota.h"

/**
 Clase CirculoConMascara, que hereda de Circulo, añadiendo una mascara, para que la pelota no penetre en él.
 La máscara está formada por un circulo con el radio de la pelota

@author Jesús Martínez Dotor
*/
class CirculoConMascara : public Circulo{

private:
	Circulo* _mascara;
	GLdouble _radio_mascara;

public:
	/** Constructores */

	CirculoConMascara(PV2D centro, GLdouble radio, GLdouble radio_mascara) :
		Circulo(centro, radio), 
		_radio_mascara(radio_mascara) {
		_mascara = new Circulo(centro, radio+radio_mascara);
		_mascara->setColor(RGBColor(0.8, 0.8, 0.8));
		_mascara->setColorLinea(RGBColor(0.8, 0.8, 0.8));
	}
	
	/** Destructor */
	~CirculoConMascara() {
	}


	bool interseccion(PV2D p, PV2D v, PV2D v_1, PV2D &n, GLdouble &hit, bool usar_mascaras)
	{
		if(usar_mascaras)
			return _mascara->interseccion(p, v, v_1, n, hit, 0);
		else
			return this->Circulo::interseccion(p, v, v_1, n, hit, 0);
	}

	void draw(bool pintar_normales, bool pintar_mascaras)
	{
		if(pintar_mascaras)
			_mascara->draw(pintar_normales, 0);
		this->Circulo::draw(pintar_normales, 0);
	}

}; //CirculoConMascara

#endif // __CIRCULOCONMASCARA_H