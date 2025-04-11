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
    try {
        // Image Input Path
        cout << "[INPUT] Absolute image path: ";
        cin >> inputPath;
        if (inputPath.empty())
            throw runtime_error("Input path cannot be empty.");

        // Error Method
        cout << "Error Measurement Methods:\n";
        cout << "1. Variance\n";
        cout << "2. Mean Absolute Deviation (MAD)\n";
        cout << "3. Max Pixel Difference\n";
        cout << "4. Entropy\n";
        cout << "5. Structural Similarity Index (SSIM)\n";
        cout << "[INPUT] Chosen method (number only): ";
        cin >> method;
        if (cin.fail() || method < 1 || method > 5)
            throw runtime_error("Invalid method: must be between 1 and 5.");

        // Threshold
        cout << "[INPUT] Threshold: ";
        cin >> threshold;
        if (cin.fail() || threshold < 0)
            throw runtime_error("Invalid threshold: must be >= 0.");
        switch (method) {
        case 1: // Variance
            if (threshold > 65025)
                throw runtime_error(
                    "For Variance, threshold must be <= 65025.");
            break;
        case 2: // MAD
            if (threshold > 255)
                throw runtime_error("For MAD, threshold must be <= 255.");
            break;
        case 3: // Max Pixel Difference
            if (threshold > 255)
                throw runtime_error(
                    "For Max Pixel Difference, threshold must be <= 255.");
            break;
        case 4: // Entropy
            if (threshold > 8)
                throw runtime_error("For Entropy, threshold must be <= 8.");
            break;
        case 5: // SSIM
            if (threshold > 1.0)
                throw runtime_error("For SSIM, threshold must be <= 1.0.");
            break;
        default:
            throw runtime_error("Unknown method.");
        }

        // Minimum Block Size
        cout << "[INPUT] Minimum block size: ";
        cin >> minBlockSize;
        if (cin.fail() || minBlockSize <= 0)
            throw runtime_error("Invalid minimum block size: must be > 0.");

        // Target Compression (%)
        cout << "[INPUT] Target compression percentage in floating number (1.0 "
                "= 100%) (0 to disable): ";
        cin >> percentageTarget;
        if (cin.fail() || percentageTarget < 0.0 || percentageTarget > 1.0)
            throw runtime_error(
                "Invalid percentage: must be between 0.0 and 1.0.");

        // Image Output Path
        cout << "[INPUT] Absolute result image path: ";
        cin >> outputPath;
        if (outputPath.empty())
            throw runtime_error("Output path cannot be empty.");

        // Gif Output Path
        cout << "[INPUT] Absolute result gif path: ";
        cin >> outputGifPath;
        if (outputGifPath.empty())
            throw runtime_error("GIF output path cannot be empty.");

    } catch (const runtime_error &e) {
        cerr << "[ERROR] " << e.what() << endl;
        return 1;
    }

    cout << endl << "Loading..." << endl;

    // Process
    // == Start ==
    auto start = chrono::high_resolution_clock::now();
    Image image;
    if (!image.load(inputPath)) {
        cout << "Failed to load image.\n";
        return 1;
    }

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
    cout << "Image successfully compressed!" << endl;
    cout << "[OUTPUT] execution time: " << duration.count() << "ms\n";
    cout << "[OUTPUT] image size before: " << sizeBefore << " bytes\n";
    cout << "[OUTPUT] image size after: " << sizeAfter << " bytes\n";
    cout << "[OUTPUT] compression percentage: " << compressionRatio << "%\n";
    cout << "[OUTPUT] tree depth: " << qt.getTreeDepth() << "\n";
    cout << "[OUTPUT] node count: " << qt.getNodeCount() << "\n";
    cout << "[OUTPUT] compressed image path: " << outputPath << "\n";

    return 0;
}
