rm a.out
clang-format -i main.c
cc main.c -ansi -lSDL2 -lSDL2_image -lSDL2_ttf -lm -g && ./a.out
