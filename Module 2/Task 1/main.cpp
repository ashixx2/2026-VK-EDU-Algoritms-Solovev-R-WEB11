#include <iostream>
#include <vector>
#include <string>

class HashTable {
private:
    enum CellState {
        EMPTY,
        DELETED,
        FILLED
    };

    struct Cell {
        std::string value;
        CellState state;

        Cell() {
            state = EMPTY;
        }
    };

    std::vector<Cell> table;
    int filled_count;
    int used_count;

    size_t get_hash(const std::string& str) const {
        size_t hash = 0;
        const size_t base = 31;

        // Вычисление хеша строки методом Горнера
        for (char c : str) {
            hash = hash * base + c;
        }

        return hash;
    }

    void rehash() {
        std::vector<Cell> old_table = table;

        table.clear();
        table.resize(old_table.size() * 2);

        filled_count = 0;
        used_count = 0;

        // Переносим только существующие элементы.
        // Удаленные ячейки при расширении таблицы не нужны.
        for (const Cell& cell : old_table) {
            if (cell.state == FILLED) {
                add(cell.value);
            }
        }
    }

public:
    HashTable() {
        table.resize(8);
        filled_count = 0;
        used_count = 0;
    }

    bool add(const std::string& str) {
        // При заполнении таблицы на 3/4 увеличиваем ее размер
        if (used_count * 4 >= static_cast<int>(table.size()) * 3) {
            rehash();
        }

        size_t hash = get_hash(str);
        size_t index = hash % table.size();

        int first_deleted = -1;

        // Квадратичное пробирование:
        // h, h + 1, h + 1 + 2, h + 1 + 2 + 3, ...
        for (size_t i = 0; i < table.size(); i++) {
            if (table[index].state == FILLED && table[index].value == str) {
                return false;
            }

            if (table[index].state == DELETED && first_deleted == -1) {
                first_deleted = static_cast<int>(index);
            }

            if (table[index].state == EMPTY) {
                if (first_deleted != -1) {
                    index = first_deleted;
                }
                else {
                    used_count++;
                }

                table[index].value = str;
                table[index].state = FILLED;
                filled_count++;

                return true;
            }

            index = (index + i + 1) % table.size();
        }

        // Этот случай нужен, если пустой ячейки не встретили,
        // но раньше была найдена удаленная ячейка.
        if (first_deleted != -1) {
            table[first_deleted].value = str;
            table[first_deleted].state = FILLED;
            filled_count++;

            return true;
        }

        return false;
    }

    bool remove(const std::string& str) {
        size_t hash = get_hash(str);
        size_t index = hash % table.size();

        for (size_t i = 0; i < table.size(); i++) {
            if (table[index].state == EMPTY) {
                return false;
            }

            if (table[index].state == FILLED && table[index].value == str) {
                table[index].value.clear();
                table[index].state = DELETED;
                filled_count--;

                return true;
            }

            index = (index + i + 1) % table.size();
        }

        return false;
    }

    bool contains(const std::string& str) const {
        size_t hash = get_hash(str);
        size_t index = hash % table.size();

        for (size_t i = 0; i < table.size(); i++) {
            if (table[index].state == EMPTY) {
                return false;
            }

            if (table[index].state == FILLED && table[index].value == str) {
                return true;
            }

            index = (index + i + 1) % table.size();
        }

        return false;
    }
};

int main() {
    HashTable hash_table;

    char operation;
    std::string str;

    while (std::cin >> operation >> str) {
        bool result = false;

        if (operation == '+') {
            result = hash_table.add(str);
        }
        else if (operation == '-') {
            result = hash_table.remove(str);
        }
        else if (operation == '?') {
            result = hash_table.contains(str);
        }

        if (result) {
            std::cout << "OK" << std::endl;
        }
        else {
            std::cout << "FAIL" << std::endl;
        }
    }

    return 0;
}