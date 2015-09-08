/**
  @file Lista.h

  Implementación del TAD lista, utilizando una 
  lista doblemente enlazada.

  Estructura de Datos y Algoritmos
  Facultad de Informática
  Universidad Complutense de Madrid

 (c) Marco Antonio Gómez Martín, 2012
*/
#ifndef __LISTA_H
#define __LISTA_H

#include "Excepciones.h"
#include <cassert>

/**
 Implementación del TAD Pila utilizando vectores dinámicos.

 Las operaciones son:

 - ListaVacia: -> Lista. Generadora implementada en el
   constructor sin parámetros.
 - Cons: Lista, Elem -> Lista. Generadora.
 - ponDr: Lista, Elem -> Lista. Modificadora.
 - primero: Lista - -> Elem. Observadora parcial
 - resto: Lista - -> Lista. Modificadora parcial
 - ultimo: Lista - -> Elem. Observadora parcial
 - inicio: Lista - -> Lista. Modificadora parcial
 - esVacia: Lista -> Bool. Observadora
 - numElems: Lista -> Elem. Obervadora.
 - elem: Lista, Entero - -> Elem. Observador parcial.

 @author Marco Antonio Gómez Martín
 */
template <class T>
class Lista {
private:
	/**
	 Clase nodo que almacena internamente el elemento (de tipo T),
	 y dos punteros, uno al nodo anterior y otro al nodo siguiente.
	 Ambos punteros podrían ser NULL si el nodo es el primero
	 y/o último de la lista enlazada.
	 */
	class Nodo {
	public:
		Nodo() : _sig(NULL), _ant(NULL) {}
		Nodo(T &elem) : _elem(elem), _sig(NULL), _ant(NULL) {}
		Nodo(Nodo *ant, T &elem, Nodo *sig) : 
		    _elem(elem), _sig(sig), _ant(ant) {}

		T _elem;
		Nodo *_sig;
		Nodo *_ant;
	};

public:

	/** Constructor; operación ListaVacia. */
	Lista() : _prim(NULL), _ult(NULL), _numElems(0) {}

	/** Destructor; elimina la lista doblemente enlazada. */
	~Lista() {
		libera();
	}

	/**
	 Añade un nuevo elemento en la cabeza de la lista.
	 Operación generadora.

	 @param elem Elemento que se añade en la cabecera de
	 la lista.
	*/
	void Cons(T &elem) {
		_numElems++;
		_prim = insertaElem(elem, NULL, _prim);
		if (_ult == NULL)
			_ult = _prim;
	}

	/**
	 Añade un nuevo elemento al final de la lista (a la 
	 "derecha"). Operación modificadora.

	 ponDr(e, ListaVacia) = Cons(e, ListaVacia)
	 ponDr(e, Cons(x, xs)) = Cons(x, ponDr(e, xs))
	*/
	void ponDr(T &elem) {
		_numElems++;
		_ult = insertaElem(elem, _ult, NULL);
		if (_prim == NULL)
			_prim = _ult;
	}

	/**
	 Devuelve el valor almacenado en la cabecera de la
	 lista. Es un error preguntar por el primero de
	 una lista vacía.

	 primero(Cons(x, xs)) = x
	 error primero(ListaVacia)

	 @return Elemento en la cabecera de la lista.
	 */
	T &primero() {
		if (esVacia())
			throw EListaVacia();
		return _prim->_elem;
	}

	/**
	 Devuelve el valor almacenado en la última posición
	 de la lista (a la derecha).
	 Es un error preguntar por el primero de una lista vacía.

	 ultimo(Cons(x, xs)) = x           SI esVacia(xs)
	 ultimo(Cons(x, xs)) = ultimo(xs)  SI !esVacia(xs)
	 error ultimo(ListaVacia)

	 @return Elemento en la cola de la lista.
	 */
	T &ultimo() {
		if (esVacia())
			throw EListaVacia();

		return _ult->_elem;
	}

	/**
	 Elimina el primer elemento de la lista.
	 Es un error intentar obtener el resto de una lista vacía.

	 resto(Cons(x, xs)) = xs
	 error resto(ListaVacia)
	*/
	void resto() {
		if (esVacia())
			throw EListaVacia();

		Nodo *aBorrar = _prim;
		_prim = _prim->_sig;
		borraElem(aBorrar);
		if (_prim == NULL)
			_ult = NULL;
		--_numElems;
	}

	/**
	 Elimina el último elemento de la lista.
	 Es un error intentar obtener el inicio de una lista vacía.

	 inicio(Cons(x, ListaVacia)) = ListaVacia
	 inicio(Cons(x, xs)) = Cons(x, inicio(xs)) SI !esVacia(xs)
	 error inicio(ListaVacia)
	*/
	void inicio() {
		if (esVacia())
			throw EListaVacia();

		Nodo *aBorrar = _ult;
		_ult = _ult->_ant;
		borraElem(aBorrar);
		if (_ult == NULL)
			_prim = NULL;
		--_numElems;
	}

	/**
	 Operación observadora para saber si una lista
	 tiene o no elementos.

	 esVacia(ListaVacia) = true
	 esVacia(Cons(x, xs)) = false

	 @return true si la lista no tiene elementos.
	 */
	bool esVacia() {
		return _prim == NULL;
	}

	/**
	 Devuelve el número de elementos que hay en la
	 lista.
	 numElems(ListaVacia) = 0
	 numElems(Cons(x, xs)) = 1 + numElems(xs)

	 @return Número de elementos.
	 */
	unsigned int numElems() {
		return _numElems;
	}

	/**
	 Devuelve el elemento i-ésimo de la lista, teniendo
	 en cuenta que el primer elemento (primero())
	 es el elemento 0 y el último es numElems()-1,
	 es decir idx está en [0..numElems()-1].
	 Operación observadora parcial que puede fallar
	 si se da un índice incorrecto. El índice es
	 entero sin signo, para evitar que se puedan
	 pedir elementos negativos.

	 elem(0, Cons(x, xs)) = x
	 elem(n, Cons(x, xs)) = elem(n-1, xs) si n > 0
	 error elem(n, xs) si !( 0 <= n < numElems(xs) )
	*/
	T &elem(unsigned int idx) {
		if (idx >= _numElems)
			throw EAccesoInvalido();

		Nodo *aux = _prim;
		for (int i = 0; i < idx; ++i)
			aux = aux->_sig;

		return aux->_elem;
	}

	/**
	 Clase interna que implementa un iterador sobre
	 la lista que permite recorrer la lista e incluso
	 alterar el valor de sus elementos.
	 */
	class Iterador {
	public:
		void avanza() {
			if (_act == NULL) throw EAccesoInvalido();
			_act = _act->_sig;
		}

		T &elem() {
			if (_act == NULL) throw EAccesoInvalido();
			return _act->_elem;
		}

		void pon(T &elem) {
			if (_act == NULL) throw EAccesoInvalido();
			_act->_elem = elem;
		}

		bool operator==(Iterador &other) {
			return _act == other._act;
		}

		bool operator!=(Iterador &other) {
			return !(this->operator==(other));
		}
	protected:
		// Para que pueda construir objetos del
		// tipo iterador
		friend class Lista;

		Iterador() : _act(NULL) {}
		Iterador(Nodo *act) : _act(act) {}

		// Puntero al nodo actual del recorrido
		Nodo *_act;
	};
	
	/**
	 Devuelve el iterador al principio de la lista.
	 @return iterador al principio de la lista;
	 coincidirá con final() si la lista está vacía.
	 */
	Iterador principio() {
		return Iterador(_prim);
	}

	/**
	 @return Devuelve un iterador al final del recorrido
	 (fuera de éste).
	 */
	Iterador final() {
		return Iterador(NULL);
	}

	/**
	 Permite eliminar de la lista el elemento
	 apuntado por el iterador que se pasa como parámetro.
	 El iterador recibido DEJA DE SER VÁLIDO. En su
	 lugar, deberá utilizarse el iterador devuelto, que
	 apuntará al siguiente elemento al borrado.
	 @param it Iterador colocado en el elemento que se
	 quiere borrar.
	 @return Nuevo iterador colocado en el elemento siguiente
	 al borrado (podría coincidir con final() si el
	 elemento que se borró era el último de la lista).
	 */
	Iterador borra(Iterador &it) {
		if (it._act == NULL)
			throw EAccesoInvalido();

		// Cubrimos los casos especiales donde
		// borramos alguno de los extremos
		if (it._act == _prim) {
			resto();
			return Iterador(_prim);
		} else if (it._act == _ult) {
			inicio();
			return Iterador(NULL);
		} else {
			// El elemento a borrar es interno a la lista.
			--_numElems;
			Nodo *sig = it._act->_sig;
			borraElem(it._act);
			return Iterador(sig);
		}
	}

	/**
	 Método para insertar un elemento en la lista
	 en el punto marcado por el iterador. En concreto,
	 se añade _justo antes_ que el elemento actual. Es
	 decir, si it==l.primero(), el elemento insertado se
	 convierte en el primer elemento (y el iterador
	 apuntará al segundo). Si it==l.final(), el elemento
	 insertado será el último (e it seguirá apuntando
	 fuera del recorrido).
	 @param elem Valor del elemento a insertar.
	 @param it Punto en el que insertar el elemento.
	 */
	void insertar(T &elem, Iterador &it) {

		// Caso especial: ¿añadir al principio?
		if (_prim == it._act) {
			Cons(elem);
		} else
		// Caso especial: ¿añadir al final?
		if (it._act == NULL) {
			ponDr(elem);
		}
		// Caso normal
		else {
			insertaElem(elem, it._act->_ant, it._act);
		}
	}

	// //
	// MÉTODOS DE "FONTANERÍA" DE C++ QUE HACEN VERSÁTIL
	// A LA CLASE
	// //

	/** Constructor copia */
	Lista(Lista<T> &other) : _prim(NULL), _ult(NULL) {
		copia(other);
	}

	/** Operador de asignación */
	Lista<T> &operator=(Lista<T> &other) {
		if (this != &other) {
			libera();
			copia(other);
		}
		return *this;
	}

	/** Operador de comparación. */
	bool operator==(Lista<T> &rhs) {
		if (_numElems != rhs._numElems)
			return false;
		Nodo *p1 = _prim;
		Nodo *p2 = rhs._prim;
		while ((p1 != NULL) && (p2 != NULL)) {
			if (p1->_elem != p2->_elem)
				return false;
			p1 = p1->_sig;
			p2 = p2->_sig;
		}

		return (p1 == NULL) && (p2 == NULL);
	}

	bool operator!=(Lista<T> &rhs) {
		return !(*this == rhs);
	}


protected:

	void libera() {
		libera(_prim);
		_prim = NULL;
		_ult = NULL;
	}

	void copia(Lista<T> &other) {
		// En vez de trabajar con punteros en la inserción,
		// usamos ponDr
		_prim = 0;
		_numElems = 0;

		Nodo *act = other._prim;
		while (act != NULL) {
			ponDr(act->_elem);
			act = act->_sig;
		}
	}

private:


	/**
	 Inserta un elemento entre el nodo1 y el nodo2.
	 Devuelve el puntero al nodo creado.
	 Caso general: los dos nodos existen.
	    nodo1->_sig == nodo2
	    nodo2->_ant == nodo1
	 Casos especiales: alguno de los nodos no existe
	    nodo1 == NULL y/o nodo2 == NULL
	*/
	static Nodo *insertaElem(T &e, Nodo *nodo1, Nodo *nodo2) {
		Nodo *nuevo = new Nodo(nodo1, e, nodo2);
		if (nodo1 != NULL)
			nodo1->_sig = nuevo;
		if (nodo2 != NULL)
			nodo2->_ant = nuevo;
		return nuevo;
	}

	/**
	 Elimina el nodo n. Si el nodo tiene nodos antes
	 o después, actualiza sus punteros anterior y siguiente.
	 Caso general: hay nodos anterior y siguiente.
	 Casos especiales: algunos de los nodos (anterior o siguiente
	 a n) no existen.
	*/
	static void borraElem(Nodo *n) {
		assert(n != NULL);
		Nodo *ant = n->_ant;
		Nodo *sig = n->_sig;
		if (ant != NULL)
			ant->_sig = sig;
		if (sig != NULL)
			sig->_ant = ant;
		delete n;
	}

	/**
	 Elimina todos los nodos de la lista enlazada cuyo
	 primer nodo se pasa como parámetro.
	 Se admite que el nodo sea NULL (no habrá nada que
	 liberar). En caso de pasarse un nodo válido,
	 su puntero al nodo anterior debe ser NULL (si no,
	 no sería el primero de la lista!).
	 */
	static void libera(Nodo *prim) {
		assert(!prim || !prim->_ant);

		while (prim != NULL) {
			Nodo *aux = prim;
			prim = prim->_sig;
			delete aux;
		}
	}

	// Puntero al primer y último elemento
	Nodo *_prim, *_ult;

	// Número de elementos (número de nodos entre _prim y _ult)
	unsigned int _numElems;
};

#endif // __LISTA_ENLAZADA_H
