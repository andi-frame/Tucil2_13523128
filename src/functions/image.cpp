#include "../include/image.hpp"
#include <FreeImage.h>
#include <iostream>

bool Image::load(const string &path) {
    FreeImage_Initialise();

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str());
    if (format == FIF_UNKNOWN)
        format = FreeImage_GetFIFFromFilename(path.c_str());
    if (format == FIF_UNKNOWN) return false;

    FIBITMAP *rawBitmap = FreeImage_Load(format, path.c_str());
    if (!rawBitmap) return false;

    FIBITMAP *bitmap24 = FreeImage_ConvertTo24Bits(rawBitmap);
    FreeImage_Unload(rawBitmap);

    width = FreeImage_GetWidth(bitmap24);
    height = FreeImage_GetHeight(bitmap24);
    data.resize(height, vector<Pixel>(width));

    BYTE *bits = FreeImage_GetBits(bitmap24);
    for (int y = 0; y < height; ++y) {
        BYTE *pixel = bits + y * FreeImage_GetPitch(bitmap24);
        for (int x = 0; x < width; ++x) {
            BYTE b = *pixel++;
            BYTE g = *pixel++;
            BYTE r = *pixel++;
            data[height - y - 1][x] = {r, g, b};
        }
    }

    FreeImage_Unload(bitmap24);
    FreeImage_DeInitialise();
    return true;
}

bool Image::save(const string &path) {
    FreeImage_Initialise();

    FIBITMAP *bitmap24 = FreeImage_Allocate(width, height, 24);
    if (!bitmap24) return false;

    for (int y = 0; y < height; ++y) {
        BYTE *bits = FreeImage_GetScanLine(bitmap24, height - y - 1);
        for (int x = 0; x < width; ++x) {
            Pixel p = data[y][x];
            *bits++ = p.b;
            *bits++ = p.g;
            *bits++ = p.r;
        }
    }

    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path.c_str());
    if (format == FIF_UNKNOWN) format = FIF_PNG;

    bool success = FreeImage_Save(format, bitmap24, path.c_str());
    FreeImage_Unload(bitmap24);
    FreeImage_DeInitialise();
    return success;
}

ImageMatrix Image::getBlock(int x, int y, int blockSize) {
    ImageMatrix block;
    for (int i = 0; i < blockSize; ++i) {
        vector<Pixel> row;
        for (int j = 0; j < blockSize; ++j) {
            if (y + i < height && x + j < width)
                row.push_back(data[y + i][x + j]);
            else
                row.push_back({0, 0, 0});
        }
        block.push_back(row);
    }
    return block;
}

void Image::fillBlock(int x, int y, int blockSize, Pixel color) {
    for (int i = 0; i < blockSize; ++i) {
        for (int j = 0; j < blockSize; ++j) {
            if (y + i < height && x + j < width) data[y + i][x + j] = color;
        }
    }
}
