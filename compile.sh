cd src || exit
gcc-10 -std=c18 -O3 -o openseekmap -Wall -Werror -lm "$(cat ../sources)"
