#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

template <typename K, typename T>
class HashTable {
    struct Pair {
        K _key;
        T _value;
        bool _occupied;
        Pair() : _occupied(false) {}
    };

    std::vector<Pair> _data;
    size_t _size;
    size_t _w; // Число битов для сдвига
    size_t _modMask; // Маска для модуля

    size_t hash(const K& key) {
        // Хеш-функция для различных типов ключей
        if constexpr (std::is_same_v<K, std::string>) {
            size_t hash = 0;
            for (char c : key) {
                hash = (hash * 31) + c;
            }
            return hash % _size;
        }
        else {
            // Хеш-функция для числовых типов
            return ((key * _size) >> _w) & _modMask;
        }
    };

public:
    HashTable(size_t size, size_t w) : _size(size), _w(w) {
        if (size == 0)
            throw std::invalid_argument("Size=0");

        // Рассчитываем маску для модуля
        _modMask = (1 << w) - 1;

        _data.resize(size);
    };

    HashTable(size_t size, const T& max, const T& min) : _size(size) {
        if (size == 0)
            throw std::invalid_argument("Size=0");

        _data.resize(size);
        for (size_t i = 0; i < size; ++i) {
            T value = rand() % (max - min + 1) + min;
            insert(i, value);
        }
    };

    HashTable(size_t size, const T& max, const T& min, size_t el_quant) : _size(size) {
        if (size == 0)
            throw std::invalid_argument("Size=0");

        _data.resize(size);
        for (size_t i = 0; i < el_quant; ++i) {
            K key = rand();
            T value = rand() % (max - min + 1) + min;
            insert(key, value);
        }
    };

    HashTable(const HashTable& other) : _size(other.get_size()) {
        _data.resize(_size);
        for (size_t i = 0; i < _size; ++i) {
            if (other._data[i]._occupied) {
                // Копируем только занятые элементы
                insert(other._data[i]._key, other._data[i]._value);
            }
        }
    }

    ~HashTable() {
        _data.clear();
    }

    void insert(const K& key, const T& value) {
        size_t index = hash(key);
        size_t originalIndex = index;
        while (_data[index]._occupied) {
            if (_data[index]._key == key) {
                throw std::runtime_error("Key already exists");
            }
            index = (index + 1) % _size;
            if (index == originalIndex) {
                throw std::runtime_error("Hash table is full");
            }
        }
        _data[index]._key = key;
        _data[index]._value = value;
        _data[index]._occupied = true;
    };

    void insert_or_assign(const K& key, const T& value) {
        size_t index = hash(key);
        size_t originalIndex = index;
        while (_data[index]._occupied) {
            if (_data[index]._key == key) {
                _data[index]._value = value;
                return;
            }
            index = (index + 1) % _size;
            if (index == originalIndex) {
                throw std::runtime_error("Hash table is full");
            }
        }
        _data[index]._key = key;
        _data[index]._value = value;
        _data[index]._occupied = true;
    };

    bool contains(const T& value) const {
        for (const auto& pair : _data) {
            if (pair._occupied && pair._value == value) {
                return true;
            }
        }
        return false;
    };

    void print() const {
        for (const auto& pair : _data) {
            if (pair._occupied) {
                std::cout << pair._key << ":" << pair._value << std::endl;
            }
        }
    };

    size_t get_size() const {
        return _size;
    }

    std::vector<Pair> get_data() const {
        return _data;
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            _size = other._size;
            _w = other._w;
            _modMask = other._modMask;
            _data = other._data;
        }
        return *this;
    };

    bool erase(const K& key) {
        size_t index = hash(key);
        size_t originalIndex = index;
        while (_data[index]._occupied) {
            if (_data[index]._key == key) {
                _data[index]._occupied = false;
                return true;
            }
            index = (index + 1) % _size;
            if (index == originalIndex) {
                break;
            }
        }
        return false;
    };

    T* search(const K& key) {
        size_t index = hash(key);
        size_t originalIndex = index;
        while (_data[index]._occupied) {
            if (_data[index]._key == key) {
                return &(_data[index]._value);
            }
            index = (index + 1) % _size;
            if (index == originalIndex) {
                break;
            }
        }
        return nullptr;
    };

    int count(const K& key) {
        size_t index = hash(key);
        size_t originalIndex = index;
        int count = 0;
        while (_data[index]._occupied) {
            if (_data[index]._key == key) {
                count++;
            }
            index = (index + 1) % _size;
            if (index == originalIndex) {
                break;
            }
        }
        return count;
    };
};

int main() {

    srand(time(nullptr));

    HashTable<int, int> table(10, 3); // Размер таблицы: 10, число битов для сдвига: 3

    table.insert(1, 10);
    table.insert(2, 20);
    table.insert(3, 30);
    table.insert(11, 110);
    table.insert(22, 220);
    table.insert(33, 330);

    std::cout << "Hash Table contents:" << std::endl;
    table.print();
    std::cout << std::endl;

    // Проверяем функцию contains
    std::cout << "Contains 20? " << (table.contains(20) ? "Yes" : "No") << std::endl;
    std::cout << "Contains 30? " << (table.contains(30) ? "Yes" : "No") << std::endl;
    std::cout << "Contains 50? " << (table.contains(50) ? "Yes" : "No") << std::endl;
    std::cout << std::endl;

    // Удаляем элементы
    std::cout << "Erasing key 2: " << (table.erase(2) ? "Success" : "Key not found") << std::endl;
    std::cout << "Erasing key 3: " << (table.erase(3) ? "Success" : "Key not found") << std::endl;
    std::cout << "Erasing key 5: " << (table.erase(5) ? "Success" : "Key not found") << std::endl;
    std::cout << std::endl;

    // Печатаем содержимое таблицы после удаления
    std::cout << "Hash Table contents after erasing:" << std::endl;
    table.print();
    std::cout << std::endl;

    // Проверяем функции search и count
    int* value_ptr = table.search(11);
    if (value_ptr) {
        std::cout << "Value for key 11 found: " << *value_ptr << std::endl;
    }
    else {
        std::cout << "Key 11 not found" << std::endl;
    }

    int count_11 = table.count(11);
    std::cout << "Count of key 11: " << count_11 << std::endl;

    int count_33 = table.count(33);
    std::cout << "Count of key 33: " << count_33 << std::endl;

    const int experiments = 100;
    const int num_elements = 26;

    HashTable<int, std::string> table_1(5, 3); // Размер таблицы: 5, число битов для сдвига: 3

    table_1.insert(1, "One");
    table_1.insert(2, "Two");
    table_1.insert(3, "Three");
    table_1.insert(4, "Four");
    table_1.insert(5, "Five");

    std::cout << "Hash Table_1 contents:" << std::endl;
    table_1.print();
    std::cout << std::endl;

    HashTable<char, float> table_char(5, 3); // Размер таблицы: 5, число битов для сдвига: 3

    table_char.insert('A', 3.14);
    table_char.insert('B', 2.71);
    table_char.insert('C', 1.618);
    table_char.insert('D', 0.577);
    table_char.insert('E', 1.732);

    std::cout << "Hash Table contents:" << std::endl;
    table_char.print();
    std::cout << std::endl;

    HashTable<std::string, int> table_str(5, 3); // Размер таблицы: 5, число битов для сдвига: 3

    table_str.insert("One", 1);
    table_str.insert("Two", 2);
    table_str.insert("Three", 3);
    table_str.insert("Four", 4);
    table_str.insert("Five", 5);

    std::cout << "Hash Table_str contents:" << std::endl;
    table_str.print();
    std::cout << std::endl;

    std::cout << "Experiment: Collision Counts for Different Table Sizes" << std::endl;


    // Размер хэш таблиц должен изменяться от 25 до 475 с шагом 50
    for (int table_size = 25; table_size <= 475; table_size += 50) {
        int total_collisions = 0;

        // Повторяем эксперимент experiments раз
        for (int experiment = 0; experiment < experiments; ++experiment) {
            // Создаем новую таблицу указанного размера
            HashTable<int, int> table(table_size, 100, 0);

            // Вставляем 26 случайных элементов в таблицу
            for (int i = 0; i < num_elements; ++i) {
                int key = rand() % 1000; // Генерируем случайные ключи
                try {
                    table.insert_or_assign(key, i);
                }
                catch (const std::runtime_error& e) {
                    total_collisions++;
                }
            }
        }

        std::cout << "Table size: " << table_size << ", Collisions: " << total_collisions << std::endl;   
    }
    return 0;
}