#include <iostream>
#include <vector>
#include <stdexcept>

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

    size_t hash(K key) {
        // Хеш-функция метода многократного сдвига
        return ((key * _size) >> _w) & _modMask;
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

