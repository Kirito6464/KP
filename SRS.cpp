#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Структура данных выпускника
struct Graduate {
    string fullName;        // ФИО выпускника
    string faculty;         // Факультет
    string specialty;       // Специальность
    int gradYear;           // Год выпуска
    string workplace;       // Место работы / должность
    bool isSmallBusiness;   // Работа в сфере малого бизнеса
};

// Прототипы функций
void printHeader();
void printRow(int index, const Graduate& g);
void printFooter();
void showAllGraduates(const vector<Graduate>& list);
void filterByYear(const vector<Graduate>& list);
void countByFaculty(const vector<Graduate>& list);
void countSmallBusiness(const vector<Graduate>& list);
void addGraduate(vector<Graduate>& list);
void saveToFile(const vector<Graduate>& list, const string& filename);
void loadFromFile(vector<Graduate>& list, const string& filename);
int getValidInt(int minVal, int maxVal);

int main() {
    // Настройка UTF-8 для корректного вывода кириллицы в терминале VS Code
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    vector<Graduate> graduates;
    const string filename = "graduates.txt";

    // Автоматическая загрузка данных при старте
    loadFromFile(graduates, filename);

    int choice = 0;
    do {
        cout << "\n======================================================\n";
        cout << "  ИНФОРМАЦИОННО-СПРАВОЧНАЯ СИСТЕМА УЧЕТА ВЫПУСКНИКОВ  \n";
        cout << "======================================================\n";
        cout << "1. Просмотреть весь список выпускников (таблица)\n";
        cout << "2. Добавить выпускника в текущий список\n";
        cout << "3. Создать новый пустой список (очистить текущий)\n";
        cout << "4. Поиск и просмотр выпускников конкретного года\n";
        cout << "5. Количество выпускников конкретного факультета\n";
        cout << "6. Количество выпускников, работающих в малом бизнесе\n";
        cout << "7. Сохранить текущие данные в файл\n";
        cout << "8. Загрузить данные из файла\n";
        cout << "0. Выход из программы\n";
        cout << "------------------------------------------------------\n";
        cout << "Выберите пункт меню: ";

        choice = getValidInt(0, 8);

        switch (choice) {
        case 1:
            showAllGraduates(graduates);
            break;
        case 2:
            addGraduate(graduates);
            break;
        case 3: {
            char confirm;
            cout << "Очистить текущий список? (y/n): ";
            cin >> confirm;
            cin.ignore(1000, '\n');
            if (confirm == 'y' || confirm == 'Y' || confirm == 'д' || confirm == 'Д') {
                graduates.clear();
                cout << "[Инфо] Список очищен.\n";
            }
            break;
        }
        case 4:
            filterByYear(graduates);
            break;
        case 5:
            countByFaculty(graduates);
            break;
        case 6:
            countSmallBusiness(graduates);
            break;
        case 7:
            saveToFile(graduates, filename);
            break;
        case 8:
            loadFromFile(graduates, filename);
            break;
        case 0:
            cout << "Завершение работы программы...\n";
            break;
        }

    } while (choice != 0);

    return 0;
}

void printHeader() {
    cout << "\n+" << string(4, '-') << "+" << string(26, '-') << "+" << string(18, '-') 
         << "+" << string(18, '-') << "+" << string(8, '-') << "+" << string(22, '-') 
         << "+" << string(14, '-') << "+\n";

    cout << "| " << left << setw(2) << "№"
         << " | " << setw(24) << "Ф.И.О."
         << " | " << setw(16) << "Факультет"
         << " | " << setw(16) << "Специальность"
         << " | " << setw(6)  << "Год"
         << " | " << setw(20) << "Место работы"
         << " | " << setw(12) << "Мал.бизнес"
         << " |\n";

    cout << "+" << string(4, '-') << "+" << string(26, '-') << "+" << string(18, '-') 
         << "+" << string(18, '-') << "+" << string(8, '-') << "+" << string(22, '-') 
         << "+" << string(14, '-') << "+\n";
}

void printRow(int index, const Graduate& g) {
    cout << "| " << left << setw(2) << index
         << " | " << setw(24) << (g.fullName.length() > 24 ? g.fullName.substr(0, 21) + "..." : g.fullName)
         << " | " << setw(16) << (g.faculty.length() > 16 ? g.faculty.substr(0, 13) + "..." : g.faculty)
         << " | " << setw(16) << (g.specialty.length() > 16 ? g.specialty.substr(0, 13) + "..." : g.specialty)
         << " | " << setw(6)  << g.gradYear
         << " | " << setw(20) << (g.workplace.length() > 20 ? g.workplace.substr(0, 17) + "..." : g.workplace)
         << " | " << setw(12) << (g.isSmallBusiness ? "Да" : "Нет")
         << " |\n";
}

void printFooter() {
    cout << "+" << string(4, '-') << "+" << string(26, '-') << "+" << string(18, '-') 
         << "+" << string(18, '-') << "+" << string(8, '-') << "+" << string(22, '-') 
         << "+" << string(14, '-') << "+\n";
}

void showAllGraduates(const vector<Graduate>& list) {
    if (list.empty()) {
        cout << "\n[Инфо] Список пуст!\n";
        return;
    }
    cout << "\n======================== ТАБЛИЦА ВЫПУСКНИКОВ ========================";
    printHeader();
    for (size_t i = 0; i < list.size(); ++i) {
        printRow((int)i + 1, list[i]);
    }
    printFooter();
    cout << "Всего записей: " << list.size() << "\n";
}

void filterByYear(const vector<Graduate>& list) {
    if (list.empty()) {
        cout << "\n[Инфо] Список пуст!\n";
        return;
    }
    int targetYear;
    cout << "\nВведите год выпуска для поиска: ";
    targetYear = getValidInt(1950, 2030);

    bool found = false;
    int counter = 0;

    printHeader();
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i].gradYear == targetYear) {
            printRow(++counter, list[i]);
            found = true;
        }
    }
    printFooter();

    if (!found) cout << "Выпускников за " << targetYear << " год не найдено.\n";
    else cout << "Найдено выпускников за " << targetYear << " год: " << counter << " чел.\n";
}

void countByFaculty(const vector<Graduate>& list) {
    if (list.empty()) {
        cout << "\n[Инфо] Список пуст!\n";
        return;
    }
    string targetFaculty;
    cout << "\nВведите точное название факультета: ";
    getline(cin, targetFaculty);

    int count = 0;
    printHeader();
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i].faculty == targetFaculty) {
            printRow(++count, list[i]);
        }
    }
    printFooter();

    cout << "Итого выпускников факультета \"" << targetFaculty << "\": " << count << " чел.\n";
}

void countSmallBusiness(const vector<Graduate>& list) {
    if (list.empty()) {
        cout << "\n[Инфо] Список пуст!\n";
        return;
    }
    int count = 0;
    cout << "\n============== ВЫПУСКНИКИ В МАЛОМ БИЗНЕСЕ ==============";
    printHeader();
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i].isSmallBusiness) {
            printRow(++count, list[i]);
        }
    }
    printFooter();

    cout << "Количество выпускников в малом бизнесе: " << count << " чел.\n";
}

void addGraduate(vector<Graduate>& list) {
    Graduate g;
    cout << "\n--- Добавление выпускника ---\n";
    cout << "ФИО: ";
    getline(cin, g.fullName);

    cout << "Факультет: ";
    getline(cin, g.faculty);

    cout << "Специальность: ";
    getline(cin, g.specialty);

    cout << "Год выпуска: ";
    g.gradYear = getValidInt(1950, 2030);

    cout << "Место работы и должность: ";
    getline(cin, g.workplace);

    cout << "Работает в малом бизнесе? (1 - Да, 0 - Нет): ";
    int sb = getValidInt(0, 1);
    g.isSmallBusiness = (sb == 1);

    list.push_back(g);
    cout << "[Успех] Запись добавлена!\n";
}

void saveToFile(const vector<Graduate>& list, const string& filename) {
    ofstream fout(filename.c_str());
    if (!fout.is_open()) {
        cout << "[Ошибка] Не удалось сохранить файл!\n";
        return;
    }
    fout << list.size() << "\n";
    for (size_t i = 0; i < list.size(); ++i) {
        fout << list[i].fullName << "\n"
             << list[i].faculty << "\n"
             << list[i].specialty << "\n"
             << list[i].gradYear << "\n"
             << list[i].workplace << "\n"
             << (list[i].isSmallBusiness ? 1 : 0) << "\n";
    }
    fout.close();
    cout << "[Успех] Список сохранен в файл: " << filename << "\n";
}

void loadFromFile(vector<Graduate>& list, const string& filename) {
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "[Инфо] База данных не обнаружена. Будет создан новый файл.\n";
        return;
    }
    size_t count = 0;
    if (!(fin >> count)) {
        fin.close();
        return;
    }
    fin.ignore(1000, '\n');

    list.clear();
    for (size_t i = 0; i < count; ++i) {
        Graduate g;
        getline(fin, g.fullName);
        getline(fin, g.faculty);
        getline(fin, g.specialty);
        fin >> g.gradYear;
        fin.ignore(1000, '\n');
        getline(fin, g.workplace);
        int sb;
        fin >> sb;
        fin.ignore(1000, '\n');
        g.isSmallBusiness = (sb == 1);

        list.push_back(g);
    }
    fin.close();
    cout << "[Успех] Загружено записей из файла: " << list.size() << "\n";
}

int getValidInt(int minVal, int maxVal) {
    int val;
    while (true) {
        if (cin >> val && val >= minVal && val <= maxVal) {
            cin.ignore(1000, '\n');
            return val;
        }
        cout << "Ошибка! Введите целое число (" << minVal << "-" << maxVal << "): ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}