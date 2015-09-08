/**
@author Lin M. Dotor
*/

#ifndef __ESPIRAL_H
#define __ESPIRAL_H

#include <GL/freeglut.h>

#include "Malla.h"

#define lados_circ 12 //nº de lados con el que aproximaremos la circunferencia de la espiral
#define capas_espiral 150 //nº de capas (o divisiones/iteraciones) ue haremos en la espiral
#define radio_espiral 110.0 //radio de la circunferencia del satelite
#define avance_iteracion 10.0 //avance en "t" que se hará en cada iteracion de la espiral

/**
 Clase Espiral, que construye la malla de la espiral mediante exrusión por el marco de Frenet.
*/
class Espiral: public Malla{

public:
	int _perfil; //perfil de puntos

	Espiral()
	{
		/* 
		ECUACIONES PARAMÉTRICAS DE LA ESPIRAL:
			x(t) = cos(t) + t*sen(t)
			y(t) = 0
			z(t) = sen(t) – t*cos(t)
		*/

		//se inicializan los Arrays de vertices, caras y normales	
		_num_vertices = ((capas_espiral+1)*lados_circ);
		_vertice = new PV3D*[_num_vertices];		
		_num_caras = 2*(capas_espiral*lados_circ);
		_cara = new Cara*[_num_caras];
		_num_normales = 2*(capas_espiral*lados_circ);
		_normal = new PV3D*[_num_normales];

		PV3D** _perfil = new PV3D*[lados_circ]; //necesitamos el perfil original para poder realizar las transformaciones
		calcularVerticesRodaja(_perfil, lados_circ, 0, PV3D(), radio_espiral, XY);

		GLdouble t = avance_iteracion;
		GLdouble matriz[16];

		//calcula los vértices
		calcularVerticesRodaja(_vertice, lados_circ, 0, PV3D(), 0.0, XY); //punto de inicio

		for(int i=1; i<capas_espiral+1; i++)
		{
			calculaMatrizTransformacion(matriz, t);
			for(int j=0; j<lados_circ; j++)
			{
				_vertice[(lados_circ*i)+j] = aplicaMatrizAPunto(matriz, PV3D(_perfil[j])).clona();
			}
			t = t + avance_iteracion;
		}

		//calcula las caras
		for(int i=0; i<capas_espiral; i++)
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

				_cara[(lados_circ*(i+capas_espiral))+j] = new Cara(3, arrayVNcara_aux);
			}
		}
			
		//calcula las normales de todas las caras
		calcularNormales();

		set_color(RGBColor(1.0, 0.0, 0.0));

	}

	/* MÉTODOS PAR LA CONSTRUCCIÓN DE LA MATRIZ DE ESPIRAL */

	PV3D c(GLdouble t)
	{	
		GLdouble t_rad = t/180.0*PI;
		return PV3D(cos(t_rad)+(t*sin(t_rad)), 0, sin(t_rad)-(t*cos(t_rad)), true);	
	}

	PV3D cDerivada(GLdouble t)
	{
		GLdouble t_rad = t/180.0*PI;
		return PV3D(t*cos(t_rad), 0, t*sin(t_rad), true);
	}

	PV3D cDerivadaSegunda(GLdouble t)
	{
		GLdouble t_rad = t/180.0*PI;
		return PV3D(cos(t_rad)-(t*sin(t_rad)), 0, sin(t_rad)+(t*cos(t_rad)), true);
	}

	void calculaMatrizTransformacion(GLdouble m[16], GLdouble t)
	{
    	PV3D C = c(t);
    	PV3D cP = cDerivada(t);
    	PV3D cPP = cDerivadaSegunda(t);        	
		PV3D T = cP.normalizarVect();      			
		PV3D B = cP.productoVectorial(cPP).normalizarVect();
    	PV3D N = B.productoVectorial(T);

		m[0]  = N.get_x();    	
    	m[1]  = N.get_y();
    	m[2]  = N.get_z();
    	m[3]  = 0.0;

    	m[4]  = B.get_x();
    	m[5]  = B.get_y();
    	m[6]  = B.get_z();
    	m[7]  = 0.0;

    	m[8]  = T.get_x();
    	m[9]  = T.get_y();
    	m[10] = T.get_z();
    	m[11] = 0.0;

    	m[12] = C.get_x();
    	m[13] = C.get_y();
    	m[14] = C.get_z();
    	m[15] = 1.0;
	}

	PV3D aplicaMatrizAPunto(GLdouble m[], PV3D p)
	{
		return PV3D(m[0]*p.get_x()+m[4]*p.get_y()+m[8]*p.get_z()+m[12]*1, 
					m[1]*p.get_x()+m[5]*p.get_y()+m[9]*p.get_z()+m[13]*1, 
					m[2]*p.get_x()+m[6]*p.get_y()+m[10]*p.get_z()+m[14]*1, 
					true);
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
				p[i] = PV3D(centro.get_x()+radio*cos(2*PI-i*inc), centro.get_y()+radio*sin(2*PI-i*inc), centro.get_z(), true); 
				break;

			case XZ:  
				p[i] = PV3D(centro.get_x()-radio*cos(2*PI-i*inc), centro.get_y(), centro.get_z()+radio*sin(2*PI-i*inc), true); 
				break;

			case YZ:  
				p[i] = PV3D(centro.get_x(), centro.get_y()+radio*cos(2*PI-i*inc), centro.get_z()+radio*sin(2*PI-i*inc), true); 
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
	 Calcula todos los vértices de una rodaja de la espiral
	*/
	void calcularVerticesRodaja(PV3D** array_vertices, int num_vertices, int vert_inicial, PV3D centro, GLdouble radio, int plano)
	{
		PV3D* verticesRodaja = new PV3D[num_vertices];
		construirPoligonoEnPlano(num_vertices, centro, radio, verticesRodaja, plano);
		copiarArrayEnArrayPunteros(verticesRodaja, array_vertices, num_vertices, vert_inicial);
		delete[] verticesRodaja;
	}

}; //Espiral

#endif // __ESPIRAL_H