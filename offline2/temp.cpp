#include <iostream>

class MyArray {
private:
    int data[5]; // Sample data for demonstration

public:
    // Overload the index operator
    int& operator[](int index) {
        if (index >= 0 && index < 5) {
            return data[index];
        } else {
            // Handle out-of-bounds access
            std::cerr << "Index out of bounds!" << std::endl;
            // You could throw an exception or handle it in a different way
            // For simplicity, we return the first element in this case
            return data[0];
        }
    }
};

int main() {
    MyArray myArray;

    // Assign values using the overloaded index operator
    for (int i = 0; i < 5; ++i) {
        myArray[i] = i * 2;
    }

    // Access and print values using the overloaded index operator
    for (int i = 0; i < 6; ++i) {
        std::cout << "myArray[" << i << "] = " << myArray[i] << std::endl;
    }

    return 0;
}
