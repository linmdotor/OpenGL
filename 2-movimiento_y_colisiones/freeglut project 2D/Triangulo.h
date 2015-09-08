/**
@author Jesús Martínez Dotor
*/

#ifndef __TRIANGULO_H
#define __TRIANGULO_H

#include <GL/freeglut.h>

#include "PV2D.h"
#include "Obstaculo.h"
#include "Poligono.h"
#include "PintarVect.h"
#include "Pelota.h"

/**
 Clase Triangulo, que implementa el obstaculo, añadiendo características de los triangulos.
 Contiene:
	- v[3] -> posición de los 3 vertices
	- n[3] -> normales de los 3 lados
	- n_v[3] -> "normales" (inventadas) de los vertices.

	- Poligono -> poligono regular de 3 lados que se emplea para representarlo.
 
 Operaciones: 
	- intersección: Triangulo, Pelota -> bool(colision), PV2D(normal), GLdouble(tHit)
	- draw:  Triangulo -> 

@author Jesús Martínez Dotor
*/
class Triangulo : public Obstaculo{

protected:
	Poligono _poligono;

	PV2D _v[3];
	PV2D _n[3]; //normal 0 = normal del lado 0 a 1
	PV2D _n_v[3];

public:
	/** Constructores */
	Triangulo()	{}

	Triangulo(PV2D v0, PV2D v1, PV2D v2)
	{
		_v[0] = v0;
		_v[1] = v1;
		_v[2] = v2;
		_poligono = Poligono(3, _v);
		calcularNormales();		
	}

	Triangulo(PV2D v[]) : 
		_poligono(Poligono(3, v)) {
		for (int i=0; i<3; i++)
			_v[i] = v[i];
		calcularNormales();		
	}
	
	/** Destructor */
	~Triangulo() {
	}





	bool interseccion(PV2D p, PV2D v, PV2D v_1, PV2D &n, GLdouble &hit, bool usar_mascaras)
	{
		GLdouble dist[3], proy[3]; int sign[3];
		int nHits= 0; double t_hit[3]; PV2D norm[3]; //tiempos de colision y normales

		for(int i=0; i<3; i++)
		{
			PV2D vector_p_pi = p.crearVector_RestaPto2Pto(_v[i]);//.normalizarVect();
			
			dist[i] = vector_p_pi.productoEscalar(v_1.vectorNormal()); //distancia de pi a la recta R
			proy[i] = vector_p_pi.productoEscalar(v_1); //proyeccion de p_pi sobre v

			if (dist[i] > _epsilon) //vertice a la izqd de R
				sign[i] = 1;
			else if (dist[i] < -(_epsilon)) //vertice a la drcha de R
				sign[i] = -1;
			else //vertice interseca con R 
			{
				sign[i] = 0;

				t_hit[nHits] = proy[i]; //(ya que sabemos que interseca, calculamos directamente el t_hit y normal de interseccion)
				norm[nHits] = _n_v[i];
				nHits++;
			}
		}

		//calculo de las colisiones con lados
		if(abs(sign[0] + sign[1] + sign[2]) == 3) //no hay colisión (los 3 vertices a un lado u otro)
		{
			return false;
		}
		else //puede haber colision con algun lado
		{
			for(int i=0; i<3; i++)
			{
				int j = (i+1)%3;
				if(sign[i]*sign[j] < 0) //un vertice se encuentra a drcha y otro a izqda (interseccion con algún lado)
				{
					t_hit[nHits] = (proy[i]*dist[j] - dist[i]*proy[j]) / (dist[j]-dist[i]);
					norm[nHits] = _n[i];
					nHits++;
				}			
			}
		}

		int min = 0;
		//buscamos la colisión mas cercana (t_hit mas corto)
		for(int i=0; i<nHits; i++)
		{
			if(t_hit[min] > t_hit[i])
				min = i;
		}

		//sustituimos los valores de salida
		n = norm[min];
		hit = t_hit[min]/v.moduloVect(); //se divide entre el modulo del vector avance para hacer que sea 1 en cada paso
		return true;
	}

	void draw(bool pintar_normales, bool pintar_mascaras)
	{
		_poligono.draw();
		if(pintar_normales)
		{
			for (int i=0; i<3; i++)
			{
				PintarVect::draw(_v[i].puntoMedio(_v[(i+1)%3]), _n[i], 10.0);
				PintarVect::draw(_v[i], _n_v[i], 10.0); //normales de los vertices
			}
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

protected:
	void calcularNormales()
	{	//centro de masas
		PV2D baricentro = _v[0].sumarVect2Vect(_v[1]).sumarVect2Vect(_v[2]).multiplicarVect2Escalar(1.0/3.0);

		for (int i=0; i<3; i++)
		{
			PV2D lado= _v[(i+1)%3].crearVector_RestaPto2Pto(_v[i]); //vector de i+1 a i
			_n[i] = lado.vectorNormal().normalizarVect(); //normal de i a i+1

			_n_v[i] = baricentro.crearVector_RestaPto2Pto(_v[i]).normalizarVect();//normal del vertice i (desde el baricentro a i)
		}
	}

}; //Triangulo

#endif // __TRIANGULO_H