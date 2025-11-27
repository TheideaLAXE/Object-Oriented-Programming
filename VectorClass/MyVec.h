#ifndef MYVEC_H
#define MYVEC_H

class MyVec {
    int size;
    int capacity;
    int *elements;
    void _copy(const MyVec& orig);
public:
    MyVec(int size = 0, int value = 0);
    ~MyVec();
    MyVec(const MyVec& orig);
    MyVec& operator=(const MyVec& orig);
    int operator[](int i);
    int &front();

    MyVec operator+(MyVec& v2);
    int &back();
    void quicksort(bool Rev=false, int l=0, int r=-1);
    ///Rev is for if reversed, aka true for from smallest to biggest
    int &at(int pos);
    int *data();

    void print();
    void push(int val);
    void pop();
};

#endif //MYVEC_H