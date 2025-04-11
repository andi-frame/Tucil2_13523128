#pragma once
#include <string>
#include <vector>
using namespace std;

struct Pixel {
    unsigned char r, g, b;
};

using ImageMatrix = vector<vector<Pixel>>;

class Image {
public:
    ImageMatrix data;
    int width, height;

    bool load(const string &path);
    bool save(const string &path);
    ImageMatrix getBlock(int x, int y, int blockSize);
    void fillBlock(int x, int y, int blockSize, Pixel color);
};