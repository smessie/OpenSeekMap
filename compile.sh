cd src || exit
# shellcheck disable=SC2046
gcc -std=c18 -O3 -o openseekmap -Wall -Werror -g -lm main.c $(cat ../sources)
