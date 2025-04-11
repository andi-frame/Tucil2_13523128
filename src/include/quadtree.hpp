#pragma once
#include "calc_error.hpp"
#include "image.hpp"
using namespace std;

struct QuadNode {
    int x, y, size;
    Pixel color;
    bool isLeaf;
    QuadNode *children[4];

    QuadNode(int x, int y, int size);
    ~QuadNode();
};

class QuadTree {
private:
    QuadNode *root;
    Image *image;
    ErrorMethod method;
    double threshold;
    int minBlockSize;

    QuadNode *compress(int x, int y, int size);

public:
    QuadTree(Image *img, ErrorMethod method, double threshold,
             int minBlockSize);
    ~QuadTree();

    void compress();
    void reconstructImage();
    int getTreeDepth();
    int getNodeCount();

    QuadNode *getRoot() const;
};
