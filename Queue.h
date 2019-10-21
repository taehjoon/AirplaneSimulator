#pragma once
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <typeinfo>
#include <cstdlib>

using namespace std;

const int MAXWAITINGPLANES = 3;

template <class T>
class Queue {
public:
	Queue(int num = MAXWAITINGPLANES) :top(-1), capacity(num) {
		storage = new T[capacity];
	}
	void Push(const T& element) {
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
	bool IsEmpty()const {
		if (top == -1) return true;
		return false;
	};
	template <class T>
	friend ostream& operator<<(ostream& os, const Queue<T>& a);

	int getTop() const {
		return top;
	}
	T getElement(int num) {
		return storage[num];
	}
	void setElement(int num, T setValue) {
		storage[num] = setValue;
	}
private:
	int top;	//number of volume of sotrage
	int capacity;
	T* storage;
};

template<class T>
ostream& operator<<(ostream& os, const Queue<T>& a) {
	if (a.top == -1) os << "it has no element!";
	for (int i = 0; i <= a.top; i++)
		os << a.storage[i] << " ";
	os << endl;
	return os;
}