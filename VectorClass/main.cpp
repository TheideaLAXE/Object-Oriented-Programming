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
    std::cout << "\n\n";
    std::cout << v.at(100);
}

