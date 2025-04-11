#include "../include/calc_error.hpp"
#include <cmath>
#include <map>
#include <stdexcept>

// untuk node (in pixel)
Pixel averageColor(const ImageMatrix &block) {
    long r = 0, g = 0, b = 0;
    int count = block.size() * block[0].size();

    for (const auto &row : block)
        for (const auto &p : row) {
            r += p.r;
            g += p.g;
            b += p.b;
        }

    return {static_cast<unsigned char>(r / count),
            static_cast<unsigned char>(g / count),
            static_cast<unsigned char>(b / count)};
}

// untuk kalkulasi error (in double)
AverageColor computeAverageColor(const ImageMatrix &block) {
    long r = 0, g = 0, b = 0;
    int count = block.size() * block[0].size();

    for (const auto &row : block)
        for (const auto &p : row) {
            r += p.r;
            g += p.g;
            b += p.b;
        }

    return {r / (double)count, g / (double)count, b / (double)count};
}

double computeError(const ImageMatrix &block, ErrorMethod method,
                    const ImageMatrix *reconstructed) {
    AverageColor avg = computeAverageColor(block);
    int n = block.size() * block[0].size();
    double err = 0;

    switch (method) {
    case VARIANCE: {
        for (const auto &row : block)
            for (const auto &p : row)
                err += pow(p.r - avg.r, 2) + pow(p.g - avg.g, 2) +
                       pow(p.b - avg.b, 2);
        return err / n;
    }

    case MEAN_ABSOLUTE_DEVIATION: {
        for (const auto &row : block)
            for (const auto &p : row)
                err += abs(p.r - avg.r) + abs(p.g - avg.g) + abs(p.b - avg.b);
        return err / n;
    }

    case MAX_PIXEL_DIFFERENCE: {
        int minR = 255, minG = 255, minB = 255;
        int maxR = 0, maxG = 0, maxB = 0;
        for (const auto &row : block)
            for (const auto &p : row) {
                if (p.r > maxR) maxR = p.r;
                if (p.r < minR) minR = p.r;

                if (p.g < minG) minG = p.g;
                if (p.g > maxG) maxG = p.g;

                if (p.b < minB) minB = p.b;
                if (p.b > maxB) maxB = p.b;
            }
        return (maxR - minR + maxG - minG + maxB - minB) / 3.0;
    }

    case ENTROPY: {
        std::vector<int> histR(256, 0), histG(256, 0), histB(256, 0);

        for (const auto &row : block) {
            for (const auto &p : row) {
                histR[p.r]++;
                histG[p.g]++;
                histB[p.b]++;
            }
        }

        auto computeChannelEntropy =
            [&](const std::vector<int> &hist) -> double {
            double entropy = 0.0;
            for (int count : hist) {
                if (count > 0) {
                    double p = static_cast<double>(count) / n;
                    entropy -= p * std::log2(p);
                }
            }
            return entropy;
        };

        double entropyR = computeChannelEntropy(histR);
        double entropyG = computeChannelEntropy(histG);
        double entropyB = computeChannelEntropy(histB);

        return (entropyR + entropyG + entropyB) / 3.0;
    }

    case SSIM: {
        if (!reconstructed)
            throw invalid_argument("SSIM requires a reconstructed block.");

        const double C1 = 6.5025;  // (0.01 * 255)^2
        const double C2 = 58.5225; // (0.03 * 255)^2

        auto computeLuminance = [](const Pixel &p) -> double {
            return 0.299 * p.r + 0.587 * p.g + 0.114 * p.b;
        };

        double mu_x = 0.0, mu_y = 0.0;
        for (size_t i = 0; i < block.size(); ++i)
            for (size_t j = 0; j < block[0].size(); ++j) {
                mu_x += computeLuminance(block[i][j]);
                mu_y += computeLuminance((*reconstructed)[i][j]);
            }
        mu_x /= n;
        mu_y /= n;

        double sigma_x = 0.0, sigma_y = 0.0, sigma_xy = 0.0;
        for (size_t i = 0; i < block.size(); ++i)
            for (size_t j = 0; j < block[0].size(); ++j) {
                double x = computeLuminance(block[i][j]);
                double y = computeLuminance((*reconstructed)[i][j]);
                sigma_x += (x - mu_x) * (x - mu_x);
                sigma_y += (y - mu_y) * (y - mu_y);
                sigma_xy += (x - mu_x) * (y - mu_y);
            }

        sigma_x /= (n - 1);
        sigma_y /= (n - 1);
        sigma_xy /= (n - 1);

        double nume = (2 * mu_x * mu_y + C1) * (2 * sigma_xy + C2);
        double deno =
            (mu_x * mu_x + mu_y * mu_y + C1) * (sigma_x + sigma_y + C2);
        double ssim = nume / deno;

        return 1.0 - ssim;
    }

    default:
        return 0;
    }
}
