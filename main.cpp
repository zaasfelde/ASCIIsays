#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <cstdlib>

std::vector<std::string> wrapTextToLines(const std::string& message, int maxLength = 50) {
    std::vector<std::string> lines;

    std::stringstream ss(message);
    std::string word;
    std::string currentLine;

    while (ss >> word) {
        if (currentLine.length() + word.length() + (currentLine.empty() ? 0 : 1) > maxLength) {
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
                currentLine.clear();
            }

            if (word.length() > maxLength) {
                for (size_t i = 0; i < word.length(); i += maxLength) {
                    lines.push_back(word.substr(i, maxLength));
                }
            } else {
                currentLine = word;
            }
        } else {
            if (!currentLine.empty()) {
                currentLine += " ";
            }
            currentLine += word;
        }
    }

    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    return lines;
}

int getVisualLength(const std::string& str) {
    int visualLength = 0;
    for (size_t i = 0; i < str.length(); i++) {
        if (static_cast<unsigned char>(str[i]) >= 0xD0 && static_cast<unsigned char>(str[i]) <= 0xDF) {
            visualLength++;
            i++;
        } else {
            visualLength++;
        }
    }
    return visualLength;
}

int countCyrillicChars(const std::string& str) {
    int count = 0;
    for (size_t i = 0; i < str.length(); i++) {
        if (static_cast<unsigned char>(str[i]) >= 0xD0 && static_cast<unsigned char>(str[i]) <= 0xDF) {
            count++;
            i++;
        }
    }
    return count;
}

void NikanorSays(const std::string& message, const std::string& artPath = "asciinikanor.txt") {
    std::vector<std::string> messageLines = wrapTextToLines(message, 50);

    int maxVisualLength = 0;
    for (const auto& line : messageLines) {
        int visualLength = getVisualLength(line);
        if (visualLength > maxVisualLength) {
            maxVisualLength = visualLength;
        }
    }

    std::string topDash;
    for (int i = 0; i < (maxVisualLength/2); ++i) {
        topDash += "-";
    }

    std::cout << "                ." << topDash << "\"" << topDash << ".\n";

    std::string topSpaces;
    for (int i = 0; i <= maxVisualLength; ++i) {
        topSpaces += " ";
    }
    std::cout << "               /" << topSpaces << "  \\\n";

    for (size_t i = 0; i < messageLines.size(); ++i) {
        const auto& line = messageLines[i];
        int visualLength = getVisualLength(line);
        int spacesNeeded = maxVisualLength - visualLength;
        std::string padding;
        for (int j = 0; j < spacesNeeded; ++j) {
            padding += " ";
        }

        std::cout << "              |  " << line << padding << "  |\n";
    }

    std::string bottomSpaces;
    for (int i = 0; i <= maxVisualLength; ++i) {
        bottomSpaces += " ";
    }
    std::cout << "              \\ " << bottomSpaces << "  /\n";

    std::string bottomDash;
    for (int i = 0; i < (maxVisualLength/2); ++i) {
        bottomDash += "-";
    }
    std::cout << "                `" << bottomDash << "\"" << bottomDash << "'\n";

    std::string spacesForBackslash;
    int tailSpaces = static_cast<int>(maxVisualLength * 0.6);
    if (tailSpaces > 25) {
        tailSpaces = 15;
    }
    for (int i = 0; i < tailSpaces; ++i) {
        spacesForBackslash += " ";
    }

    std::cout << "               " << spacesForBackslash << "\\" << std::endl;

    // Выводим ASCII арт из указанного файла
    std::ifstream file(artPath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        file.close();
    } else {
        std::cerr << "Ошибка: не удалось открыть файл с ASCII артом: " << artPath << std::endl;
        std::cerr << "Проверьте путь к файлу." << std::endl;
    }
}

void showUsage(const std::string& programName) {
    std::cout << "Использование:" << std::endl;
    std::cout << "  " << programName << " \"сообщение\" [путь_к_ascii_арту]" << std::endl;
    std::cout << std::endl;
    std::cout << "Примеры:" << std::endl;
    std::cout << "  " << programName << " \"Привет мир!\"" << std::endl;
    std::cout << "  " << programName << " \"Я Никанор\" /home/user/nikanor_art.txt" << std::endl;
    std::cout << "  " << programName << " \"Текст сообщения\" ./my_art.txt" << std::endl;
}

int main(int argc, char* argv[]) {

    const char* username = std::getenv("USER");
    std::string message;
    std::string artPath = "/home/" + std::string(username) + "/asciinikanor.txt"; // путь по умолчанию

    // Обработка аргументов командной строки
    if (argc == 1) {
        // Если аргументов нет - интерактивный режим
        std::cout << "Введите сообщение: ";
        std::getline(std::cin, message);
        std::cout << "Введите путь к ASCII арту (или Enter для значения по умолчанию): ";
        std::string inputPath;
        std::getline(std::cin, inputPath);
        if (!inputPath.empty()) {
            artPath = inputPath;
        }
    }
    else if (argc == 2) {
        // Один аргумент - только сообщение
        message = argv[1];
    }
    else if (argc == 3) {
        // Два аргумента - сообщение и путь к арту
        message = argv[1];
        artPath = argv[2];
    }
    else {
        // Слишком много аргументов
        std::cerr << "Ошибка: слишком много аргументов!" << std::endl;
        showUsage(argv[0]);
        return 1;
    }

    // Проверяем, не запрашивается ли помощь
    if (message == "-h" || message == "--help") {
        showUsage(argv[0]);
        return 0;
    }

    NikanorSays(message, artPath);
    return 0;
}
