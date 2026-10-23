#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

struct Client {
    char surname[30];
    char country[30];
    int year;
    double cost;
};

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

void print_bin_file(fstream& f, int n) {
    Client c;
    f.seekg(0);
    cout << setw(12) << "Фамилия" << " | "
         << setw(12) << "Страна" << " | "
         << setw(6)  << "Год" << " | "
         << setw(12) << "Стоимость" << endl;
    cout << "---------------------------------------------" << endl;

    for (int i = 0; i < n; i++) {
        f.read((char*)&c, sizeof(Client));
        cout << setw(12) << c.surname << " | "
             << setw(12) << c.country << " | "
             << setw(6)  << c.year << " | "
             << setw(12) << fixed << setprecision(2) << c.cost << endl;
    }
    cout << endl;
}

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

void find_by_country(fstream& f, int n, const char* target) {
    Client c;
    f.seekg(0);
    bool found = false;

    cout << "Результаты поиска для страны \"" << target << "\":" << endl;
    for (int i = 0; i < n; i++) {
        f.read((char*)&c, sizeof(Client));
        if (strcmp(c.country, target) == 0) {
            found = true;
            cout << setw(12) << c.surname << " | "
                 << setw(12) << c.country << " | "
                 << setw(6)  << c.year << " | "
                 << setw(12) << fixed << setprecision(2) << c.cost << endl;
        }
    }
    if (!found) cout << "Записей не найдено." << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    const char* txt_file = "clients.txt";
    const char* bin_file = "clients.dat";

    create_default_txt(txt_file);

    int count = txt_to_bin(txt_file, bin_file);
    if (count == 0) return 1;

    fstream file(bin_file, ios::in | ios::out | ios::binary);
    if (!file) return 1;

    cout << "До сортировки:" << endl;
    print_bin_file(file, count);

    sort_bin_file(file, count);

    cout << "После сортировки по стоимости:" << endl;
    print_bin_file(file, count);

    char search_country[30];
    cout << "Введите страну X для поиска: ";
    cin >> search_country;

    find_by_country(file, count, search_country);

    file.close();
    return 0;
}