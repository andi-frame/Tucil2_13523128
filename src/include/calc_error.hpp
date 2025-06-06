#pragma once
#include "image.hpp"
using namespace std;

enum ErrorMethod {
    VARIANCE = 1,
    MEAN_ABSOLUTE_DEVIATION = 2,
    MAX_PIXEL_DIFFERENCE = 3,
    ENTROPY = 4,
    SSIM = 5
};

struct AverageColor {
    double r, g, b;
};

double computeError(const ImageMatrix &block, ErrorMethod method,
                    const ImageMatrix *reconstructed);
Pixel averageColor(const ImageMatrix &block);
AverageColor computeAverageColor(const ImageMatrix &block);
