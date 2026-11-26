#ifndef WORLD_H
#define WORLD_H

enum class Terrain {
    Sea, Beach, Plain, Forest, Desert, Mountain
};

class Tile {
private:
    int x;
    int y;
    double height;
    Terrain terrain;
public:
    Tile(int px, int py, double h, Terrain t);
    int getX() const;
    int getY() const;
    double getHeight() const;
    Terrain getTerrain() const;
};

class WorldMap {
private:
    struct Node {
        Tile* tile;
        Node* next;
        Node(const Tile& value);
        ~Node();
    };
    int width;
    int height;
    Node* first;
    Node* last;
public:
    WorldMap(int w, int h);
    WorldMap(const WorldMap&) = delete;
    WorldMap& operator=(const WorldMap&) = delete;
    ~WorldMap();
    int getWidth() const;
    int getHeight() const;
    void addTile(const Tile& tile);
    const Tile& getTile(int x, int y) const;
};

class MapRenderer {
private:
    static char terrainSymbol(Terrain terrain);
    static const char* terrainName(Terrain terrain);
public:
    void print(const WorldMap& map) const;
    void printLegend() const;
    void printStatistics(const WorldMap& map) const;
};

#endif
