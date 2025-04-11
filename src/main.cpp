#include "include/image.hpp"
#include "include/quadtree.hpp"
#include <chrono>
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

int main() {
    int method, minBlockSize;
    double threshold, percentageTarget;
    string inputPath, outputPath, outputGifPath;

    // Inputs
    cout << "[INPUT] Absolute image path: ";
    cin >> inputPath;

    cout << "Error Measurement Methods:" << endl;
    cout << "1. Variance" << endl;
    cout << "2. Mean Absolute Deviation (MAD)" << endl;
    cout << "3. Max Pixel Difference" << endl;
    cout << "4. Entropy" << endl;
    cout << "5. Structural Similarity Index (SSIM)" << endl;
    cout << "[INPUT] choosen method (number only): ";
    cin >> method;

    cout << "[INPUT] Threshold: ";
    cin >> threshold;

    cout << "[INPUT] minimum block size: ";
    cin >> minBlockSize;

    cout << "[INPUT] target compression percentage in floating number (1.0 = "
            "100%) "
            "(0 to disable): ";
    cin >> percentageTarget;

    cout << "[INPUT] absolute result image path: ";
    cin >> outputPath;

    cout << "[INPUT] absolute result gif path: ";
    cin >> outputGifPath;

    cout << endl << endl;

    // Process
    Image image;
    if (!image.load(inputPath)) {
        cout << "Failed to load image.\n";
        return 1;
    }

    // == Start ==
    auto start = chrono::high_resolution_clock::now();

    QuadTree qt(&image, static_cast<ErrorMethod>(method), threshold,
                minBlockSize);
    qt.compress();
    qt.reconstructImage();

    if (!image.save(outputPath)) {
        cout << "Failed to save image.\n";
        return 1;
    }

    // ukuran file sebelum dan sesudah
    uintmax_t sizeBefore = fs::file_size(inputPath);
    uintmax_t sizeAfter = fs::file_size(outputPath);

    // persentase kompresi
    double compressionRatio = 100.0 * (1.0 - ((double)sizeAfter / sizeBefore));

    // == End ==
    auto end = chrono::high_resolution_clock::now();

    // Calc duration
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    // Outputs
    cout << "[OUTPUT] waktu eksekusi: " << duration.count() << "ms\n";
    cout << "[OUTPUT] ukuran gambar sebelum: " << sizeBefore << " bytes\n";
    cout << "[OUTPUT] ukuran gambar setelah: " << sizeAfter << " bytes\n";
    cout << "[OUTPUT] persentase kompresi: " << compressionRatio << "%\n";
    cout << "[OUTPUT] kedalaman pohon: " << qt.getTreeDepth() << "\n";
    cout << "[OUTPUT] banyak simpul pada pohon: " << qt.getNodeCount() << "\n";
    cout << "[OUTPUT] gambar hasil kompresi pada alamat yang sudah ditentukan: "
         << outputPath << "\n";

    return 0;
}
