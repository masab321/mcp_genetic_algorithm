#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int N = 10;

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

Node* create_node() {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        cout << "Memory allocation failed." << endl;
        exit(EXIT_FAILURE);
    }
    new_node->data = -1;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

Node* create_pool() {
    int pool_size = N + N;
    Node* start = create_node();
    Node* it = start;
    for (int i = 0; i < pool_size - 1; i++) {
        Node* nxt = create_node();
        it->next = nxt;
        nxt->prev = it;
        it = it->next;
    }
    return start;
}

Node* create_list_head(Node** pool_start, int d) {
    Node* start = *pool_start;

    *pool_start = (*pool_start)->next;
    if (*pool_start) {
        (*pool_start)->prev = NULL;
    }

    start->data = d;
    start->next = NULL;
    start->prev = NULL;
    return start;
}

void list_push_back(Node** list_end, Node** pool_start, int d) {
    (*list_end)->next = *pool_start;

    *pool_start = (*pool_start)->next;
    if (*pool_start) {
        (*pool_start)->prev = NULL;
    }

    (*list_end)->next->data = d;
    (*list_end)->next->prev = *list_end;
    (*list_end)->next->next = NULL;
    (*list_end) = (*list_end)->next;
}

void erase_node(Node** cur, Node** pool_start) {
    if (*cur == NULL) {
        return;
    }

    if ((*cur)->prev != NULL and (*cur)->next != NULL) {
        (*cur)->prev->next = (*cur)->next;
        (*cur)->next->prev = (*cur)->prev;
    }

    if ((*cur)->prev == NULL) {
        (*cur)->next->prev = NULL;
    }

    if ((*cur)->next == NULL) {
        (*cur)->prev->next = NULL;
    }

    Node* it = (*cur)->next;

    (*cur)->next = *pool_start;
    (*pool_start)->prev = *cur;
    
    *pool_start = *cur;
    (*pool_start)->data = -1;
    (*pool_start)->prev = NULL;
    
    *cur = it;
}


void free_list(Node** head) {
    Node* temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

void print_list(Node* head) {
    Node* current = head;
    int ls = 0;
    while (current != NULL) {
        ls++;
        printf("%d <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
    printf("List size %d\n", ls);
}


class DoublyLinkedList {
private:
    struct Node {
        int _data;
        Node* _next;
        Node* _prev;
    };

    Node* _pool_start;
    Node* _list_head;
    Node* _list_end;
    int _pool_size;

    Node* create_node() {
        Node* _new_node = (Node*)malloc(sizeof(Node));
        if (!_new_node) {
            cerr << "Memory allocation failed." << endl;
            exit(EXIT_FAILURE);
        }
        _new_node->_data = -1;
        _new_node->_next = nullptr;
        _new_node->_prev = nullptr;
        return _new_node;
    }

    void initialize_pool(int _size) {
        _pool_start = create_node();
        Node* _it = _pool_start;
        for (int _i = 0; _i < _size - 1; ++_i) {
            Node* _nxt = create_node();
            _it->_next = _nxt;
            _nxt->_prev = _it;
            _it = _it->_next;
        }
    }

    void free_pool() {
        Node* _temp;
        while (_pool_start != nullptr) {
            _temp = _pool_start;
            _pool_start = _pool_start->_next;
            free(_temp);
        }
    }

    void free_list() {
        Node* _temp;
        while (_list_head != nullptr) {
            _temp = _list_head;
            _list_head = _list_head->_next;
            free(_temp);
        }
    }

public:
    DoublyLinkedList(int _N) : _pool_start(nullptr), _list_head(nullptr), _list_end(nullptr), _pool_size(2 * _N) {
        initialize_pool(_pool_size);
    }

    ~DoublyLinkedList() {
        free_pool();
        free_list();
    }

    void create_list_head(int _data) {
        if (!_pool_start) {
            cerr << "Pool is empty." << endl;
            return;
        }
        Node* _start = _pool_start;
        _pool_start = _pool_start->_next;
        if (_pool_start) {
            _pool_start->_prev = nullptr;
        }
        _start->_data = _data;
        _start->_next = nullptr;
        _start->_prev = nullptr;
        _list_head = _start;
        _list_end = _start;
    }

    void push_back(int _data) {
        if (!_pool_start) {
            cerr << "Pool is empty." << endl;
            return;
        }
        Node* _new_node = _pool_start;
        _pool_start = _pool_start->_next;
        if (_pool_start) {
            _pool_start->_prev = nullptr;
        }
        _new_node->_data = _data;
        _new_node->_next = nullptr;
        _new_node->_prev = _list_end;
        if (_list_end) {
            _list_end->_next = _new_node;
        }
        _list_end = _new_node;
        if (!_list_head) {
            _list_head = _new_node;
        }
    }

    void erase_node(Node*& _cur) {
        if (_cur == nullptr) {
            return;
        }
        if (_cur->_prev != nullptr && _cur->_next != nullptr) {
            _cur->_prev->_next = _cur->_next;
            _cur->_next->_prev = _cur->_prev;
        } else if (_cur->_prev == nullptr) {
            if (_cur->_next) {
                _cur->_next->_prev = nullptr;
            }
            _list_head = _cur->_next;
        } else if (_cur->_next == nullptr) {
            if (_cur->_prev) {
                _cur->_prev->_next = nullptr;
            }
            _list_end = _cur->_prev;
        }
        _cur->_next = _pool_start;
        if (_pool_start) {
            _pool_start->_prev = _cur;
        }
        _pool_start = _cur;
        _pool_start->_data = -1;
        _pool_start->_prev = nullptr;
        _cur = _cur->_next;
    }

    void print_list() {
        Node* _current = _list_head;
        int _ls = 0;
        while (_current != nullptr) {
            ++_ls;
            cout << _current->_data << " <-> ";
            _current = _current->_next;
        }
        cout << "NULL\n";
        cout << "List size: " << _ls << "\n";
    }
};

int main() {
    int _N = 10;
    DoublyLinkedList _dll(_N);
    _dll.create_list_head(1);
    _dll.push_back(2);
    _dll.push_back(3);
    _dll.push_back(4);
    _dll.print_list();
    return 0;
}
