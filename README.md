# Tucil2_13523128
oleh: 13523128 - Andi Farhan Hidayat

## Penjelasan
Sebuah program untuk melakukan compress gambar menggunakan quadtree dengan algoritma divide and conquer dengan 5 metode penghitungan error: Variance, Mean Absolute Deviation, Max Pixel Difference, Entropy, dan Structural Similarity Index (SSIM). Program ini ditulis dalam bahasa C++ dan memanfaatkan library eksternal FreeImage. Ini merupakan penugasan IF2211 Strategi Algoritma Semester II Tahun 2024/2025.

## Requirement
1. Free Image:
```bash
sudo apt update
sudo apt install libfreeimage-dev
```

## Langkah Kompilasi
```bash
g++ -std=c++17 -o bin/quadtree src/main.cpp src/functions/image.cpp src/model/quadtree.cpp src/functions/calc_error.cpp -lfreeimage
```

## Langkah Menjalankan Program
```bash
./bin/quadtree
```
