/**
@author Jesús Martínez Dotor
*/

#ifndef __TRIANGULOCONMASCARA_H
#define __TRIANGULOCONMASCARA_H

#include <GL/freeglut.h>

#include "Triangulo.h"
#include "Circulo.h"
#include "PV2D.h"
#include "Obstaculo.h"
#include "Poligono.h"
#include "PintarVect.h"
#include "Pelota.h"

/**
 Clase TrianguloConMascara, que hereda de Triangulo, añadiendo una mascara, para que la pelota no penetre en él. 
 La mascara está formada por 3 circulos en cada vértice (con el radio de la pelota) y 3 triangulos en cada lado.

@author Jesús Martínez Dotor
*/
class TrianguloConMascara : public Triangulo{

protected:
	Obstaculo* _mascara[6];
	GLdouble _radio_mascara;

public:
	/** Constructores */

	TrianguloConMascara(PV2D v[], GLdouble radio_mascara) :
		Triangulo(v), 
		_radio_mascara(radio_mascara) {
		crearMascara();
	}
	
	/** Destructor */
	~TrianguloConMascara() {
	}



	bool interseccion(PV2D p, PV2D v, PV2D v_1, PV2D &n, GLdouble &hit, bool usar_mascaras) 
	{
		if(usar_mascaras)
		{ 
			PV2D n_aux, n_min;
			GLdouble hit_aux, hit_min = 2;
			bool colision = false;

			for(int i=0; i<6; i++)
			{
				if (_mascara[i]->interseccion(p, v, v_1, n_aux, hit_aux, 0)  &&  hit_aux < hit_min)
				{
					hit_min = hit_aux;
					n_min = n_aux;
					colision = true;
				}
			}

			n = n_min;
			hit = hit_min;
			return colision;
		}
		else
			return this->Triangulo::interseccion(p, v, v_1, n, hit, 0);
	}

	void draw(bool pintar_normales, bool pintar_mascaras)
	{
		if(pintar_mascaras)
			for(int i=0; i<6; i++)
				_mascara[i]->draw(pintar_normales, 0);
		this->Triangulo::draw(pintar_normales, 0);
	}

	void crearMascara()
	{// LOS TRIANGULOS HAY QUE CALCULARLOS BIEN

		for (int i=0; i<3; i++) //circulos por cada vertice
		{
			_mascara[i] = new Circulo(_v[i], _radio_mascara);
			_mascara[i]->setColor(RGBColor(0.8, 0.8, 0.8));
			_mascara[i]->setColorLinea(RGBColor(0.8, 0.8, 0.8));
		}

		for (int i=0; i<3; i++) //triangulos por cada lado
		{
			PV2D vector_suma = _n[i].multiplicarVect2Escalar(_radio_mascara);
			_mascara[i+3] = new Triangulo(_v[i], _v[i].sumarVect2Pto(vector_suma), _v[(i+1)%3].sumarVect2Pto(vector_suma));
			_mascara[i+3]->setColor(RGBColor(0.8, 0.8, 0.8));
			_mascara[i+3]->setColorLinea(RGBColor(0.8, 0.8, 0.8));
		}
	}


}; //TrianguloConMascara

#endif // __TRIANGULOCONMASCARA_H