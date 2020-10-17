cd src || exit
# shellcheck disable=SC2046
gcc-10 -std=c18 -O3 -o openseekmap -Wall -Werror -g -lm $(cat ../sources)
