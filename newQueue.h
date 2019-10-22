#include "newBag.h"
#include <cstdlib>
#include <ctime>
#include <typeinfo>
#include <cstdlib>
const int MAXWAITINGPLANES = 3;

template <class T>
class Queue :public Bag<T> {
	using Bag<T>::capacity;
	using Bag<T>::top;
	using Bag<T>::storage;
public:
	Queue(int capacity = MAXWAITINGPLANES) :Bag<T>(capacity) {}
	//~Queue() {}
	T Element() const {
		return this->storage[0];
	}
	void Push(T& element) {
		if (top == capacity) throw "It can't put in the Queue";
		top = top + 1;
		storage[top] = element;
	}
	void Delete(int num) {
		if (top < 0) throw "This Queue is empty.";
		if (num <= top) {
			for (int i = num; i < top; i++) {
				storage[i] = storage[i + 1];
			}
			top--;
		}
	}
	template<class T>
	friend ostream& operator<<(ostream& os, const Queue<T>& q);

};

template<class T>
ostream& operator<<(ostream& os, const Queue<T>& q) {
	if (q.top == -1) os << "it has no element!";
	for (int i = 0; i <= q.top; i++)
		os << q.storage[i] << " ";
	os << endl;
	return os;
}
