#ifndef NODO_H
#define NODO_H

#include "Arista.h"
#include <list>


template<class T, class O>
class Nodo
{
	public:
		Nodo(T valor){
			this->valor=valor;
		}
		~Nodo(){}
        bool operator<(const Nodo<T,O>& y)
        {
            return this->key < y.key;
        }
        bool operator>(const Nodo<T,O>& y)
        {
            return this->key > y.key;
        }
	public:
		T valor;
        //Los siguientes 3 campos son para prim
        int key;
        Nodo<T,O>* parent;
        bool agregado;

        std::list<Arista<T,O>* > aristas; //Adyancensias
};



#endif
