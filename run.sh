rm a.out
clang-format -i main.c
cc main.c -ansi -lSDL2 -lSDL2_image && ./a.out
