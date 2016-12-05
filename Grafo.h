#ifndef GRAFO_H
#define GRAFO_H

#include <algorithm>
#include <queue>
#include "Nodo.h"

template<class T,class O>
void imprimir(std::list<Arista<T,O>* > lista)
{
	std::cout <<":::"<<std::endl;
	for(typename std::list<Arista<T,O>* >::iterator it=lista.begin();it!=lista.end();it++)
	{
		std::cout<<"\t Peso: "<<(*it)->peso<<" Para: "<<(*it)->to->valor<< std::endl;
	}
	std::cout<<std::endl;

}


template<class T, class O>
class Grafo
{
	typedef typename std::list<Nodo<T,O>* >::iterator iterador;
	typedef typename std::list<Arista<T,O>* >::iterator aristait;
	public:
		Grafo(){}
		void insertarNodo(T);
        void insertarVertice(Nodo<T,O>* p);
		void mostrarNodos();
		void insertarArista(Nodo<T,O>*&,Nodo<T,O>*,O);
		void insertarArista(T, T,O);
		bool buscarNodo(T);
		~Grafo(){}
	public:
		std::list<Nodo<T,O>* > nodos;
    private:
        int INF=9999999;
};

template<typename T> void print_queue(T q) {
    while(!q.empty()) {
        std::cout << (q.top())->valor << ":"<<(q.top())->key<<" ";
        q.pop();
    }
    std::cout << '\n';
}


template<class T, class O>
void Grafo<T,O>::insertarArista(T from, T to, O peso)
{
	Nodo<T,O>** noditos=new Nodo<T,O>*[2];
	noditos[0]=0;noditos[1]=0;

	for(iterador it=this->nodos.begin();it!=this->nodos.end();it++)
	{
		if((*it)->valor==from)
			noditos[0]=*it;
		if((*it)->valor==to)
			noditos[1]=*it;
	}
	if (noditos[0]==0 || noditos[1]==0)
		return ;
	insertarArista(noditos[0],noditos[1],peso);
}


//no direccionado
template<class T, class O>
void Grafo<T,O>::insertarArista(Nodo<T,O>*& from, Nodo<T,O>* to, O peso)
{
	from->aristas.push_back(new Arista<T,O>(peso,to));
	to->aristas.push_back(new Arista<T,O>(peso,from));
}

template<class T, class O>
void Grafo<T,O>::mostrarNodos()
{
	std::cout<<"Nodos:  "<<std::endl;;
	for(iterador it=this->nodos.begin();it!=nodos.end();it++)
	{
		std::cout<< (*it)->valor;
		imprimir((*it)->aristas);
	}
	std::cout<<std::endl;
}

template<class T,class O>
bool Grafo<T,O>::buscarNodo(T nodo)
{
	for(iterador it=this->nodos.begin();it!=nodos.end();it++)
	{
		if ((*it)->valor==nodo)
			return true;
	}
	return false;
}

template<class T, class O>
void Grafo<T,O>::insertarNodo(T nuevo)
{
	if (buscarNodo(nuevo))
		return;
	this->nodos.push_back(new Nodo<T,O>(nuevo));
}

template<class T, class O>
void Grafo<T,O>::insertarVertice(Nodo<T,O>* p)
{
    if (buscarNodo(p->valor))
        return;
    this->nodos.push_back(p);
}


#endif 
