#ifndef ARISTA_H
#define ARISTA_H

template<class T, class O>
class Nodo;

template<class T, class O>
class Arista
{
	public:
		Arista(O peso,Nodo<T,O>* to)
		{
			this->peso=peso;
			this->to=to;
			this->from=0;
		}
		~Arista(){}
	public:

		Nodo<T,O>* to;
		O peso;
		Nodo<T,O>* from;
};

#endif
