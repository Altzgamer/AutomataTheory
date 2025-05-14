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

    std::cout << "������� ���������� �����: ";
    std::cin >> count;
    std::cout << "�������� ����� (0 - ��������, 1 - �������): ";
    std::cin >> longMode;

    std::ofstream correctFile("correct.txt");
    std::ofstream incorrectFile("incorrect.txt");

    if (!correctFile.is_open() || !incorrectFile.is_open()) {
        std::cerr << "������ �������� ������ ��� ������\n";
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

    std::cout << "������������� ������� � ����� correct.txt � incorrect.txt\n";
    std::cout << "����: " << correctCount << " ���������� ������, " << incorrectCount << " ������������ ������.\n";

    system("pause");
    return 0;
}