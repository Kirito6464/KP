#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct node {
    int a;
    node* next;
};

typedef node* Node_ptr;

Node_ptr create_list(int n) {
    Node_ptr head = nullptr, pred = nullptr;
    for (int i = 0; i < n; i++) {
        Node_ptr p = new node;
        p->a = rand() % 10;
        p->next = nullptr;
        if (!head) {
            head = p;
        } else {
            pred->next = p;
        }
        pred = p;
    }
    return head;
}

void print_node(Node_ptr p) {
    for (Node_ptr x = p; x != nullptr; x = x->next) {
        cout << x->a << " ";
    }
    cout << endl;
}

void insert_after_L(Node_ptr head, int L, int L1) {
    Node_ptr x = head;
    while (x != nullptr) {
        if (x->a == L) {
            Node_ptr n = new node;
            n->a = L1;
            n->next = x->next;
            x->next = n;
            x = n->next; // защита от бесконечного цикла, если L == L1
        } else {
            x = x->next;
        }
    }
}

void free_list(Node_ptr head) {
    while (head != nullptr) {
        Node_ptr temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned)time(nullptr));

    int n;
    cout << "Введите размер списка: ";
    cin >> n;

    Node_ptr K = create_list(n);
    cout << "Исходный список K:" << endl;
    print_node(K);

    int L, L1;
    cout << "Введите значение L: ";
    cin >> L;
    cout << "Введите значение L1: ";
    cin >> L1;

    insert_after_L(K, L, L1);

    cout << "Список после вставки:" << endl;
    print_node(K);

    free_list(K);
    return 0;
}