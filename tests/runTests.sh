#!/usr/bin/env sh

echo "Starting to run tests"

echo "Compiling..."
gcc-10 -std=c18 -O3 -o algorithm.test.out -Wall -Werror -g -lm algorithm.test.c ../src/algorithm.c ../src/strings/string_util.c ../src/strings/accents.c ../src/bitwise_operators.c
gcc-10 -std=c18 -O3 -o bitwise_operators.test.out -Wall -Werror -g -lm bitwise_operators.test.c ../src/bitwise_operators.c ../src/strings/string_util.c ../src/strings/accents.c

echo "Running tests"
echo ""

./algorithm.test.out
./bitwise_operators.test.out

echo ""

echo "All tests run"
exit 1
