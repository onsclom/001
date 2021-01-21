/* Project 001 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

/* defines */
#define TITLE "Project 001"
#define WIDTH 256
#define HEIGHT 256
#define SCALE 3

/* global variables */
SDL_Window *window;
SDL_Renderer *renderer;

/* to do, think of smarter way */
/* possibly char struct? */
SDL_Texture *img;
int img_w, img_h;

int running = 1;

int setup() {
  /* todo: add error checking! */
  window =
      SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       WIDTH * SCALE, HEIGHT * SCALE, 0);
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED & SDL_RENDERER_PRESENTVSYNC);

  SDL_RenderSetScale(renderer, SCALE, SCALE);

  /* load image */
  img = IMG_LoadTexture(renderer, "art/character.png");
  SDL_QueryTexture(img, NULL, NULL, &img_w, &img_h);
}

int quit() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

int main() {
  setup();
  printf("HELLO WORLD!\n");
  printf("reformate?");

  SDL_Rect charRect;
  charRect.x = WIDTH / 2;
  charRect.y = HEIGHT / 2;
  charRect.w = img_w;
  charRect.h = img_h;

  while (running) {
    /* get input */
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = 0;
      }
    }

    /* update screen */
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, img, NULL, &charRect);

    SDL_RenderPresent(renderer);
  }

  quit();
}
