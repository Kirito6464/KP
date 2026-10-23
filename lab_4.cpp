using namespace System;

const int ROWS = 3;
const int COLS = 5;

void FillMatrix(array<double, 2>^ matr, Random^ rnd) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            matr[i, j] = Math::Round((rnd->NextDouble() * 20.0 - 10.0), 2);
        }
    }
}

void PrintMatrix(array<double, 2>^ matr, String^ name) {
    Console::WriteLine(L"Матрица {0}:", name);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            Console::Write(L"{0,7:F2} ", matr[i, j]);
        }
        Console::WriteLine();
    }
    Console::WriteLine();
}

array<int>^ CountPositiveCols(array<double, 2>^ matr) {
    array<int>^ res = gcnew array<int>(COLS);
    for (int j = 0; j < COLS; j++) {
        int count = 0;
        for (int i = 0; i < ROWS; i++) {
            if (matr[i, j] > 0.0) count++;
        }
        res[j] = count;
    }
    return res;
}

void PrintArray(array<int>^ arr, String^ name) {
    Console::Write(L"Массив {0}: ", name);
    for each (int val in arr) {
        Console::Write(L"{0} ", val);
    }
    Console::WriteLine(L"\n");
}

int main(array<System::String ^> ^args) {
    Random^ rnd = gcnew Random();

    array<double, 2>^ A = gcnew array<double, 2>(ROWS, COLS);
    array<double, 2>^ Q = gcnew array<double, 2>(ROWS, COLS);

    FillMatrix(A, rnd);
    FillMatrix(Q, rnd);

    PrintMatrix(A, L"A");
    PrintMatrix(Q, L"Q");

    array<int>^ B = CountPositiveCols(A);
    array<int>^ R = CountPositiveCols(Q);

    PrintArray(B, L"B");
    PrintArray(R, L"R");

    return 0;
}