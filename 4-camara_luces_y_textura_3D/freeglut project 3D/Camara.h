/**
@author Lin M. Dotor
*/

#ifndef __CAMARA_H
#define __CAMARA_H

#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "PV3D.h"

/**
 Clase Camara, que almacena los diversos parámetros de la cámara y permite modificarla.
*/
class Camara{

private:
	// Camera parameters
	PV3D *_eye, *_look, *_up, *_u, *_v, *_n;

	// Viewing frustum parameters
	GLdouble _xRight, _xLeft, _yTop, _yBot, _near, _far;

	//Matriz Vista
	GLdouble _mV[16];

	bool _perspectiva; //indica si se muestra en vista perspectiva o ortogonal

public:
	/** Constructor */
	Camara() : _xRight(150.0), _xLeft(-_xRight), 
		_yTop(150.0), _yBot(-_yTop), 
		_near(1), _far(10000), 
		_perspectiva(false) 
	{
		_eye = new PV3D(500.0, 400.0, 400.0, true);
		_look = new PV3D(0.0, 0.0, 0.0, true);
		_up = new PV3D(0.0, 1.0, 0.0, true);
		_u = new PV3D();
		_v = new PV3D();
		_n = new PV3D();

		// Camera set up
		actualizarCAM();

		// Frustum set up
		actualizarAVE();
	}
	
	/** Destructor */
	~Camara() {
		delete _eye;
		delete _look;
		delete _up;
		delete _u;
		delete _v;
		delete _n;
	}

	/**	setters y getters */
	void set_xRight(GLdouble xR) {
		_xRight = xR;
	}
	
	void set_xLeft(GLdouble xL) {
		_xLeft = xL;
	}

	void set_yTop(GLdouble yT) {
		_yTop = yT;
	}

	void set_yBot(GLdouble yB) {
		_yBot = yB;
	}

	void set_near(GLdouble n) {
		_near = n;
	}

	void set_far(GLdouble f) {
		_far = f;
	}

	GLdouble get_xRight() {
		return _xRight;
	}
	
	GLdouble get_xLeft() {
		return _xLeft;
	}

	GLdouble get_yTop() {
		return _yTop;
	}

	GLdouble get_yBot() {
		return _yBot;
	}

	GLdouble get_near() {
		return _near;
	}

	GLdouble get_far() {
		return _far;
	}

	//----------CARGAR MATRIZ MODELO VISTA----------
	void setUVN(PV3D* eye, PV3D* look, PV3D* up)
	{
		_n->set(PV3D(eye->restarVect2Vect(look)).normalizarVect());
		_u->set(PV3D(up->productoVectorial(*_n)).normalizarVect());
		_v->set(PV3D(_n->productoVectorial(*_u)).normalizarVect());
	}

	//----------CARGAR MATRIZ MODELO VISTA----------
	void setMatrizModeloVista()
	{
		//carga la matriz con los valores existentes de la camara
		_mV[0] = _u->get_x();  _mV[4] = _u->get_y();  _mV[8] = _u->get_z();  _mV[12] = - _eye->productoEscalar(*_u);
		_mV[1] = _v->get_x();  _mV[5] = _v->get_y();  _mV[9] = _v->get_z();  _mV[13] = - _eye->productoEscalar(*_v);
		_mV[2] = _n->get_x();  _mV[6] = _n->get_y();  _mV[10] = _n->get_z(); _mV[14] = - _eye->productoEscalar(*_n);
		_mV[3] = 0.0;		   _mV[7] = 0.0;		  _mV[11] = 0.0;		 _mV[15] = 1.0;

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(_mV); //pone esa nueva matriz como la actual
	}


	//----------ACTUALIZAR MATRIZ AVE (PROYECCIÓN)----------
	void actualizarAVE(GLdouble xL, GLdouble xR, GLdouble yB, GLdouble yT, GLdouble n, GLdouble f) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(_perspectiva)	
			glFrustum(xL, xR, yB, yT, n, f); //vista perspectiva
			//gluPerspective(25, 1.0, n, f); //esta otra forma
		else
			glOrtho(xL, xR, yB, yT, n, f); //vista ortogonal
	}

	void actualizarAVE()
	{
		actualizarAVE(_xLeft, _xRight, _yBot, _yTop, _near, _far);
	}

	//----------ACTUALIZAR MATRIZ CAMARA----------
	void actualizarCAM(PV3D* eye, PV3D* look, PV3D* up) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();	
		gluLookAt(eye->get_x(), eye->get_y(), eye->get_z(), look->get_x(), look->get_y(), look->get_z(), up->get_x(), up->get_y(), up->get_z());
		setUVN(eye, look, up); //se actulizan los vectores u, v, n
	}

	void actualizarCAM()
	{
		actualizarCAM(_eye, _look, _up);
	}


	//------SET PERSPECTIVA----------
	void setPerspectiva(bool pers)
	{
		if(pers) //activar perspectiva
		{
			_perspectiva = true;
			_xRight=0.2;
			_xLeft=-_xRight;
			_yTop=0.2;
			_yBot=-_yTop;
			actualizarAVE();
		}
		else
		{
			_perspectiva = false;
			_xRight=150; 
			_xLeft=-_xRight;
			_yTop=150;
			_yBot=-_yTop;
			actualizarAVE();
		}		
	}


	//----------TRASLACIÓN PV----------
	void moverPV(GLdouble x, GLdouble y)
	{
		_xLeft += x; _xRight += x;
		_yTop += y;  _yBot += y;

		actualizarAVE();		
	}

	void moverPVIzquierda() { moverPV(-(_xRight - _xLeft)/10, 0.0); }
	void moverPVDerecha()	{ moverPV((_xRight - _xLeft)/10, 0.0);  }
	void moverPVArriba() { moverPV(0.0, (_yTop - _yBot)/10); }
	void moverPVAbajo() {	moverPV(0.0, -(_yTop - _yBot)/10); }

	

	//----------TRASLACIÓN CAM----------
	void trasladar(GLdouble x, GLdouble y, GLdouble z)
	{
		_eye->set(_eye->get_x() + x, _eye->get_y() + y, _eye->get_z() + z);

		actualizarCAM();		
	}


	//----------VISTAS-----------------
	void vistaLateral()
	{
		_eye->set(200.0, 0.0, 0.0);
		_up->set(0.0, 1.0, 0.0);

		actualizarCAM();
	}

	void vistaFrontal()
	{
		_eye->set(0.0, 0.0, 200.0);
		_up->set(0.0, 1.0, 0.0);

		actualizarCAM();
	}

	void vistaCenital()
	{
		_eye->set(0.0, 200.0, 0.0);
		_up->set(1.0, 0.0, 0.0);

		actualizarCAM();
	}

	void vistaEsquina()
	{
		_eye->set(500.0, 400.0, 400.0);
		_up->set(0.0, 1.0, 0.0);

		actualizarCAM();
	}
	
	//----------GIRO CAM----------
	typedef enum {X, Y, Z} Ejes;
	void girarEnEje(GLdouble avance_grados, int eje)
	{
		GLdouble PI = 3.141592653589793;
		GLdouble inc_angulo = (((2*PI)/360.0) * avance_grados);

		GLdouble radio; //radio al eje sobre el que gira
		GLdouble angulo; //angulo de la circunferencia sobre la que gira

		switch (eje) {
		case X: 
			if (_eye->get_y() != 0.0 || _eye->get_z() != 0.0)
			{
				radio = sqrt(pow(_eye->get_y(), 2.0) + pow(_eye->get_z(), 2.0));
				angulo = atan(_eye->get_z()/_eye->get_y());
				if(_eye->get_y()<0.0)
					angulo = angulo + PI;
				_eye->set_y(radio*cos(angulo+inc_angulo));
				_eye->set_z(radio*sin(angulo+inc_angulo));
			}
			break;

		case Y:  
			if(_eye->get_x() != 0.0 || _eye->get_z() != 0.0) //evit realizar calculos sin está sobre el eje (no hay giro)
			{
				radio = sqrt(pow(_eye->get_x(), 2.0) + pow(_eye->get_z(), 2.0));
				angulo = atan(_eye->get_z()/_eye->get_x());
				if(_eye->get_x()<0.0)
					angulo = angulo + PI;
				_eye->set_x(radio*cos(angulo+inc_angulo));
				_eye->set_z(radio*sin(angulo+inc_angulo));
			}
			break;

		case Z:  
			if(_eye->get_x() != 0.0 || _eye->get_y() != 0.0) //evit realizar calculos sin está sobre el eje (no hay giro)
			{
				radio = sqrt(pow(_eye->get_x(), 2.0) + pow(_eye->get_y(), 2.0));
				angulo = atan(_eye->get_y()/_eye->get_x());
				if(_eye->get_x()<0.0)
					angulo = angulo + PI;
				_eye->set_x(radio*cos(angulo+inc_angulo));
				_eye->set_y(radio*sin(angulo+inc_angulo));
			}
			break;
		}

		actualizarCAM();
	}


	//----------ROLL, YAW, PITCH----------
	// http://www.cs.uky.edu/~cheng/cs535/SamplePrograms/OpenGL/3DView_Transf/Camera.h
	void roll(GLdouble ang)
	{
		GLdouble PI = 3.141592653589793;
		GLdouble cs = cos(((2*PI)/360.0) * ang);
		GLdouble sn = sin(((2*PI)/360.0) * ang);

		PV3D u_aux = PV3D(_u);
		_u->set(cs*u_aux.get_x() - sn*_v->get_x(), cs*u_aux.get_y() - sn*_v->get_y(), cs*u_aux.get_z() - sn*_v->get_z() );
		_v->set(sn*u_aux.get_x() + cs*_v->get_x(), sn*u_aux.get_y() + cs*_v->get_y(), sn*u_aux.get_z() + cs*_v->get_z() );
		setMatrizModeloVista();
	}

	void yaw(GLdouble ang)
	{
		GLdouble PI = 3.141592653589793;
		GLdouble cs = cos(((2*PI)/360.0) * ang);
		GLdouble sn = sin(((2*PI)/360.0) * ang);

		PV3D u_aux = PV3D(_u);
		_u->set(cs*u_aux.get_x() + sn*_n->get_x(), cs*u_aux.get_y() + sn*_n->get_y(), cs*u_aux.get_z() + sn*_n->get_z() );
		_n->set(-sn*u_aux.get_x() + cs*_n->get_x(), -sn*u_aux.get_y() + cs*_n->get_y(), -sn*u_aux.get_z() + cs*_n->get_z() );
		setMatrizModeloVista();
	}

	void pitch(GLdouble ang)
	{
		GLdouble PI = 3.141592653589793;
		GLdouble cs = cos(((2*PI)/360.0) * ang);
		GLdouble sn = sin(((2*PI)/360.0) * ang);

		PV3D n_aux = PV3D(_n);
		_n->set(cs*n_aux.get_x() + sn*_v->get_x(), cs*n_aux.get_y() + sn*_v->get_y(), cs*n_aux.get_z() + sn*_v->get_z() );
		_v->set(-sn*n_aux.get_x() + cs*_v->get_x(), -sn*n_aux.get_y() + cs*_v->get_y(), -sn*n_aux.get_z() + cs*_v->get_z() );
		setMatrizModeloVista();
	}

	//----------ZOOM PV----------
	void zoom(GLdouble factorEscala)
	{	
		GLdouble centroX = (_xRight + _xLeft)/2;
		GLdouble centroY = (_yTop + _yBot)/2;

		GLdouble nuevoAncho = (_xRight - _xLeft)/factorEscala;
		GLdouble nuevoAlto = (_yTop - _yBot)/factorEscala;

		//aplicamos el zoom
		_xLeft = centroX - nuevoAncho/2;
		_xRight = centroX + nuevoAncho/2;
		_yBot = centroY - nuevoAlto/2;
		_yTop = centroY + nuevoAlto/2;

		actualizarAVE();
	}

	void zoomPercent(GLdouble percent)
	{
		zoom(percent/100);
	}




}; //Camara

#endif // __CAMARA_H