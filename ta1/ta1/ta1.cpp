#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <string>
#include "Recognizers/IMyRecognizer.h"
#include "Recognizers/RegexRecognizer.h"
#include "Recognizers/FlexRecognizer.h"
#include "Recognizers/SmcRecognizer.h"


int validCount = 0;
int invalidCount = 0;

void processInputManual(IMyRecognizer& recognizer) {
    std::unordered_map<std::string, std::vector<std::string>> commandStats;
    std::string command;

    while (true) {
        std::cout << "Введите команду (или 'exit' для выхода): ";
        std::getline(std::cin, command);
        if (command == "exit")
            break;

        if (recognizer.processCommand(command, commandStats)) {
            std::cout << "Valid" << std::endl;
            validCount++;
        }
        else {
            std::cout << "Invalid" << std::endl;
            invalidCount++;
        }
    }

    if (!commandStats.empty()) {
        std::cout << "\nСтатистика использования команд:\n";
        for (const auto& entry : commandStats) {
            std::cout << entry.first;
            for (const auto& key : entry.second) {
                std::cout << " " << key;
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "\nНет корректных команд для статистики.\n";
    }
}

void processInputFile(IMyRecognizer& recognizer, const std::string& inputFile, const std::string& outputFile) {
    std::unordered_map<std::string, std::vector<std::string>> commandStats;
    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть входной файл '" << inputFile << "'\n";
        return;
    }

    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть выходной файл '" << outputFile << "'\n";
        inFile.close();
        return;
    }

    std::string command;
    while (std::getline(inFile, command)) {
        if (command.empty()) continue;
        if (recognizer.processCommand(command, commandStats)) {
            outFile << command << ": Valid\n";
            validCount++;
        }
        else {
            outFile << command << ": Invalid\n";
            invalidCount++;
        }
    }

    inFile.close();
    outFile.close();

    std::cout << "\nРезультаты обработки:\n";
    std::cout << validCount << " правильных, " << invalidCount << " неправильных\n";
    std::cout << "Нажмите Enter для выхода...";
    std::cin.get();
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    int choice;
    std::cout << "Выберите способ ввода:\n"
        << "1 - Ввести команду вручную\n"
        << "2 - Проверить команды из файла\n"
        << "Введите ваш выбор: ";
    std::cin >> choice;
    std::cin.ignore();

    std::unique_ptr<IMyRecognizer> recognizer;
    std::string method = (argc > 1) ? argv[1] : "flex";
    if (method == "regex") {
        recognizer = std::make_unique<RegexRecognizer>();
    }
    else if (method == "flex") {
        recognizer = std::make_unique<FlexRecognizer>();
    }
    else if (method == "smc") {
        recognizer = std::make_unique<SmcRecognizer>();
    }
    else {
        std::cerr << "Неизвестный метод: " << method << "\n";
        return 1;
    }

    if (choice == 1) {
        processInputManual(*recognizer);
    }
    else if (choice == 2) {
        std::string inputFile, outputFile;
        std::cout << "Введите имя входного файла с расширением: ";
        std::getline(std::cin, inputFile);
        std::cout << "Введите имя выходного файла с расширением: ";
        std::getline(std::cin, outputFile);
        processInputFile(*recognizer, inputFile, outputFile);
    }
    else {
        std::cerr << "Некорректный выбор!" << std::endl;
        return 1;
    }

    std::cout  << validCount << " правильных, " << invalidCount << " неправильных\n";


    return 0;
}
