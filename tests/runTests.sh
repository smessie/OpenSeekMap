#!/usr/bin/env sh

echo "Starting to run tests"

echo "Compiling..."
gcc-10 -std=c18 -O3 -o test.out -Wall -Werror -g -lm test.c algorithm.test.c bitwise_operators.test.c ../src/algorithm.c ../src/strings/string_util.c ../src/strings/accents.c ../src/bitwise_operators.c

echo "Running tests"
echo ""

./test.out

echo ""

echo "All tests run"
exit 1
