/**
@author Jesús Martínez Dotor
*/

#ifndef __RGBCOLOR_H
#define __RGBCOLOR_H

#include <GL/freeglut.h>

/**
 Clase RGBColor, que los 3 componentes de un color.
 
 Operaciones: 
	- changeColor: RGBColor ->
	
@author Jesús Martínez Dotor
*/
class RGBColor {

private:
	// Componentes
	GLfloat _red;
	GLfloat _green;
	GLfloat _blue;

public:
	/** Constructores */
	RGBColor() : _red(0.0), _green(0.0), _blue(0.0) {}
	RGBColor(GLfloat red, GLfloat green, GLfloat blue) : _red(red), _green(green), _blue(blue) {}
	RGBColor(const RGBColor &c) : _red(c._red), _green(c._green), _blue(c._blue) {}

	/** Destructor */
	~RGBColor() {
	}

	/**	setters y getters */
	GLfloat get_red() {
		return _red;
	}

	GLfloat get_green() {
		return _green;
	}

	GLfloat get_blue() {
		return _blue;
	}

	/**
	 Cambia el color empleado en la escena, por el propio
	*/
	void changeColor() {
		glColor3f(_red,_green,_blue); 
	}

	/**
	 Modifica cierta cantidad (porcentaje) el color original para que se parezca al pasado por parámetro.
	 Es como una "media" entre los 2 colores
	 */
	void transicion(RGBColor c2, GLfloat cant)	{
		_red = _red + ((c2.get_red() - _red)*cant);
		_green = _green + ((c2.get_green() - _green)*cant);
		_blue = _blue + ((c2.get_blue() - _blue)*cant);
	}

}; //Color

#endif // __RGBCOLOR_H