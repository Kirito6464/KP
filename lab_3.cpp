#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

class MyArray {
private:
    int size;
    double* data;

public:
    MyArray(int n) {
        size = (n > 0) ? n : 1;
        data = new double[size];
        for (int i = 0; i < size; i++) {
            data[i] = (rand() % 101 - 50); // случайные числа [-50; 50]
        }
    }

    MyArray(const MyArray& other) {
        size = other.size;
        data = new double[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    ~MyArray() {
        delete[] data;
    }

    void print() const {
        for (int i = 0; i < size; i++) {
            cout << setw(7) << fixed << setprecision(1) << data[i] << " ";
        }
        cout << endl;
    }

    double operator--() {
        double sum = 0.0;
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (data[i] < 0) {
                sum += data[i];
                count++;
            }
        }
        if (count == 0) return 0.0;
        return sum / count;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned)time(nullptr));

    int n;
    cout << "Введите размер массива: ";
    cin >> n;

    MyArray arr1(n);
    cout << "\nМассив arr1:" << endl;
    arr1.print();

    double avg = --arr1;
    cout << "Среднее арифметическое отрицательных элементов = " << avg << endl;

    cout << "\nКопия массива (arr2):" << endl;
    MyArray arr2 = arr1;
    arr2.print();
    cout << "Среднее отрицательных в arr2 = " << (--arr2) << endl;

    return 0;
}