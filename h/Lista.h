/*
 * Lista.h
 *
 *  Created on: Apr 27, 2020
 *      Author: OS1
 */

#ifndef LISTA_H_
#define LISTA_H_
#include <iostream.h>
#include "usable.h"

template <class T>
class List {
public:


	struct Elem {
		struct Elem *next;
		T data;
		Elem(const T& p, Elem *sl1 = nullptr) :data(p), next(sl1) {}
	};
	Elem *first;
	Elem *last;
	Elem *current;
	Elem *before_current;

	int n;

	void move(List& l1)
	{
		first = l1.first;
			last = l1.last;
			current = before_current = nullptr;
	}
	void copy(const List& l1)
	{
		first = last = current = before_current = nullptr;
		for (Elem* pok = l1.first; pok; pok = pok->next)
		{
			Elem* novi = new Elem(pok->data);
			last = (!first ? first : last->next) = novi;
			if (pok == l1.current) current = novi;
			if (pok == l1.before_current) before_current = novi;
		}
	}
	void delete_list()
	{
		Elem *t = first;
			while (first)
			{
				t = first->next;
				delete first;
				first = t;
			}
			n=0;
	};
	List() { first = last = before_current = current = nullptr; n=0; }

	List& addEnd(const T& t)
	{
		last = (!first ? first : last->next) = new Elem(t);
		n++;
		return *this;
		// TODO: insert return statement here
	}

	List& addBegin(const T& t)
		{
			if(first==nullptr){
				last=first=new Elem(t);
			}
			else
			{
			Elem* temp=new Elem(t);
			temp->next=first;
			first=temp;
			}
			n++;
			return *this;
			// TODO: insert return statement here
		}

	int currentExists() const
	{
		if (current != nullptr) return 1;
		else
			return 0;
	}

	List& onFirst()
	{
				current = first;
					before_current = nullptr;
			return *this;
	}
//	const Lista& naPrvi() const
//	{
//		tekuci = prvi;
//		prethodni = nullptr;
//		return *this;
//		// TODO: insert return statement here
//	}
	List& onNext()
	{
		before_current = current;
		if (current) current = current->next;
		return *this;
		// TODO: insert return statement here
	}
//	const Lista& naSled() const
//	{
//		prethodni = tekuci;
//			if (tekuci) tekuci = tekuci->sl;
//			return *this;
//			// TODO: insert return statement here
//	}
	T& getCurrentNode()
	{
		if (!currentExists()) cout<<"ne postoji tekuci";
			return current->data;
			// TODO: insert return statement here
	}
//	const T& dohvatiTek() const
//	{
//		if (!currentExists()) cout<<"ne postoji tekuci";
//			return current->data;
//			// TODO: insert return statement here
//	}
	List& deleteCurrNode()
	{
		if (!current) cout <<"ne postoji tekuci";
		else{
		Elem *stari = current;
		current = current->next;
		(!before_current ? first : before_current->next) = current;
		if (!current) last = before_current;
		delete stari;
		n--;
		return *this;
		}
		// TODO: insert return statement here
	}

	int list_size() {return n;};
	int list_is_empty() {return n==0;};

	friend ostream& operator<<(ostream& os, const List& l1)
	{

		for (Elem *tek = l1.first; tek; tek = tek->next)
		{
			os << tek->data << endl;
		}

		return os;

	}

};


#endif /* BCC_LISTA_H_ */
