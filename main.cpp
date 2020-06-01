#include <iostream>
#include <fstream>

using namespace std;

const int MATRIX_SIZE = 100;
const char* PATH = "/home/andrew/CLionProjects/lab3/kek.rtr";

struct node{
    int key = 0, val = 0;
    node* next = nullptr;

    node(int v, int i){
        key = i;
        val = v;
    }
};

struct list {
    int size = 0;
    node *head = nullptr;
    node *local_head = nullptr;

    void add(int val, int key) {
        if (head == nullptr) head = new node(val, key);
        else {
            local_head = head;
            while (local_head->next != nullptr) {
                local_head = local_head->next;
            }
            local_head->next = new node(val, key);
        }
        size++;
    }

    int get_by_key(int key) {
        local_head = head;
        while (local_head != nullptr && local_head->key != key) local_head = local_head->next;

        if (local_head != nullptr) return local_head->val;

        return 0;
    }

    node* get_by_index(int index){
        if(index >= size) return nullptr;

        local_head = head;

        for(int i = 0; i < index; i++) local_head = local_head->next;

        return local_head;
    }

    void clear(){
        head = nullptr;
        local_head = nullptr;
        size = 0;
    }

    void print(){
        local_head = head;
        while (local_head != nullptr){
            cout << "key = " << local_head->key << "\tvalue = " << local_head->val << endl;
            local_head = local_head->next;
        }
    }
};

int* matrix_to_array(int** matrix){
    int* arr = new int[MATRIX_SIZE*MATRIX_SIZE];
    for(int i = 0; i < MATRIX_SIZE; i++){
        for(int j = 0; j < MATRIX_SIZE; j++){
            arr[i*MATRIX_SIZE+j] = matrix[i][j];
        }
    }
    return arr;
}
//Зберігає матрицю в ліст, не зберігаючи нулі(стиснено)
list* matrix_compression(int** matrix){
    list* arr = new list;

    for(int i = 0; i < MATRIX_SIZE; i++){
        for(int j = 0; j < MATRIX_SIZE; j++){
            if(matrix[i][j] != 0) arr->add(matrix[i][j], i*MATRIX_SIZE+j);
        }
    }
    return arr;
}

int** list_uncompresion(list* l){
    int** matrix = new int* [MATRIX_SIZE];

    for(int i = 0; i < MATRIX_SIZE; i++){
        matrix[i] = new int[MATRIX_SIZE];
        for(int j = 0; j < MATRIX_SIZE; j++){
            matrix[i][j] = l->get_by_key(i*MATRIX_SIZE+j);
        }
    }
    return matrix;
}
//зберігання матриці в ліст з вказання м послідовної кількості символів підряд
list* matrix_indexing(int** matrix){
    int* arr = matrix_to_array(matrix);
    list* l = new list;

    int count = 1;
    for(int i = 0; i < MATRIX_SIZE*MATRIX_SIZE; i++){
        if(i != 0){
            if(arr[i] == arr[i-1]){
                count++;
            }else{
                l->add(count, arr[i-1]);
                count = 1;
            }
        }
    }
    l->add(count, arr[MATRIX_SIZE*MATRIX_SIZE-1]);

    return l;
}

int** list_unindexing(list* l){
    int** matrix = new int* [MATRIX_SIZE];
    matrix[0] = new int [MATRIX_SIZE];

    int i = 0, j = 0;


    int index = 0;
    node* n = l->get_by_index(index);
    while(n != nullptr){
        for(int k = 0; k < n->val; k++){
            matrix[i][j] = n->key;

            if(j+1 == MATRIX_SIZE){
                i++;
                j = 0;
                matrix[i] = new int[MATRIX_SIZE];
            }else j++;
        }

        index++;
        n = l->get_by_index(index);
    }

    return matrix;
}

int** add(int** matrix, int** m){
    for(int i = 0; i < MATRIX_SIZE; i++){
        for(int j = 0; j < MATRIX_SIZE; j++){
            matrix[i][j] += m[i][j];
        }
    }
    return matrix;
}

int** mul(int** matrix, int** m){
    int** new_matrix = new int* [MATRIX_SIZE];
    int count;

    for(int i = 0; i < MATRIX_SIZE; i++){
        new_matrix[i] = new int[MATRIX_SIZE];

        for(int j = 0; j < MATRIX_SIZE; j++){
            count  = 0;
            for(int k = 0; k < MATRIX_SIZE; k++){
                count += matrix[k][j] * m[i][k];
            }
            new_matrix[i][j] = count;
        }
    }

    return new_matrix;
}
//запис + читання
void write(list* l, const char* path){
    fstream f(path, ios::binary | ios::out);

    int key = 0, val = 0;
    for(int i = 0; i < l->size; i++){
        key = l->get_by_index(i)->key;
        val = l->get_by_index(i)->val;
        cout << key << " " << val << endl;
        f.write((char*)&key, sizeof(int));
        f.write((char*)&val, sizeof(int));
    }
    f.close();
}

void read(list* l, const char* path){
    fstream f(path, ios::binary | ios::in);

    int val = 0, key = 0;
    while (true){
        f.read((char*)&key, sizeof(int));
        if(f.eof()) break;
        f.read((char*)&val, sizeof(int));
        l->add(val, key);
    }
    f.close();
}

void print(int** matrix){
    for(int j = 0; j < MATRIX_SIZE; j++){
        for(int i = 0; i < MATRIX_SIZE; i++){
            cout << matrix[i][j] << '\t';
        }
        cout << endl;
    }
}

int main() {
    int** matrix = new int* [MATRIX_SIZE];
    int** n = new int* [MATRIX_SIZE];

    for(int i = 0; i < MATRIX_SIZE; i++){
        matrix[i] = new int[MATRIX_SIZE];
        n[i] = new int[MATRIX_SIZE];
        for(int j = 0; j < MATRIX_SIZE; j++){
            matrix[i][j] = i*(j+2);
            n[i][j] = -i*(j+2);
        }
    }

    print(matrix);
    list* l = matrix_compression(matrix);

    write(l, PATH);
    l->clear();
    read(l, PATH);
    l->print();
    matrix = list_uncompresion(l);
    print(matrix);
    return 0;
}
