/**
@author Jesús Martínez Dotor
*/

#ifndef __MUNECONIEVE_H
#define __MUNECONIEVE_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "ObjetoCompuesto3D.h"
#include "RGBColor.h"
#include "Sombrero.h"
#include "Disco.h"
#include "Esfera.h"
#include "Cilindro.h"
#include "Hexaedro.h"
#include "Escoba.h"
#include "Ojo.h"
#include "Nariz.h"

/**
 Clase MunecoNieve, que contiene varios objetos compuestos.
*/
class MunecoNieve : public ObjetoCompuesto3D{

private:
	PV3D _posicion;
	PV3D _rotacion_vector;
	GLfloat _rotacion_angulo;

public:
	/** Constructor */
	MunecoNieve() {
		_posicion = PV3D();
		_rotacion_vector = PV3D(0.0, 1.0, 0.0, false);
		_rotacion_angulo = 0.0;

		_num_objetos = 11;
		_objetos = new Objeto3D*[11];

		 //bola de abajo
		_objetos[0] = new Esfera(0.5, 30, 30, true, RGBColor(1.0, 1.0, 1.0), false, RGBColor(0.0, 0.0, 0.0));
		_objetos[0]->get_mT()->traslada(new PV3D(0.0, 50.0, 0.0, false));
		_objetos[0]->get_mT()->escala(new PV3D(100.0, 100.0, 100.0, false));		
		
		//bola de arriba
		_objetos[1] = new Esfera(0.5, 30, 30, true, RGBColor(1.0, 1.0, 1.0), false, RGBColor(0.0, 0.0, 0.0));
		_objetos[1]->set_color(RGBColor(1.0, 1.0, 1.0));
		_objetos[1]->set_pintar_lineas(false);
		_objetos[1]->get_mT()->traslada(new PV3D(0.0, 120.0, 0.0, false));
		_objetos[1]->get_mT()->escala(new PV3D(60.0, 60.0, 60.0, false));
		
		//sombrero
		_objetos[2] = new Sombrero();
		_objetos[2]->get_mT()->traslada(new PV3D(0.0, 146.5, 0.0, false));
		_objetos[2]->get_mT()->rota(90.0, new PV3D(-1.0, 0.0, 0.0, false));

		//nariz
		_objetos[3] = new Nariz(1);
		_objetos[3]->get_mT()->traslada(new PV3D(20.0, 120.0, 0.0, false));
		_objetos[3]->get_mT()->escala(new PV3D(15.0, 15.0, 15.0, false));
		_objetos[3]->get_mT()->rota(90.0, new PV3D(0.0, 1.0, 0.0, false));		

		//ojoIz
		_objetos[4] = new Ojo(GL_LIGHT1); 
		_objetos[4]->get_mT()->traslada(new PV3D(28.0, 130.0, -13.0, false));
		_objetos[4]->get_mT()->escala(new PV3D(15.0, 15.0, 15.0, false));
		_objetos[4]->get_mT()->rota(100.0, new PV3D(0.0, 1.0, 0.0, false));
		
		//ojoDr
		_objetos[5] = new Ojo(GL_LIGHT2);  
		_objetos[5]->get_mT()->traslada(new PV3D(28.0, 130.0, 13.0, false));
		_objetos[5]->get_mT()->escala(new PV3D(15.0, 15.0, 15.0, false));
		_objetos[5]->get_mT()->rota(80.0, new PV3D(0.0, 1.0, 0.0, false));
		
		//boton1
		_objetos[6] = new Disco(0.1, 0.5, 15, 1, true, RGBColor(1.0, 0.0, 0.0), false, RGBColor(0.0, 0.0, 0.0)); 
		_objetos[6]->get_mT()->traslada(new PV3D(46.0, 70.0, 0.0, false));
		_objetos[6]->get_mT()->escala(new PV3D(15.0, 15.0, 15.0, false));
		_objetos[6]->get_mT()->rota(90.0, new PV3D(-0.35, 1.0, 0.0, false));

		//boton2
		_objetos[7] = new Disco(0.1, 0.5, 15, 1, true, RGBColor(1.0, 0.0, 0.0), false, RGBColor(0.0, 0.0, 0.0)); 
		_objetos[7]->get_mT()->traslada(new PV3D(50.0, 50.0, 0.0, false));
		_objetos[7]->get_mT()->escala(new PV3D(15.0, 15.0, 15.0, false));
		_objetos[7]->get_mT()->rota(90.0, new PV3D(0.0, 1.0, 0.0, false));

		//boton3
		_objetos[8] = new Disco(0.1, 0.5, 15, 1, true, RGBColor(1.0, 0.0, 0.0), false, RGBColor(0.0, 0.0, 0.0)); 
		_objetos[8]->get_mT()->traslada(new PV3D(46.0, 30.0, 0.0, false));
		_objetos[8]->get_mT()->escala(new PV3D(15.0, 15.0, 15.0, false));
		_objetos[8]->get_mT()->rota(90.0, new PV3D(0.35, 1.0, 0.0, false));

		//escoba
		_objetos[9] = new Escoba(); 
		_objetos[9]->get_mT()->traslada(new PV3D(35.0, 150.0, 50.0, false));
		_objetos[9]->get_mT()->escala(new PV3D(0.8, 0.8, 0.8, false));
		_objetos[9]->get_mT()->rota(90.0, new PV3D(0.05, 0.05, -1.0, false));

		//mano
		_objetos[10] = new Esfera(0.5, 30, 30, true, RGBColor(1.0, 1.0, 1.0), false, RGBColor(0.0, 0.0, 0.0));
		_objetos[10]->get_mT()->traslada(new PV3D(30.0, 70.0, 45.0, false));
		_objetos[10]->get_mT()->escala(new PV3D(25.0, 25.0, 25.0, false));
	}

	/** Destructor */
	~MunecoNieve() {
	}

	PV3D get_posicion() {
		return _posicion;
	}

	void set_posicion(PV3D posicion) {
		_posicion = posicion;
	}

	PV3D get_rotacion_vector() {
		return _rotacion_vector;
	}

	void set_rotacion_vector(PV3D rotacion) {
		_rotacion_vector = rotacion;
	}

	GLfloat get_rotacion_angulo() {
		return _rotacion_angulo;
	}

	void set_rotacion_angulo(GLfloat angulo) {
		_rotacion_angulo = angulo;
	}

	/*
	Avanza el muñeco según la dirección a la que mira
	*/
	void avanza(GLdouble mov) {
		GLdouble ang = get_rotacion_angulo();
		GLdouble PI = 3.141592653589793;
		GLdouble cs = cos(((2*PI)/360.0) * ang);
		GLdouble sn = sin(((2*PI)/360.0) * ang);
		set_posicion(get_posicion().sumarVect2Pto(PV3D(cs, 0.0, -sn, true).multiplicarVect2Escalar(mov)));

		_objetos[9]->get_mT()->rota(mov, new PV3D(1.0, 0.0, 0.0, false));	
	}
	/*
	Sobrecarga del draw para que se pueda mover el muñeco
	*/
	void draw() {
		get_mT()->reiniciaMatriz();
		get_mT()->traslada(&_posicion);
		get_mT()->rota(_rotacion_angulo, &_rotacion_vector);
		glPushMatrix();			
			glMultMatrixf(get_mT()->get_m());
			ObjetoCompuesto3D::draw();
		glPopMatrix();
	}

}; //MunecoNieve

#endif // __MUNECONIEVE_H