#include <iostream>
using namespace std;

int main() {
    int method, minBlockSize;
    double threshold, percentageTarget;
    string inputPath, outputPath, outputGifPath;

    cout << "Input absolute image path: ";
    cin >> inputPath;

    cout << "Error Measurement Methods:";
    cout << "1. Variance";
    cout << "2. Mean Absolute Deviation (MAD)";
    cout << "3. Max Pixel Difference";
    cout << "4. Entropy";
    cout << "5. Structural Similarity Index (SSIM)";
    cout << "Input choosen method (number only): ";
    cin >> method;

    cout << "Input Threshold: ";
    cin >> threshold;

    cout << "Input minimum block size: ";
    cin >> minBlockSize;

    cout << "Input target compression percentage in floating number (1.0 = "
            "100%) "
            "(0 to disable): "
         << endl;
    cin >> percentageTarget;

    cout << "Input absolute result image path: ";
    cin >> outputPath;

    cout << "Input absolute result gif path: ";
    cin >> outputGifPath;

    return 0;
}
