/* Project 001 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>

/* defines */
#define TITLE "Project 001"
#define WIDTH 256
#define HEIGHT 256
#define SCALE 3

/* global variables */
SDL_Window *window;
SDL_Renderer *renderer;
struct Character {
  SDL_Texture *sprite;
  SDL_Rect rect;
} character;
int running = 1;
TTF_Font *font;

unsigned int frameCount = 0;
unsigned int prevFrameCount = 0;
unsigned int secondsPast = 0;

int color_white[] = {0xfb, 0xf7, 0xf3};
int color_beige[] = {0xe5, 0xb0, 0x83};
int color_green[] = {0x42, 0x6e, 0x5d};
int color_black[] = {0x20, 0x28, 0x3d};

int setup() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
                 SDL_GetError());
    return 3;
  }

  window =
      SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       WIDTH * SCALE, HEIGHT * SCALE, 0);

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED & SDL_RENDERER_PRESENTVSYNC);

  if (window == NULL || renderer == NULL) {
    printf("making window or renderer failed");
    return 3;
  }

  TTF_Init();
  font = TTF_OpenFont("assets/m5x7.ttf", 16);

  SDL_RenderSetScale(renderer, SCALE, SCALE);

  /* load char image */
  character.sprite = IMG_LoadTexture(renderer, "art/character.png");
  /* set player rect to char image size */
  SDL_QueryTexture(character.sprite, NULL, NULL, &(character.rect.w),
                   &(character.rect.h));

  character.rect.x = WIDTH / 2;
  character.rect.y = HEIGHT / 2;
}

int quit() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  TTF_Quit();
}

int writeText(char *text, int x, int y) {
  SDL_Color fontColor = {255, 255, 255, 255};
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, fontColor);
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);
  SDL_Rect textRect = {x, y - 4, 0, 0};
  SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

  SDL_DestroyTexture(textTexture);
  SDL_FreeSurface(textSurface);
}

int main() {
  setup();
  printf("HELLO WORLD!\n");

  while (running) {
    /* update */
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = 0;
      }
    }

    /* animation */
    character.rect.y = HEIGHT / 2 + sin(SDL_GetTicks() * .001) * 20;

    /* draw */
    SDL_SetRenderDrawColor(renderer, color_black[0], color_black[1],
                           color_black[2], 0);
    SDL_RenderClear(renderer);

    char fps_string[10]; /* 10 is enough for 5 digit fps */
    sprintf(fps_string, "FPS: %d", prevFrameCount);
    writeText(fps_string, 2, 2);

    SDL_RenderCopy(renderer, character.sprite, NULL, &character.rect);
    SDL_RenderPresent(renderer);
    frameCount += 1;

    if (SDL_GetTicks() / 1000 > secondsPast) {
      secondsPast = SDL_GetTicks() / 1000;
      printf("FPS: %d\n", frameCount);
      prevFrameCount = frameCount;
      frameCount = 0;
    }
  }

  quit();
}
