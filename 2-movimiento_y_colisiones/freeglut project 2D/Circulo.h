/**
@author Lin M. Dotor
*/

#ifndef __CIRCULO_H
#define __CIRCULO_H

#include <GL/freeglut.h>
#include <algorithm>

#include "PV2D.h"
#include "Obstaculo.h"
#include "Poligono.h"
#include "Pelota.h"

/**
 Clase Circulo, que implementa el obstaculo, añadiendo características de los círculos.
 Contiene:
	- centro -> posición del centro de la pelota
	- radio -> longitud del radio
	
	- Poligono -> poligono regular de n lados que se emplea para representarlo.
 
 
 Operaciones: 
	- intersección: Circulo, Pelota -> bool(colision), PV2D(normal), GLdouble(tHit)
	- draw:  Circulo -> 

@author Lin M. Dotor
*/
class Circulo : public Obstaculo{

protected:
	Poligono _poligono;

	PV2D _centro;
	GLdouble _radio;

	PV2D _normal;
	PV2D _origen_normal;

public:
	/** Constructores */
	Circulo() {}

	Circulo(PV2D centro, GLdouble radio) :
		_centro(centro), _radio(radio), 
		_poligono(Poligono(20, centro, radio)) {}
	
	/** Destructor */
	~Circulo() {
	}


	bool interseccion(PV2D p, PV2D v, PV2D v_1, PV2D &n, GLdouble &hit, bool usar_mascaras)
	{
		//ecuación de segundo grado A^2 + Bt + C = 0, donde:
			//A = (v_1*v_1) ; B = 2*(CP * v_1) ; C = CP*CP - R^2

		PV2D v_cp = _centro.crearVector_RestaPto2Pto(p);

		GLdouble A = v_1.productoEscalar(v_1);
		GLdouble B = 2*(v_cp.productoEscalar(v_1));
		GLdouble C = v_cp.productoEscalar(v_cp) - pow(_radio, 2.0);

		GLdouble discriminante = pow(B, 2.0) - 4*A*C;

		if(discriminante < -(_epsilon)) //no hay intersección
		{
			_origen_normal = PV2D(0.0, 0.0);
			_normal = PV2D(0.0, 0.0);

			return false;
		}
		if(discriminante > _epsilon) // hay intersección en 2 ptos
		{
			hit = min((-B-sqrt(discriminante))/(2*A), (-B+sqrt(discriminante))/(2*A)) /v.moduloVect();
			PV2D punto_choque = p.sumarVect2Pto(v.multiplicarVect2Escalar(hit));
			n = _centro.crearVector_RestaPto2Pto(punto_choque).normalizarVect();

			_origen_normal = punto_choque;
			_normal = n;
		}
		else //hay interseccion en 1 pto
		{
			hit = -B/(2*A) /v.moduloVect();
			PV2D punto_choque = p.sumarVect2Pto(v.multiplicarVect2Escalar(hit));
			n = _centro.crearVector_RestaPto2Pto(punto_choque).normalizarVect();

			_origen_normal = punto_choque;
			_normal = n;			
		}

		return true;		
	}

	void draw(bool pintar_normales, bool pintar_mascaras)
	{
		_poligono.draw();
		if(pintar_normales)
		{
			PintarVect::draw(_origen_normal, _normal, 10.0);
			PintarVect::draw(_origen_normal, _normal, 10.0);
		}
	}

	void setColor(RGBColor c)
	{
		_poligono.set_colorrelleno(c);
	}

	void setColorLinea(RGBColor c)
	{
		_poligono.set_colorlinea(c);
	}

}; //Circulo

#endif // __CIRCULO_H