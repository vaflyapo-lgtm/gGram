#!/usr/bin/env bash
# Сборка мода gGramo (Arch Linux)
set -e
cd "$(dirname "$0")"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"
echo "Готово: ./build/gGramo"
