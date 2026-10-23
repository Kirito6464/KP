#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

// Структура записи о клиенте
struct Client {
    char surname[30]; // Фамилия
    char country[30]; // Страна поездки
    int year;         // Год поездки
    double cost;      // Стоимость тура
};

// Функция для ровного вывода текста в терминале Linux (корректно считает символы UTF-8)
void print_cell(const char* str, int width) {
    cout << str;
    int chars_count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        // В UTF-8 каждый новый символ начинается не с байта 10xxxxxx (0x80)
        if ((str[i] & 0xC0) != 0x80) {
            chars_count++;
        }
    }
    for (int i = chars_count; i < width; i++) {
        cout << ' ';
    }
}

// Создание тестового файла, если его еще нет в папке
void create_default_txt(const char* name) {
    ifstream test(name);
    if (!test) {
        ofstream out(name);
        out << "Иванов Турция 2022 65000.50\n"
            << "Смирнов Египет 2023 54000.00\n"
            << "Кузнецов ОАЭ 2021 120000.00\n"
            << "Попов Турция 2023 72000.00\n"
            << "Васильев Италия 2019 95000.00\n";
        out.close();
    }
}

// Чтение текстового файла и запись в бинарный
int txt_to_bin(const char* txt_name, const char* bin_name) {
    ifstream in(txt_name);
    if (!in) return 0;

    ofstream out(bin_name, ios::binary);
    if (!out) {
        in.close();
        return 0;
    }

    Client c;
    int count = 0;
    while (in >> c.surname >> c.country >> c.year >> c.cost) {
        out.write((char*)&c, sizeof(Client));
        count++;
    }

    in.close();
    out.close();
    return count;
}

// Вывод бинарного файла на экран
void print_bin_file(fstream& f, int n) {
    Client c;
    f.seekg(0);

    print_cell("Фамилия", 12);
    cout << " | ";
    print_cell("Страна", 10);
    cout << " | " << setw(6) << "Год" << " | " << setw(12) << "Стоимость" << endl;
    cout << "----------------------------------------------------" << endl;

    for (int i = 0; i < n; i++) {
        f.read((char*)&c, sizeof(Client));
        print_cell(c.surname, 12);
        cout << " | ";
        print_cell(c.country, 10);
        cout << " | " << setw(6) << c.year
             << " | " << setw(12) << fixed << setprecision(2) << c.cost << endl;
    }
    cout << endl;
}

// Сортировка записей прямо в бинарном файле по возрастанию стоимости
void sort_bin_file(fstream& f, int n) {
    Client ci, cj, min_elem;
    int min_idx;

    for (int i = 0; i < n - 1; i++) {
        f.seekg(i * sizeof(Client));
        f.read((char*)&min_elem, sizeof(Client));
        min_idx = i;

        for (int j = i + 1; j < n; j++) {
            f.seekg(j * sizeof(Client));
            f.read((char*)&cj, sizeof(Client));
            if (cj.cost < min_elem.cost) {
                min_elem = cj;
                min_idx = j;
            }
        }

        if (min_idx != i) {
            f.seekg(i * sizeof(Client));
            f.read((char*)&ci, sizeof(Client));

            f.seekp(i * sizeof(Client));
            f.write((char*)&min_elem, sizeof(Client));

            f.seekp(min_idx * sizeof(Client));
            f.write((char*)&ci, sizeof(Client));
        }
    }
}

// Поиск клиентов, посетивших страну target
void find_by_country(fstream& f, int n, const char* target) {
    Client c;
    f.seekg(0);
    bool found = false;

    cout << "Результаты поиска для страны \"" << target << "\":" << endl;
    print_cell("Фамилия", 12);
    cout << " | ";
    print_cell("Страна", 10);
    cout << " | " << setw(6) << "Год" << " | " << setw(12) << "Стоимость" << endl;
    cout << "----------------------------------------------------" << endl;

    for (int i = 0; i < n; i++) {
        f.read((char*)&c, sizeof(Client));
        if (strcmp(c.country, target) == 0) {
            found = true;
            print_cell(c.surname, 12);
            cout << " | ";
            print_cell(c.country, 10);
            cout << " | " << setw(6) << c.year
                 << " | " << setw(12) << fixed << setprecision(2) << c.cost << endl;
        }
    }
    if (!found) {
        cout << "Клиентов, посетивших данную страну, не найдено." << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    const char* txt_file = "clients.txt";
    const char* bin_file = "clients.dat";

    // Создаем clients.txt с исходными данными, если файла нет
    create_default_txt(txt_file);

    // 1. Формируем бинарный файл из текстового
    int count = txt_to_bin(txt_file, bin_file);
    if (count == 0) {
        cout << "Ошибка: нет данных для обработки!" << endl;
        return 1;
    }

    // 2. Открываем двоичный файл на чтение и запись
    fstream file(bin_file, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Ошибка открытия двоичного файла!" << endl;
        return 1;
    }

    cout << "До сортировки:" << endl;
    print_bin_file(file, count);

    // 3. Сортируем бинарный файл по возрастанию стоимости прямо на диске
    sort_bin_file(file, count);

    cout << "После сортировки по стоимости:" << endl;
    print_bin_file(file, count);

    // 4. Поиск по стране X
    char search_country[30];
    cout << "Введите страну X для поиска: ";
    cin >> search_country;

    find_by_country(file, count, search_country);

    file.close();
    return 0;
}