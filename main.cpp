#include "perlin.h"
#include <iostream>
#include <limits>

// чтение числа
int readNumber(const char* message, int minimum, int maximum) {
    int value;

    while (true) {
        std::cout << message;

        if (std::cin >> value && value >= minimum && value <= maximum) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Ошибка: введите целое число от "
                  << minimum << " до " << maximum << ".\n";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// вывод карты
void showMap(int width, int height, unsigned int seed) {
    std::cout << "\nГенерация карты мира...\n";
    std::cout << "Параметры: ширина = " << width
              << ", высота = " << height
              << ", Seed = " << seed << "\n";

    MapGenerator* generator = new PerlinMapGenerator(seed);
    WorldMap* map = generator->generate(width, height);

    MapRenderer renderer;
    renderer.print(*map);
    renderer.printLegend();
    renderer.printStatistics(*map);

    delete map;
    delete generator;
}

// ручная генерация
void runManualGeneration() {
    std::cout << "\n[РУЧНОЙ РЕЖИМ] Введите параметры карты:\n";

    int width = readNumber("Ширина карты (10-100): ", 10, 100);
    int height = readNumber("Высота карты (10-50): ", 10, 50);
    unsigned int seed = static_cast<unsigned int>(
        readNumber("Seed генерации (1-1000000): ", 1, 1000000));

    showMap(width, height, seed);
}

// демонстрация
void runDemo() {
    std::cout << "\nГенерация карты с предустановленными параметрами...\n";
    showMap(40, 20, 42);
}

// главная функция
int main() {
    std::cout << "=======================================================\n";
    std::cout << "  Лабораторная работа №5. Генерация карты (Шум Перлина)\n";
    std::cout << "=======================================================\n";

    int choice = -1;

    while (choice != 0) {
        std::cout << "\n--- Главное меню ---\n";
        std::cout << "1. Сгенерировать карту вручную (ввод параметров)\n";
        std::cout << "2. Быстрая демонстрация (тестовые параметры)\n";
        std::cout << "0. Выход из программы\n";

        choice = readNumber("Ваш выбор: ", 0, 2);

        switch (choice) {
            case 1:
                runManualGeneration();
                break;
            case 2:
                runDemo();
                break;
            case 0:
                std::cout << "Выход из программы.\n";
                break;
        }
    }

    return 0;
}