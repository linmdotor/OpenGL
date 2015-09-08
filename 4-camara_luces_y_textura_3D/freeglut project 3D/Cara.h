/**
@author Jesús Martínez Dotor
*/

#ifndef __CARA_H
#define __CARA_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

/**
 Clase Cara, que define una cara de un polígono 3D.
*/
class Cara{

private:
	int _num_vertices;
	int** _matrizVN; //matriz de [_num_vert][2] donde almacena los vertices y sus normales

public:
	/** Constructores */
	Cara(int n, int vertice_normal[][2]) : _num_vertices(n) 
	{
		_matrizVN = new int *[n];
		for(int i=0; i<n; i++)
		{
			_matrizVN[i] = new int[2];
			_matrizVN[i][0] = vertice_normal[i][0];
			_matrizVN[i][1] = vertice_normal[i][1]; //por el momento no se rellenan las normales, se rellenan al final del proceso
		}
	}
	
	/** Destructor */
	~Cara() {
		for(int i=0; i<_num_vertices; i++) //borrar columnas
			delete[] _matrizVN[i];
		delete[] _matrizVN; //borrar filas
		delete _matrizVN; //borrar puntero
		_matrizVN = NULL;
	}

	/**	setters y getters */
	
	void setNormal(int ind_normal)
	{
		for (int i=0; i<_num_vertices; i++)
			_matrizVN[i][1] = ind_normal;
	}

	int get_num_vertices() {
		return _num_vertices;
	}

	int getIndiceVerticeK(int k) {
		return _matrizVN[k][0];
	}

	int getIndiceNormalK(int k) {
		return _matrizVN[k][1];
	}

	

}; //Cara

#endif // __CARA_H