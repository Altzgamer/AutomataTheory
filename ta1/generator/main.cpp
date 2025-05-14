#include "Generator.h"
#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <vector> 


int main() {
    setlocale(LC_ALL, "Russian");
    CommandGenerator generator;
    int count;
    bool longMode;

    std::cout << "Введите количество строк: ";
    std::cin >> count;
    std::cout << "Выберите режим (0 - короткие, 1 - длинные): ";
    std::cin >> longMode;

    std::ofstream correctFile("correct.txt");
    std::ofstream incorrectFile("incorrect.txt");

    if (!correctFile.is_open() || !incorrectFile.is_open()) {
        std::cerr << "Ошибка открытия файлов для записи\n";
        return 1;
    }

    int correctCount = 0, incorrectCount = 0;

    for (int i = 0; i < count; ++i) {
        correctFile << generator.generateValidCommand(longMode) << "\n";
        correctCount++;
    }

    for (int i = 0; i < count; ++i) {
        incorrectFile << generator.generateInvalidCommand(longMode) << "\n";
        incorrectCount++;
    }

    correctFile.close();
    incorrectFile.close();

    std::cout << "Сгенерированы команды в файлы correct.txt и incorrect.txt\n";
    std::cout << "Итог: " << correctCount << " правильных команд, " << incorrectCount << " неправильных команд.\n";

    system("pause");
    return 0;
}