#include <iostream>
#include <cstdlib>
#include <cctype>
using namespace std;

struct Node {
    int _data;
    Node* _next;
    Node* _prev;

    Node* operator++() {
        if (this->_next) {
            return this->_next;
        }
        return nullptr;
    }

    Node* operator++(int) {
        Node* temp = this;
        if (this->_next) {
            return this->_next;
        }
        return nullptr;
    }
};

class Dlist {
private:
    Node* _pool_head;
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
        _pool_head = create_node();
        Node* _it = _pool_head;
        for (int _i = 0; _i < _size - 1; ++_i) {
            Node* _nxt = create_node();
            _it->_next = _nxt;
            _nxt->_prev = _it;
            _it = _it->_next;
        }
    }

    void free_pool() {
        Node* _temp;
        while (_pool_head != nullptr) {
            _temp = _pool_head;
            _pool_head = _pool_head->_next;
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
    Dlist(int _N) : _pool_head(nullptr), _list_head(nullptr), _list_end(nullptr), _pool_size(2 * _N) {
        initialize_pool(_pool_size);
    }

    ~Dlist() {
        free_pool();
        free_list();
    }

    void create_list_head(int _data) {
        if (!_pool_head) {
            cerr << "Pool is empty." << endl;
            return;
        }
        Node* _start = _pool_head;
        _pool_head = _pool_head->_next;
        if (_pool_head) {
            _pool_head->_prev = nullptr;
        }
        _start->_data = _data;
        _start->_next = nullptr;
        _start->_prev = nullptr;
        _list_head = _start;
        _list_end = _start;
    }

    void push_back(int _data) {
        if (!_pool_head) {
            cerr << "Pool is empty." << endl;
            return;
        }
        Node* _new_node = _pool_head;
        _pool_head = _pool_head->_next;
        if (_pool_head) {
            _pool_head->_prev = nullptr;
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
        _cur->_next = _pool_head;
        if (_pool_head) {
            _pool_head->_prev = _cur;
        }
        _pool_head = _cur;
        _pool_head->_data = -1;
        _pool_head->_prev = nullptr;
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

    void print_pool() {
        Node* _current = _pool_head;
        int pool_size = 0;
        cout << "Pool: ";
        while (_current != nullptr) {
            ++pool_size;
            cout << _current->_data << " <-> ";
            _current = _current->_next;
        }
        cout << "NULL\n";
        cout << "Pool size: " << pool_size << "\n";
    }

    Node* begin() {
        return _list_head;
    }

    Node* end() {
        return nullptr;
    }

    void clear_list() {
        Node* current = _list_head;
        while (current) {
            Node* next = current->_next;
            erase_node(current);
            current = next;
        }
    }
};

void robust_test(Dlist& _dll) {
    for (int _test_count = 0; _test_count < 100; ++_test_count) {
        cout << "Test Iteration: " << _test_count + 1 << endl;

        _dll.create_list_head(0);
        
        for (int _i = 1; _i <= 10; ++_i) {
            _dll.push_back(_i);
        }

        cout << "List after adding 10 elements: ";
        _dll.print_list();
        _dll.print_pool();

        int _erase_count = 0;
        Node* _current = _dll.begin();
        bool _erase_even = (_test_count % 2 == 0);

        while (_current) {
            if ((_current->_data % 2 == 0) == _erase_even) {
                Node* to_erase = _current;
                _current = _current->_next;
                _dll.erase_node(to_erase);
                ++_erase_count;
            } else {
                _current = _current->_next;
            }
        }

        cout << "List after erasing odd/even nodes: ";
        _dll.print_list();
        _dll.print_pool();

        _dll.clear_list();
        cout << "List cleared after iteration " << _test_count + 1 << ".\n";
        
        cout << "List after clearing: ";
        _dll.print_list();
        cout << "Pool after clearing: ";
        _dll.print_pool();
        
        cout << "Press any key to continue to the next iteration..." << endl;
        cin.ignore();
    }
}

int main() {
    int _N = 10;
    Dlist _dll(_N);
    robust_test(_dll);

    return 0;
}
