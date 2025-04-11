# Tucil2_13523128

oleh: 13523128 - Andi Farhan Hidayat

## Penjelasan

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
1. ./bin/quadtree
2. 