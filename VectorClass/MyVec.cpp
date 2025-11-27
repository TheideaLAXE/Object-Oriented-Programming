#include "MyVec.h"
#include <iostream>

MyVec::MyVec(const int size, const int value) {
    int capacity = 4;
    while (size >= capacity) {
        capacity*=2;
    }
    elements = new int[capacity];
    this->capacity = capacity;
    this->size = size;

    for (int i=0; i<this->size;i++ ) {
        elements[i] = value;
    }
    //std::cout<<"Constructing finished";
}

MyVec::~MyVec() {
    delete [] elements;
    elements = nullptr;
    //std::cout<<"Destruction Finished\n";
}

void MyVec::_copy(const MyVec& orig) {
    this->capacity = orig.capacity;
    this->size = orig.size;
    this->elements = new int[this->capacity];
    for (int i=0; i<this->size; i++) {
        this->elements[i] = orig.elements[i];
    }
}

MyVec::MyVec(const MyVec& orig) {
    _copy(orig);
}
MyVec& MyVec::operator=(const MyVec& orig) {
    if (this != &orig) {
        delete [] this->elements;
        _copy(orig);
    }
    return *this;
}

void MyVec::print() {
    std::cout<<"C: "<< this->capacity << " S: " << this->size <<":\n";
    for (int i=0; i<this->size; i++) {
        std::cout<<this->elements[i] << " ";
    }
    std::cout<<std::endl;
}

void MyVec::push(int val) {
    if (this->size == this->capacity) {
        int newcap = this->capacity*2;
        int *tmp = new int[newcap];
        for (int i=0; i<this->size; i++) {
            tmp[i] = this->elements[i];
        }
        delete [] elements;
        elements = tmp;
        this->capacity = newcap;
    }

    this->elements[this->size] = val;
    this->size++;
}

void MyVec::pop() {
    if (size < 1) return;

    size--;
    if (capacity>15 && capacity/2 > size) {
        const int newcap = capacity/2;
        int *tmp = new int[newcap];
        for (int i=0; i<size; i++) {
            tmp[i] = elements[i];
        }
        delete [] elements;
        elements = tmp;
        capacity = newcap;
    }
}

int MyVec::operator[](const int i) {
    if (i<0 || i > this->size-1) {
        return 42;
    }
    return this->elements[i];
}

///HOMEWORK

int &MyVec::back() {
    if (size<1) throw std::out_of_range("Empty vector\n");
        return this->elements[size-1];
}

int &MyVec::front() {
    if (size<1) throw std::out_of_range("Empty vector\n");
        return this->elements[0];
}

int &MyVec::at(const int pos) { //same with operator[]
    if (size<1) throw std::out_of_range("Empty vector\n");
    if (pos>=size || pos<0) throw std::out_of_range("Index not in vector\n");
    return elements[pos];
}

MyVec MyVec::operator+(MyVec &v2) {
    if (this->size > v2.size) {
        MyVec v(this->size, 0);
        for (int i=0; i<v2.size; i++)           v.at(i)=this->at(i)+v2.at(i);
        for (int i=v2.size; i<this->size; i++)  v.at(i)=this->at(i);
        return v;
    }
    MyVec v(v2.size, 0);
    for (int i=0; i<this->size; i++)        v.at(i)=this->at(i)+v2.at(i);
    for (int i=this->size; i<v2.size; i++)  v.at(i)=v2.at(i);
    return v;
}

int *MyVec::data() {
    return this->elements;
}

void MyVec::quicksort(const bool Rev, int l, int r) {
    if (r == -1) r = this->size - 1;
    if (l >= r) return;

    int i = l, j = r;
    const int p = this->elements[(l+r)/2];

    while (i <= j) {
        if (!Rev) {
            while (this->at(i)>p) i++;
            while (this->at(j)<p) j--;
        }
        else {
            while (this->at(i)<p) i++;
            while (this->at(j)>p) j--;
        }

        if (i<=j) {
            const int t=this->at(i);
            this->at(i)=this->at(j);
            this->at(j)=t;
            i++;
            j--;
        }
    }
    quicksort(Rev, l, j);
    quicksort(Rev, i, r);
}