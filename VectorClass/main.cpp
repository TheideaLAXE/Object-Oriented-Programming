#include "MyVec.cpp"
#include <iostream>

int main() {
    MyVec v;
    v.print();
    for (int i=0; i<10; i++) {
        v.push(100-i);
    }
    v.quicksort(1);
    v.print();
    v.quicksort();
    v.print();
    std::cout << "\n\n";
    int *i = v.data();
    std::cout << *i << "\n";
    std::cout << i[0] << "\n";
    std::cout << v.at(0) << "\n";
    v.at(0)=13;
    std::cout << v.at(0) << "\n";

    MyVec v1, v2;
    for (int i=0; i<9; i++) v1.push(i);
    for (int i=0; i<9; i++) v2.push(i+17);
    MyVec v3 = v1+v2;
    v3.print();

    std::cout << "test1\n";
    std::cout << v.at(100) << "\n";
    std::cout << "test2\n";

}

