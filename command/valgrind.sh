#!/bin/bash

ninja -C build

valgrind --leak-check=full --show-leak-kinds=all -s \
          --track-origins=yes --log-file=valgrind-out.txt \
          --suppressions=./valgrind ./build/Kromblast