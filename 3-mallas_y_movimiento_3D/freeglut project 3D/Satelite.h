/**
@author Lin M. Dotor
*/

#ifndef __SATELITE_H
#define __SATELITE_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "Malla.h"

#define lados_circ 12 //nº de lados con el que aproximaremos la circunferencia
#define capas_antena 15 //nº de capas (o divisiones) ue haremos en la antena del satelite
#define radio_satelite 30.0 //radio de la circunferencia del satelite

/**
 Clase Satelite, que construye la malla del satélite en su constructor.
*/
class Satelite: public Malla{

public:
	Satelite()
	{
		//se inicializan los Arrays de vertices, caras y normales		
		_num_vertices = (3*lados_circ)+(capas_antena*lados_circ);
		_vertice = new PV3D*[_num_vertices];		
		_num_caras = 2*((2*lados_circ)+(capas_antena*lados_circ))+1;
		_cara = new Cara*[_num_caras];
		_num_normales = 2*((2*lados_circ)+(capas_antena*lados_circ))+1;
		_normal = new PV3D*[_num_normales];

		//se calculan los vertices de la parte inferior(el ciclindro)
		PV3D centro_actual = PV3D();	
		GLdouble radio_actual = radio_satelite;
		calcularVerticesRodaja(_vertice, lados_circ, lados_circ*0, centro_actual, radio_actual, XZ);

		centro_actual = centro_actual.sumarVect2Pto(PV3D(0.0, radio_satelite*2, 0.0, false));
		radio_actual = radio_satelite;
		calcularVerticesRodaja(_vertice, lados_circ, lados_circ*1, centro_actual, radio_actual, XZ);

		//se calculan los vertices de la unión entre la antena y el ciclindro
		centro_actual = centro_actual.sumarVect2Pto(PV3D(0.0, radio_satelite/10, 0.0, false));
		radio_actual = radio_satelite/10;
		calcularVerticesRodaja(_vertice, lados_circ, lados_circ*2, centro_actual, radio_actual, XZ);

		//se calculan los vertices de la antena
		GLdouble incremento_altura = radio_satelite/capas_antena;
		GLdouble incremento_radio = (radio_satelite-radio_actual)/capas_antena;

		for(int i=0; i<capas_antena; i++)
		{
			centro_actual = centro_actual.sumarVect2Pto(PV3D(0.0, incremento_altura, 0.0, false));
			radio_actual = radio_actual+incremento_radio;
			calcularVerticesRodaja(_vertice, lados_circ, lados_circ*(3+i), centro_actual, radio_actual, XZ);
		}
			
		//se calculan las caras que unen los vertices de todo el satélite
		int arrayVNcara_tapa[lados_circ][2]; //tapa del satelite
		for(int i=0; i<lados_circ; i++) 
		{
			arrayVNcara_tapa[i][0] = i;
			arrayVNcara_tapa[i][1] = 0;
		}
		_cara[_num_caras-1] = new Cara(lados_circ, arrayVNcara_tapa);

		for(int i=0; i<capas_antena+2; i++)
		{
			for(int j=0; j<lados_circ; j++)
			{

				int arrayVNcara_aux[3][2];
			
				arrayVNcara_aux[0][1] = 0;
				arrayVNcara_aux[1][1] = 0;
				arrayVNcara_aux[2][1] = 0;

				int _i = (lados_circ*i)+j; //i
				int suc_i = (lados_circ*i)+((j+1)%lados_circ); //suc(i)
				int _i_N = ((lados_circ*i)+j+lados_circ)%_num_vertices; //i+N
				int suc_i_N = ((lados_circ*i)+((j+1)%lados_circ)+lados_circ)%_num_vertices; //suc(i)+N

				arrayVNcara_aux[0][0] = suc_i;
				arrayVNcara_aux[1][0] = _i;
				arrayVNcara_aux[2][0] = suc_i_N;

				_cara[(lados_circ*i)+j] = new Cara(3, arrayVNcara_aux);

				arrayVNcara_aux[0][0] = _i;
				arrayVNcara_aux[1][0] = _i_N;
				arrayVNcara_aux[2][0] = suc_i_N;

				_cara[(lados_circ*(i+capas_antena+2))+j] = new Cara(3, arrayVNcara_aux);
			}
		}
		
		//calcula las normales de todas las caras
		calcularNormales();

		set_color(RGBColor(0.0, 1.0, 0.0));
	}
	
	/**
	Construye un nuevo polígono a partir de los parámetros de entrada.
	Almacena los distintos puntos en el vector que es pasado como parámetro.
	*/
	typedef enum {XY, XZ, YZ} Plano;
	void construirPoligonoEnPlano(int num_lados, PV3D centro, GLdouble radio, PV3D p[], int plano)
	{
		GLdouble inc = (2*PI/num_lados);

		for (int i=0; i<num_lados; i++)
		{
			switch (plano) {
			case XY:  
				p[i] = PV3D(radio*cos(2*PI-i*inc), radio*sin(2*PI-i*inc), centro.get_z(), true); 
				break;

			case XZ:  
				p[i] = PV3D(-radio*cos(2*PI-i*inc), centro.get_y(), radio*sin(2*PI-i*inc), true); 
				break;

			case YZ:  
				p[i] = PV3D(centro.get_x(), radio*cos(2*PI-i*inc), radio*sin(2*PI-i*inc), true); 
				break;
			}
		}
	}

	/*
	 Copia un determinado array, en las posiciones determinadas de un array de punteros
	*/
	void copiarArrayEnArrayPunteros(PV3D origen[], PV3D** destino, int num_elems_origen, int dest_min)
	{
		for(int i=0; i<num_elems_origen; i++)
		{
			destino[dest_min+i] = new PV3D(origen[i]);
		}
	}

	/*
	 Calcula todos los vértices de una rodaja del satélite 
	*/
	void calcularVerticesRodaja(PV3D** array_vertices, int num_vertices, int vert_inicial, PV3D centro, GLdouble radio, int plano)
	{
		PV3D* verticesRodaja = new PV3D[num_vertices];
		construirPoligonoEnPlano(num_vertices, centro, radio, verticesRodaja, plano);
		copiarArrayEnArrayPunteros(verticesRodaja, array_vertices, num_vertices, vert_inicial);
		delete[] verticesRodaja;
	}

}; //Satelite

#endif // __SATELITE_H