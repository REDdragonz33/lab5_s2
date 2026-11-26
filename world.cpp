#include "world.h"
#include <iostream>
#include <iomanip>

// узел списка
WorldMap::Node::Node(const Tile& value) : tile(new Tile(value)), next(0) {}

// деструктор узла
WorldMap::Node::~Node() { delete tile; }

// конструктор клетки
Tile::Tile(int px, int py, double h, Terrain t)
    : x(px), y(py), height(h), terrain(t) {}

// получение x
int Tile::getX() const { return x; }

// получение y
int Tile::getY() const { return y; }

// получение высоты
double Tile::getHeight() const { return height; }

// получение ландшафта
Terrain Tile::getTerrain() const { return terrain; }

// конструктор
WorldMap::WorldMap(int w, int h) : width(w), height(h), first(0), last(0) {}

// деструктор
WorldMap::~WorldMap() {
    Node* current = first;
    while (current) {
        Node* following = current->next;
        delete current;
        current = following;
    }
}

// получение ширины
int WorldMap::getWidth() const { return width; }

// получение высоты
int WorldMap::getHeight() const { return height; }

// добавление клетки
void WorldMap::addTile(const Tile& tile) {
    Node* node = new Node(tile);
    if (!first) first = node;
    else last->next = node;
    last = node;
}

// поиск клетки по координатам
const Tile& WorldMap::getTile(int x, int y) const {
    Node* current = first;
    while (current) {
        if (current->tile->getX() == x && current->tile->getY() == y)
            return *(current->tile);
        current = current->next;
    }
    // если клетка не найдена возвращаем пустую
    static const Tile empty(-1, -1, 0.0, Terrain::Plain);
    return empty;
}

// символ ландшафта
char MapRenderer::terrainSymbol(Terrain terrain) {
    switch (terrain) {
        case Terrain::Sea: return '~';
        case Terrain::Beach: return '.';
        case Terrain::Plain: return ' ';
        case Terrain::Forest: return 'T';
        case Terrain::Desert: return ':';
        case Terrain::Mountain: return '^';
    }
    return '?';
}

// имя ландшафта
const char* MapRenderer::terrainName(Terrain terrain) {
    switch (terrain) {
        case Terrain::Sea: return "Море";
        case Terrain::Beach: return "Пляж";
        case Terrain::Plain: return "Равнина";
        case Terrain::Forest: return "Лес";
        case Terrain::Desert: return "Пустыня";
        case Terrain::Mountain: return "Горы";
    }
    return "Неизвестно";
}

// вывод карты
void MapRenderer::print(const WorldMap& map) const {
    std::cout << "\nКАРТА МИРА\n";
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x)
            std::cout << terrainSymbol(map.getTile(x, y).getTerrain());
        std::cout << '\n';
    }
}

// вывод легенды
void MapRenderer::printLegend() const {
    std::cout << "\nОбозначения: ~ море, . пляж, пробел — равнина, "
                 "T лес, : пустыня, ^ горы\n";
}

// вывод статистики
void MapRenderer::printStatistics(const WorldMap& map) const {
    int sea = 0, beach = 0, plain = 0, forest = 0, desert = 0, mountain = 0;
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            switch (map.getTile(x, y).getTerrain()) {
                case Terrain::Sea: ++sea; break;
                case Terrain::Beach: ++beach; break;
                case Terrain::Plain: ++plain; break;
                case Terrain::Forest: ++forest; break;
                case Terrain::Desert: ++desert; break;
                case Terrain::Mountain: ++mountain; break;
            }
        }
    }
    int total = map.getWidth() * map.getHeight();
    std::cout << "\nСтатистика ландшафтов:\n";
    if (total == 0) return;
    std::cout << std::fixed << std::setprecision(1);
    std::cout << terrainName(Terrain::Sea) << ": " << sea << " (" << 100.0 * sea / total << "%)\n";
    std::cout << terrainName(Terrain::Beach) << ": " << beach << " (" << 100.0 * beach / total << "%)\n";
    std::cout << terrainName(Terrain::Plain) << ": " << plain << " (" << 100.0 * plain / total << "%)\n";
    std::cout << terrainName(Terrain::Forest) << ": " << forest << " (" << 100.0 * forest / total << "%)\n";
    std::cout << terrainName(Terrain::Desert) << ": " << desert << " (" << 100.0 * desert / total << "%)\n";
    std::cout << terrainName(Terrain::Mountain) << ": " << mountain << " (" << 100.0 * mountain / total << "%)\n";
}