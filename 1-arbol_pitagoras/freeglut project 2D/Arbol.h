#ifndef __ARBOL_H
#define __ARBOL_H

#include <GL/freeglut.h>

#include "Lista.h"
#include "Cuadrado.h"

#include <iostream>
using namespace std;

#include <math.h> // pow()

//RGBColor MARRON = RGBColor(0.3, 0.2, 0.1);
//RGBColor VERDE = RGBColor(0.0, 1.0, 0.0);

/**
 Clase Arbol, que almacena 2 listas de Cuadrados y 1 ángulo.
	 - _listacuadrados -> Cuadrados ya calculados (internos) del Arbol
	 - _listafrontera -> Cuadrados que son las hojas del Arbol
	 - _angulo -> ángulo derecho del triangulo formado al anidar 2 nuevos Cuadrados

 Operaciones: 
	- draw:  Arbol -> 
	- seleccionar: Arbol, Punto, GLdouble -> Bool
	- anidar: Arbol ->

@author Jesús Martínez Dotor
*/
class Arbol {

private:
	// Listas de Cuadrados
	Lista<Cuadrado> _listacuadrados;
	int _nivel;
	//Lista<Cuadrado> _listafrontera;

	//Ángulo derecho del triángulo de Pitágoras
	GLdouble _angulo;

public:
	/** Constructores */
	Arbol() {
		Cuadrado c = Cuadrado();
		c.set_colorrelleno(RGBColor(0.3, 0.2, 0.1));
		c.set_linea(false);
		_listacuadrados.ponDr(c);
		_nivel = 1;
		_angulo = 45.0;
	}

	Arbol(Cuadrado cuad, GLdouble ang) {
		_listacuadrados.ponDr(cuad);
		_nivel = 1;
		_angulo = ang;
	}

	/** Destructor */
	~Arbol() {
	}

	/**
	 Dibuja la escena
	*/
	void draw() {
		int numeroElemsTotales = pow(2.0, _nivel) - 1;

		Lista<Cuadrado>::Iterador it_cuad = _listacuadrados.principio();
		for(int i = 0; i < numeroElemsTotales; i++)
		{
			Cuadrado c = it_cuad.elem();
			c.draw();
			it_cuad.avanza();
		}
	}

	/**
	 Determina si algún vertice de algún cuadrado está "cerca" de un punto dado
	 Debe estar al menos a lado/3 de algún vértice
	*/
	void seleccionar(Cuadrado* &c_original, Punto p, GLdouble distancia_min) {
		bool encontrado = false;
		Cuadrado* c_aux;

		int numeroElemsTotales = pow(2.0, _nivel) - 1;
		Lista<Cuadrado>::Iterador it_cuad = _listacuadrados.principio();
		for(int i = 0; i < numeroElemsTotales && !encontrado; i++)
		{
			c_aux = &it_cuad.elem();
			//distancia_min = c_aux->get_p(0).distnciaPto2Pto(c_aux->get_p(1))/3; //esto es por si se quiere seleccionar respecto al lado/3
			encontrado = c_aux->seleccionar(p, distancia_min);
			it_cuad.avanza();
		}

		if(c_original != NULL) //el original siempre lo deselecciono
		{
			c_original->set_seleccionado(false);
			c_original = NULL;
		}
		
		if (encontrado)
		{		
			c_aux->set_seleccionado(true);
			c_original = c_aux;
		}
	}

	/**
	 Calcula los siguientes triangulos de las fronteras, y almacena estas.
	*/
	void anidar() {
		int numeroElemsTotales = pow(2.0, _nivel) - 1;
		int numeroElemsFrontera = pow(2.0, _nivel-1);

		if (_listacuadrados.numElems() <= numeroElemsTotales) //no hay más elementos creados en la lista (se calculan más)
		{
			//Avanza un iterador hasta que se pone al principio de los "frontera"
			Lista<Cuadrado>::Iterador it_cuad = _listacuadrados.principio();
			for(int i = 0; i < numeroElemsFrontera-1; i++)
					it_cuad.avanza();

			//recorre los que son "frontera", y los anida, añadiendo los nuevos detrás
			for(int i = 0; i < numeroElemsFrontera; i++)
			{
				Cuadrado cuad = it_cuad.elem();
				Cuadrado cuadnuevo1;
				Cuadrado cuadnuevo2;
				//calculo los 2 siguientes, guardandolos en una nueva lista frontera.
				cuad.anidar(cuadnuevo1, cuadnuevo2, _angulo);			
			
				 /* genera aleatorio entre -0.05 y 0.05: */
				GLdouble porcentaje_transicion;
				RGBColor color_modificado;

				porcentaje_transicion = generar_aleatorio(0.0, 0.2);
				color_modificado = RGBColor(cuad.get_colorrelleno());
				color_modificado.transicion(RGBColor(0.0, 1.0, 0.0), porcentaje_transicion);
				cuadnuevo1.set_colorrelleno(color_modificado);
				cuadnuevo1.set_linea(false);

				porcentaje_transicion = generar_aleatorio(0.0, 0.2);
				color_modificado = RGBColor(cuad.get_colorrelleno());
				color_modificado.transicion(RGBColor(0.0, 1.0, 0.0), porcentaje_transicion);
				cuadnuevo2.set_colorrelleno(color_modificado);
				cuadnuevo2.set_linea(false);


				_listacuadrados.ponDr(cuadnuevo1);
				_listacuadrados.ponDr(cuadnuevo2);

				it_cuad.avanza();
			}

			_nivel++;
		}
		else //hay elementos creados anteriormente en la lista (solamente se incrementa el nivel)
		{
			_nivel++;
		}
	}

	/**
	 Desanida un nivel (deja de mostrar los de niveles superiores).
	*/
	void desanidar() {
		if(_nivel > 1)
			_nivel--;
	}

	/**
	 Genera un aleatorio GLdouble entre 2 límites
	*/
	GLdouble generar_aleatorio(GLdouble min, GLdouble max) {
		return (((GLdouble) rand()*(max-min))/ (GLdouble)RAND_MAX + min);
	}




}; //Arbol

#endif // __ARBOL_H