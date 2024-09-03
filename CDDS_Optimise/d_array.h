#pragma once

constexpr auto ARRAY_BUFFER = 3;

template <class T>
class d_array
{
public:
	
	//Default size 1 empty array
	d_array() : d_array(1) {}
	//Create an empty array of given size
	d_array(int size) {
		data = new T[size+ARRAY_BUFFER];
		allocedElements = size;
		usedElements = 0;
	}
	//Creates an array of given size with one object 
	d_array(T& obj, int size) : d_array(size) {
		data[0] = obj;
		usedElements = 1;
	}
	//Copies an array
	d_array(d_array& copy) : d_array(copy.allocedElements + ARRAY_BUFFER){
		for (int it = 0; it < allocedElements; ++it) {
			data[it] = copy[it];
		}
		usedElements = copy.usedElements;
	}
	//destructor
	~d_array() {
		delete[] data;
	}

public:

	T& operator[](int index) {
		return data[index];
	}

	T* Begin() {
		return data[0];
	}

	T* End() {
		return data[allocedElements-1];
	}

	int Size() {
		return allocedElements;
	}

	void PushFront(T& object) {

		d_array<T> temp(*this);

		if (usedElements == allocedElements) {
			allocedElements+= ARRAY_BUFFER;
			data = new T[allocedElements];
		}

		data[0] = object;
		usedElements++;

		for (int i = 1; i < allocedElements; ++i) {
			data[i] = temp[i-1];
		}
	}
	void PushMiddle(T& object) {
		
		if (usedElements == allocedElements) {
			allocedElements += ARRAY_BUFFER;
		}

		T* temp = new T[allocedElements];

		int middle = (usedElements / 2);

		int j = 0;

		for (int i = 0; i < usedElements; ++i) {
			if (i == middle) {
				temp[i] = object;
				usedElements++;
			}
			else {
				temp[i] = data[j];
				j++;
			}
		}
		data = temp;
	}
	void PushBack(T& object) {
		
		d_array<T> temp(*this);
		
		if (usedElements == allocedElements) {
			allocedElements += ARRAY_BUFFER;
			data = new T[allocedElements];

			for (int i = 0; i < usedElements; ++i) {
				data[i] = temp[i];
			}
		}
		data[usedElements] = object;
		usedElements++;
	}

	void Remove(const T& d) {

		T* temp = new T[allocedElements];

		int j = 0;
		for (int i = 0; i < allocedElements; i++) {
			if (data[i] != d) { //If item is not the item to delete, adds it to the array
				temp[j] = data[i];
				j++;
				usedElements--;
			}
		}
		data = temp;
	}
	//Remove item at index
	void Remove(int index) {

		if (index > allocedElements) {
			return;
		}

		T* temp = new T[allocedElements];

		int j = 0;

		for (int i = 0; i < allocedElements; i++) {
			if (i != index) {
				temp[j] = data[i];
				j++;
			}
		}
		data = temp;
		usedElements--;
	}

	void Trim() {
		Trim(0);
	}
	void Trim(int buffer) {
		if (usedElements == 0 && buffer == 0) { //Avoids a zero length array
			buffer++;
		}

		//Temp container
		d_array<T> temp(*this);

		//Resize data
		data = new T[usedElements + buffer];
		
		//Add items back to data
		for (int i = 0; i < usedElements; ++i) {
			data[i] = temp[i];
		}
		//Set allocated elements to the new size
		allocedElements = usedElements + buffer;
	}
	
	void Clear() {
		data = new T[allocedElements];
		usedElements = 0;
	}


private:

	T* data;
	int allocedElements, usedElements;

};
