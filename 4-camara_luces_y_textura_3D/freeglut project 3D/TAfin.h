/**
@author Jesús Martínez Dotor
*/

#ifndef __TAFIN_H
#define __TAFIN_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "PV3D.h"

/**
 Clase TAfin, que construye determina la matriz de una transformación afín.
*/
class TAfin{

protected:
	GLfloat _m[16];

public:
	/** Constructor */
	TAfin() {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glGetFloatv(GL_MODELVIEW_MATRIX, _m);
		glPopMatrix();
	}

	/** Destructor */
	~TAfin() {
	}

	GLfloat* get_m() {
		return _m;
	}

	/** Métodos para las transformaciones afines */
	void set_m(GLfloat m[16]) {
		for(int i=0; i<16; i++)
			_m[i] = m[i];
	}

	/*
	 Traslada en función de la matriz
	*/
	void traslada(PV3D *v){
		glMatrixMode(GL_MODELVIEW); //empleamos esta, como podíamos haber usado otra
		glPushMatrix();
			glLoadIdentity();
			glTranslatef(v->get_x(), v->get_y(), v->get_z());
			GLfloat mm[16];
			//Almacena la matriz actual en la modelo-vista
			glGetFloatv(GL_MODELVIEW_MATRIX, mm);
		glPopMatrix();
		postMultiplica(mm);
	}

	/*
	 Rota en función de la matriz
	*/
	void rota(GLfloat angle, PV3D *v){
		glMatrixMode(GL_MODELVIEW); //empleamos esta, como podíamos haber usado otra
		glPushMatrix();
			glLoadIdentity();
			glRotatef(angle, v->get_x(), v->get_y(), v->get_z());
			GLfloat mm[16];
			//Almacena la matriz actual en la modelo-vista
			glGetFloatv(GL_MODELVIEW_MATRIX, mm);
		glPopMatrix();
		postMultiplica(mm);
	}


	/*
	 Escala en función de la matriz
	*/
	void escala(PV3D *v){
		glMatrixMode(GL_MODELVIEW); //empleamos esta, como podíamos haber usado otra
		glPushMatrix();
			glLoadIdentity();
			glScalef(v->get_x(), v->get_y(), v->get_z());
			GLfloat mm[16];
			//Almacena la matriz actual en la modelo-vista
			glGetFloatv(GL_MODELVIEW_MATRIX, mm);
		glPopMatrix();
		postMultiplica(mm);
	}

	/*
	Reinicia la Matriz Transformación
	*/
	void reiniciaMatriz() {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glGetFloatv(GL_MODELVIEW_MATRIX, _m);
		glPopMatrix();
	}

	/*
	PostMultiplica la matriz por una matriz dada
	*/
	void postMultiplica(GLfloat mm[16]){
		glMatrixMode(GL_MODELVIEW); //empleamos esta, como podíamos haber usado otra
		glPushMatrix();
			glLoadMatrixf(_m);
			glMultMatrixf(mm);
			glGetFloatv(GL_MODELVIEW_MATRIX, _m);
		glPopMatrix();
	}

}; //TAfin

#endif // __TAFIN_H