/**
@author Jesús Martínez Dotor
*/

#ifndef __MALLA_H
#define __MALLA_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "Objeto3D.h"
#include "PV3D.h"
#include "Cara.h"
#include "RGBColor.h"

const GLdouble PI = 3.141592653589793;

/**
 Clase Malla, que construye una malla de objeto 3D.
*/
class Malla : public Objeto3D{

protected:
	int _num_vertices;
	PV3D** _vertice;
	int _num_normales; // =numCaras, (en la mayoría de los casos)
	PV3D** _normal;
	int _num_caras;
	Cara** _cara;

public:
	/** Destructor */
	~Malla() {
		delete[] _vertice;	
		_vertice = NULL;
		delete[] _normal;	
		_normal = NULL;
		delete[] _cara;
		_cara = NULL;
	}

	/*
	 Calcula la normal a una cara mediante Newell
	*/
	PV3D calcularVectorNormalNewell(Cara* c)
	{
		PV3D normal;

		for(int i=0; i<c->get_num_vertices(); i++)
		{							
			PV3D* v_act =_vertice[c->getIndiceVerticeK(i)];
			PV3D* v_sig =_vertice[(c->getIndiceVerticeK((i+1)% c->get_num_vertices()))];

			normal = PV3D((v_act->get_y() - v_sig->get_y()) * (v_act->get_z() + v_sig->get_z()), 
						(v_act->get_z() - v_sig->get_z()) * (v_act->get_x() + v_sig->get_x()), 
						(v_act->get_x() - v_sig->get_x()) * (v_act->get_y() + v_sig->get_y()), 
						false);
		}

		return normal.normalizarVect();
	}

	/*
	 Calcula todas las normales, llamando a Newell por cada cara.
	*/
	void calcularNormales()
	{
		for (int i=0; i<_num_caras; i++)
		{
			_normal[i] = new PV3D(calcularVectorNormalNewell(_cara[i]));
			_num_normales++;
			_cara[i]->setNormal(i);
		}
	}


	/*
	 Dibuja toda la malla independientemente de cómo esté construida la malla.
	*/
	void draw()
	{	
		if(_pintar_caras)
		{
			_color.changeColor();
			dibuja(GL_POLYGON);
		}
		if(_pintar_lineas)
		{
			_colorlineas.changeColor();
			dibuja(GL_LINE_LOOP);
		}
	}

protected:

	/*
	 Método auxiliar para pintar la malla

	 Los modos admitidos son:
		- GL_POLYGON -> dibuja las caras rellenas
		- GL_LINE_LOOP -> dibuja las aristas (sin rellenas las caras)
		- GL_POINTS -> dibuja solo los puntos de los vértices
	*/
	void dibuja(GLenum mode)
	{	
		if(_textureID != 0) //si tiene textura, las activo
			glEnable(GL_TEXTURE_2D);

		for (int i=0; i<_num_caras; i++)
		{	
			

			glLineWidth(1.0);
			glBegin(mode);
			
			for(int j=0; j<_cara[i]->get_num_vertices(); j++)
			{
				int iN = _cara[i]->getIndiceNormalK(j);
				int iV = _cara[i]->getIndiceVerticeK(j);
				
				glNormal3d(_normal[iN]->get_x(), _normal[iN]->get_y(), _normal[iN]->get_z());
				
				if(_textureID != 0) //si tiene textura
				{
					switch(j) { //se agrega textura a la cara
						case 0: glTexCoord2f(0, 0); break;
						case 1: glTexCoord2f(1, 0); break;
						case 2: glTexCoord2f(1, 1); break;
						case 3: glTexCoord2f(0, 1); break;
					}
					
				}

				glVertex3d(_vertice[iV]->get_x(), _vertice[iV]->get_y(), _vertice[iV]->get_z());	
			}

			glEnd();			
		}

		glDisable(GL_TEXTURE_2D); //desactivo simpre, por si acaso
	}

}; //Malla

#endif // __MALLA_H