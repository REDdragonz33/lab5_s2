#include "perlin.h"
#include <cmath>

// узел перестановки
PerlinNoise::NumberNode::NumberNode(int v) : value(v), next(0) {}

// конструктор
PerlinNoise::PerlinNoise(unsigned int seed) : permutation(0) {
    shufflePermutation(seed);
}

// деструктор
PerlinNoise::~PerlinNoise() {
    NumberNode* current = permutation;
    while (current) {
        NumberNode* following = current->next;
        delete current;
        current = following;
    }
}

// перемешивание перестановки
void PerlinNoise::shufflePermutation(unsigned int seed) {
    NumberNode* values = 0;
    NumberNode* tail = 0;
    for (int i = 0; i < 256; ++i) {
        NumberNode* node = new NumberNode(i);
        if (!values) values = node;
        else tail->next = node;
        tail = node;
    }

    // собственный генератор псевдослучайных чисел lcg
    unsigned int state = seed;
    for (int i = 255; i > 0; --i) {
        state = state * 1664525u + 1013904223u;
        int j = static_cast<int>(state % static_cast<unsigned int>(i + 1));
        NumberNode* left = values;
        NumberNode* right = values;
        for (int step = 0; step < i; ++step) left = left->next;
        for (int step = 0; step < j; ++step) right = right->next;
        int temporary = left->value;
        left->value = right->value;
        right->value = temporary;
    }

    NumberNode* source = values;
    NumberNode* outputTail = 0;
    for (int i = 0; i < 512; ++i) {
        NumberNode* node = new NumberNode(source->value);
        if (!permutation) permutation = node;
        else outputTail->next = node;
        outputTail = node;
        source = source->next;
        if (!source) source = values;
    }

    while (values) {
        NumberNode* following = values->next;
        delete values;
        values = following;
    }
}

// получение значения по индексу
int PerlinNoise::valueAt(int index) const {
    index %= 512;
    if (index < 0) index += 512;
    NumberNode* current = permutation;
    for (int i = 0; i < index; ++i) current = current->next;
    return current->value;
}

// сглаживание
double PerlinNoise::fade(double value) {
    return value * value * value * (value * (value * 6.0 - 15.0) + 10.0);
}

// интерполяция
double PerlinNoise::interpolate(double a, double b, double weight) {
    return a + weight * (b - a);
}

// градиент
double PerlinNoise::gradient(int hash, double x, double y) {
    switch (hash & 7) {
        case 0: return x + y;
        case 1: return -x + y;
        case 2: return x - y;
        case 3: return -x - y;
        case 4: return x;
        case 5: return -x;
        case 6: return y;
        default: return -y;
    }
}

// вычисление шума
double PerlinNoise::noise(double x, double y) const {
    int cellX = static_cast<int>(std::floor(x)) & 255;
    int cellY = static_cast<int>(std::floor(y)) & 255;
    double localX = x - std::floor(x);
    double localY = y - std::floor(y);
    double u = fade(localX);
    double v = fade(localY);

    int aa = valueAt(valueAt(cellX) + cellY);
    int ab = valueAt(valueAt(cellX) + cellY + 1);
    int ba = valueAt(valueAt(cellX + 1) + cellY);
    int bb = valueAt(valueAt(cellX + 1) + cellY + 1);

    double bottom = interpolate(gradient(aa, localX, localY),
                                gradient(ba, localX - 1.0, localY), u);
    double top = interpolate(gradient(ab, localX, localY - 1.0),
                             gradient(bb, localX - 1.0, localY - 1.0), u);
    return interpolate(bottom, top, v) * 0.7;
}

// деструктор генератора
MapGenerator::~MapGenerator() {}

// конструктор генератора
PerlinMapGenerator::PerlinMapGenerator(unsigned int seed) : noise(seed) {}

// выбор ландшафта
Terrain PerlinMapGenerator::chooseTerrain(double value) const {
    if (value < -0.10) return Terrain::Sea;
    if (value < -0.04) return Terrain::Beach;
    if (value < 0.04) return Terrain::Plain;
    if (value < 0.10) return Terrain::Forest;
    if (value < 0.17) return Terrain::Desert;
    return Terrain::Mountain;
}

// генерация карты
WorldMap* PerlinMapGenerator::generate(int width, int height) const {
    WorldMap* result = new WorldMap(width, height);
    const double scale = 0.09;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double nx = x * scale;
            double ny = y * scale;
            double value = noise.noise(nx, ny) * 0.65
                         + noise.noise(nx * 2.0, ny * 2.0) * 0.25
                         + noise.noise(nx * 4.0, ny * 4.0) * 0.10;
            result->addTile(Tile(x, y, value, chooseTerrain(value)));
        }
    }
    return result;
}