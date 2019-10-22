#pragma once
#include <iostream>
using namespace std;
template<class T>
class Bag {
protected:
	T* storage;
	int capacity;
	int top;
public:
	Bag(const int limit=3) :capacity(limit), top(-1) {
		storage = new T[capacity];
		/*capacity : num of input data10
		top : number of stored data0~10
		storage : collector*/
	}
	/*~Bag() {
		delete [] storage;
	}*/
	T Element() const {
		if (top == -1) throw "top is -1, storage has no element";
		return storage[top];
	}
	void Push(const T& element) {
		if (top == capacity-1) {
			capacity *= 2;
			T* newStorage = new T[capacity];
			delete[] storage;
			storage = newStorage;
		}
		storage[++top] = element;
		if (top != 0) {
			int randomindex = rand() % (top);
			T swap = storage[top];
			storage[top] = storage[randomindex];
			storage[randomindex] = swap;
		}
	}
	void Pop() {
		if (top == 0) throw "it has no value";
		storage[top--].~T();
	}
	int getTop() const {
		return top;
	}
	bool isEmpty() {
		if (top < 0) return true;
		return false;
	}
	T getElement(int index) const{
		return storage[index];
	}
	void setElement(int num, T setValue) {
		storage[num] = setValue;
	}
	template<class T>
	friend ostream& operator<<(ostream& os, const Bag<T>& b);
};
template<class T>
ostream& operator<<(ostream& os, const Bag<T>& b) {
	if (b.top == -1) os << "it has no element!";
	for (int i = 0; i <= b.top; i++)
		os << b.storage[i] << " ";
	os << endl;
	return os;
}