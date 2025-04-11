#include "../include/quadtree.hpp"

QuadNode::QuadNode(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height), isLeaf(false) {
    fill(begin(children), end(children), nullptr);
}

QuadNode::~QuadNode() {
    for (auto &child : children)
        delete child;
}

QuadTree::QuadTree(Image *img, ErrorMethod method, double threshold,
                   int minBlockSize)
    : image(img), method(method), threshold(threshold),
      minBlockSize(minBlockSize), root(nullptr) {}

QuadTree::~QuadTree() { delete root; }

QuadNode *QuadTree::compress(int x, int y, int width, int height) {
    auto block = image->getBlock(x, y, width, height);
    double error = computeError(block, method);

    if (error <= threshold || (width * height) <= minBlockSize) {
        auto node = new QuadNode(x, y, width, height);
        node->isLeaf = true;
        node->color = averageColor(block);
        return node;
    }

    auto node = new QuadNode(x, y, width, height);

    int halfW = width / 2;
    int halfH = height / 2;

    node->children[0] = compress(x, y, halfW, halfH);
    node->children[1] = compress(x + halfW, y, width - halfW, halfH);
    node->children[2] = compress(x, y + halfH, halfW, height - halfH);
    node->children[3] =
        compress(x + halfW, y + halfH, width - halfW, height - halfH);

    return node;
}

void QuadTree::compress() {
    root = compress(0, 0, image->width, image->height);
}

void fill(Image *image, QuadNode *node) {
    if (!node) return;
    if (node->isLeaf) {
        image->fillBlock(node->x, node->y, node->width, node->height,
                         node->color);
    } else {
        for (auto &child : node->children)
            fill(image, child);
    }
}

void QuadTree::reconstructImage() { fill(image, root); }

int countNodes(QuadNode *node) {
    if (!node) return 0;
    int count = 1;
    for (auto &child : node->children)
        count += countNodes(child);
    return count;
}

int getDepth(QuadNode *node) {
    if (!node || node->isLeaf) return 0;
    int maxDepth = 0;
    for (auto &child : node->children)
        maxDepth = max(maxDepth, getDepth(child));
    return 1 + maxDepth;
}

int QuadTree::getTreeDepth() { return getDepth(root); }

int QuadTree::getNodeCount() { return countNodes(root); }

QuadNode *QuadTree::getRoot() const { return root; }
