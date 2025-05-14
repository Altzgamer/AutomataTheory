#include "timeCounter.h"
#include "../generator/Generator.h"
#include <iostream>
#include <vector>
#include <memory>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    CommandGenerator cg;
    std::vector<std::string> validStringsShort;
    std::vector<std::string> invalidStringsShort;

    // Генерация команд в коротком режиме
    for (int i = 0; i < 1000; ++i) {
        validStringsShort.push_back(cg.generateValidCommand(false));
        invalidStringsShort.push_back(cg.generateInvalidCommand(false));
    }

    
    std::vector<std::unique_ptr<IMyRecognizer>> recognizers;
    recognizers.push_back(std::make_unique<RegexRecognizer>());
    recognizers.push_back(std::make_unique<FlexRecognizer>());
    recognizers.push_back(std::make_unique<SmcRecognizer>());

    std::vector<std::string> methods = { "regex", "flex", "smc" };

    // Измерение времени для каждого лексера в коротком режиме
    for (size_t i = 0; i < recognizers.size(); ++i) {
        std::cout << "Анализ с лексером " << methods[i] << ":\n";

        double avgTimeValidShort = TimeAnalyzer::timeAnalysis(*recognizers[i], validStringsShort);
        double avgTimeInvalidShort = TimeAnalyzer::timeAnalysis(*recognizers[i], invalidStringsShort);

        // Вывод результатов для короткого режима
        std::cout << "  Короткий режим (valid): " << avgTimeValidShort << " микросекунд\n";
        std::cout << "  Короткий режим (invalid): " << avgTimeInvalidShort << " микросекунд\n";
    }

    return 0;
}
