#ifndef PERLIN_H
#define PERLIN_H

#include "world.h"

class PerlinNoise {
private:
    struct NumberNode {
        int value;
        NumberNode* next;
        NumberNode(int v);
    };
    NumberNode* permutation;

    int valueAt(int index) const;
    void shufflePermutation(unsigned int seed);
    static double fade(double value);
    static double interpolate(double a, double b, double weight);
    static double gradient(int hash, double x, double y);

public:
    explicit PerlinNoise(unsigned int seed);
    PerlinNoise(const PerlinNoise&) = delete;
    PerlinNoise& operator=(const PerlinNoise&) = delete;
    ~PerlinNoise();
    double noise(double x, double y) const;
};

class MapGenerator {
public:
    virtual ~MapGenerator();
    virtual WorldMap* generate(int width, int height) const = 0;
};

class PerlinMapGenerator : public MapGenerator {
private:
    PerlinNoise noise;
    Terrain chooseTerrain(double value) const;

public:
    explicit PerlinMapGenerator(unsigned int seed);
    WorldMap* generate(int width, int height) const override;
};

#endif