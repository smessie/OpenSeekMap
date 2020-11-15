#!/usr/bin/env sh

echo "Starting to run tests"

echo "Compiling..."
# shellcheck disable=SC2046
gcc -std=c18 -O3 -o test.out -Wall -Werror -g -lm test.c algorithm.test.c bitwise_operators.test.c $(cat ../sources | sed 's/^/..\/src\//ig')

echo "Running tests"
echo ""

valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=definite,indirect,possible ./test.out

echo ""

echo "All tests run"
exit 1
