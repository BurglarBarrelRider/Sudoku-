#include <iostream>
#include <vector> //  для работы с динамическими массивами - vector
#include <ctime> // для работы со временем.
#include <cstdlib> // функции стандартной библиотеки С++
#include <limits> //  для работы с числами и их границами
#include <iomanip> // для работы с форматированным выводом
#include <algorithm> //  для работы с алгоритмами на контейнерах
#include <random> //  для генерации случайных чисел

using namespace std;

const int SIZE = 9; 
    
    // В коде используется вложенный вектор(динамический массив) который отображает матрицу 
    // vector<vector<int>> matrix = {
    //     {1, 2, 3},
    //     {4, 5, 6},
    //     {7, 8, 9}
    // };

    // // Вывод элементов вложенного вектора
    // for (int i = 0; i < matrix.size(); ++i) {
    //     for (int j = 0; j < matrix[i].size(); ++j) {
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    
// Функция для отображения сетки Судоку с цветными значениями
void printGrid(const vector<vector<int>>& grid, const vector<vector<int>>& original, const vector<vector<bool>>& errors) {
    cout << "Sudoku Puzzle:" << endl;
    for (int i = 0; i < SIZE; ++i) {
        if (i % 3 == 0 && i != 0)
            cout << "-----------------------" << endl;
        for (int j = 0; j < SIZE; ++j) {
            if (j % 3 == 0 && j != 0)
                cout << "| ";
            if (grid[i][j] == 0)
                cout << "_ "; // пустые ячейки вида "_"
            else if (original[i][j] == 0 && errors[i][j])
                cout << "\033[1;31m" << grid[i][j] << "\033[0m "; // red for incorrect
            else if (original[i][j] == 0)
                cout << "\033[1;33m" << grid[i][j] << "\033[0m "; // orange for right player input
            else
                cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

// проверка можно ли разместить данное число в данной позиции
bool isValid(const vector<vector<int>>& grid, int row, int col, int num) {
    
    // Пример того, как используется адрес переменной grid
    // vector<vector<int>> grid = {
    //     {1, 2, 3},
    //     {4, 5, 6},
    //     {7, 8, 9}
    // };
    // isValid(&grid, 0, 0, 5);         
        // &grid указатель на grid позволяет функции isValid не изменять вектор grid и
        // и при этом получить доступ к элементам gridа для проверок которые выполняет isValid. Оптимизирует работу с памятью так как
        // так как использование ссылки/адреса позволяет избежать копирования данных и эффективно работать с большими структурами данных
        
    for (int i = 0; i < SIZE; ++i) {
        if (grid[row][i] == num && i != col)
            return false;
        if (grid[i][col] == num && i != row)
            return false;
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (grid[i + startRow][j + startCol] == num && (i + startRow != row || j + startCol != col))
                return false;
        }
    }
    return true;
}

// function to check if(no duplicate numbers in rows, columns, or 3x3 subgrids == false)  Если число дублируется, функция вернет true, иначе false
bool isDuplicated(const vector<vector<int>>& grid, int row, int col, int num) {
    for (int i = 0; i < SIZE; ++i) {
        if (grid[row][i] == num && i != col)
            return true;
        if (grid[i][col] == num && i != row)
            return true;
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (grid[i + startRow][j + startCol] == num && (i + startRow != row || j + startCol != col))
                return true;
        }
    }
    return false;
}

// функция с использованием стандартной функции shuffle из <algorithm>
void shuffleNumbers(vector<int>& numbers) {
    srand(time(nullptr));
    shuffle(numbers.begin(), numbers.end(), default_random_engine(time(nullptr)));
}

// перебирает все клетки судоку и для каждой пустой клетки пытается поставить все возможные числа 1 до 9, 
// проверяя их соответствие. Если число подходит то функция вызывается рекурсивно для следующей клетки (см. Рекурсия)
// в случае если все клетки заполнены верно, функция возвращает true.
bool solveSudoku(vector<vector<int>>& grid) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (grid[row][col] == 0) {
                vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                shuffleNumbers(numbers);
                for (int num : numbers) {
                    if (isValid(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (solveSudoku(grid))
                            return true;
                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void generateSudoku(vector<vector<int>>& grid, int difficulty) {
    grid.clear();
    grid.resize(SIZE, vector<int>(SIZE, 0));

    solveSudoku(grid);  

    int count;
    switch (difficulty) {
        case 1: count = 40; break;
        case 2: count = 55; break;
        case 3: count = 65; break;
        default: count = 55;
    }

    srand(time(nullptr)); //
    while (count > 0) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (grid[row][col] != 0) {
            grid[row][col] = 0;
            count--;
        }
    }
}

// Проверка на полностью решенное Судоку
bool isSolved(const vector<vector<int>>& grid) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (grid[i][j] == 0)
                return false; // ВОзвращает false если есть хотя бы один пустой элемент
        }
    }
    return true;
}

int main() {
    srand(time(nullptr)); // seed for random number generation

    vector<vector<int>> grid, original;
    vector<vector<bool>> errors(SIZE, vector<bool>(SIZE, false));

    int choice;
    cout << "Welcome to Sudoku Puzzle Game!" << endl;
    cout << "Choose difficulty level:" << endl;
    cout << "1. Easy\n2. Medium\n3. Hard" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (cin.fail() || choice < 1 || choice > 3) {
        cout << "Invalid input. Please enter a number between 1 and 3." << endl;
        return 1;
    }

    generateSudoku(grid, choice); 
    original = grid;

    printGrid(grid, original, errors); // вывод клетки Судоку 



    while (!isSolved(grid)) {
        int row, col, value;
        cout << "Enter your solution (row col value) or 'q' to quit: ";
        if (!(cin >> row >> col >> value)) {
            cout << "Not possible. Please enter integers." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (row < 1 || row > SIZE || col < 1 || col > SIZE || value < 1 || value > SIZE) {
            cout << "Not possible. Please enter row and column values between 1 and 9 and value between 1 and 9." << endl;
            continue;
        }
        if (original[row - 1][col - 1] != 0) {
            cout << "Cell is fixed. Better Try again." << endl;
            continue;
        }
        grid[row - 1][col - 1] = value;         // заполнение значением 
        if (isDuplicated(grid, row - 1, col - 1, value))
            errors[row - 1][col - 1] = true;
        else
            errors[row - 1][col - 1] = false;
        printGrid(grid, original, errors);      // вывод отображение обновленного Судоку
    }

    if (isSolved(grid))
        cout << "Congratulations! You solved the Sudoku puzzle!" << endl;

    return 0;
}