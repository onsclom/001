/* Project 001 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* defines */
#define TITLE "Project 001"
#define WIDTH 256
#define HEIGHT 144
#define BLOCKSIZE 4
#define SCALE 3

/* global variables */
SDL_Window *window;
SDL_Renderer *renderer;
struct Character {
  SDL_Texture *sprite;
  SDL_Rect rect;
  float x;
  float y;
} character;
struct Block {
  SDL_Texture *sprite;
  SDL_Rect rect;
} block;
int running = 1;
TTF_Font *font;

unsigned int frameCount = 0;
unsigned int prevFrameCount = 0;
unsigned int secondsPast = 0;

int color_white[] = {0xfb, 0xf7, 0xf3};
int color_beige[] = {0xe5, 0xb0, 0x83};
int color_green[] = {0x42, 0x6e, 0x5d};
int color_black[] = {0x20, 0x28, 0x3d};

int blocks[WIDTH / BLOCKSIZE * HEIGHT / BLOCKSIZE];

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
  SDL_QueryTexture(character.sprite, NULL, NULL, &character.rect.w,
                   &character.rect.h);
  block.sprite = IMG_LoadTexture(renderer, "art/blocks.png");
  block.rect.w = BLOCKSIZE;
  block.rect.h = BLOCKSIZE;
  /*
SDL_QueryTexture(block.sprite, NULL, NULL, &block.rect.w, &block.rect.h);
  */

  character.x = WIDTH / 2;
  character.y = HEIGHT / 2;

  /* initialize map */
  int y = 0;
  for (y; y < HEIGHT / BLOCKSIZE; y++) {
    int x = 0;
    for (x; x < WIDTH / BLOCKSIZE; x++) {
      blocks[y * (WIDTH / BLOCKSIZE) + x] = rand() % 10 == 0;
      if (y == 0 || x == 0 || x == WIDTH / BLOCKSIZE - 1 ||
          y == HEIGHT / BLOCKSIZE - 1) {
        blocks[y * (WIDTH / BLOCKSIZE) + x] = 1;
      }
    }
  }
}

int quit() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  TTF_Quit();
}

int writeText(char *text, int x, int y) {
  SDL_Color fontColor = {color_white[0], color_white[1], color_white[2], 255};
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

int DrawBlocks() {
  int y = 0;
  for (y; y < HEIGHT / BLOCKSIZE; y++) {
    int x = 0;
    for (x; x < WIDTH / BLOCKSIZE; x++) {
      if (blocks[y * (WIDTH / BLOCKSIZE) + x] == 1) {
        SDL_Rect curBlock = {x * block.rect.w, y * block.rect.h, block.rect.w,
                             block.rect.h};

        /*get neighbor index*/
        unsigned int neighborIndex = 0;
        int neighbors[] = {0, -1, 1, 0, 0, 1, -1, 0};
        int i = 0;
        for (i; i < 4; i++) {
          int x_off = neighbors[i * 2];
          int y_off = neighbors[i * 2 + 1];

          if (x + x_off < 0 || y + y_off < 0 ||
              x + x_off >= WIDTH / BLOCKSIZE ||
              y + y_off >= HEIGHT / BLOCKSIZE) {
            /* this neighbor is outside of the map */
            neighborIndex += pow(2, i);
          } else if (blocks[(y + y_off) * (WIDTH / BLOCKSIZE) + (x + x_off)]) {
            /* this neighbor is a block */
            neighborIndex += pow(2, i);
          }
        }

        SDL_Rect spritesheetBlock = {neighborIndex * BLOCKSIZE, 0, BLOCKSIZE,
                                     BLOCKSIZE};
        SDL_RenderCopy(renderer, block.sprite, &spritesheetBlock, &curBlock);
      }
    }
  }
}

int main() {
  setup();
  unsigned int time = SDL_GetTicks();
  unsigned int prevTime = SDL_GetTicks();

  while (running) {
    time = SDL_GetTicks();
    unsigned int deltaTime = time - prevTime;
    prevTime = time;
    /* update */
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
          break;
        }
        break;
      case SDL_KEYUP:
        break;
      }
    }

    int x_dir = 0;
    int y_dir = 0;
    const Uint8 *keyboardstate = SDL_GetKeyboardState(NULL);
    if (keyboardstate[SDL_SCANCODE_LEFT]) {
      x_dir -= 1;
    }
    if (keyboardstate[SDL_SCANCODE_RIGHT]) {
      x_dir += 1;
    }
    if (keyboardstate[SDL_SCANCODE_UP]) {
      y_dir -= 1;
    }
    if (keyboardstate[SDL_SCANCODE_DOWN]) {
      y_dir += 1;
    }

    character.x = character.x + .05 * deltaTime * x_dir;
    character.y = character.y + .05 * deltaTime * y_dir;
    character.rect.x = character.x;
    character.rect.y = character.y;

    /* draw */
    SDL_SetRenderDrawColor(renderer, color_black[0], color_black[1],
                           color_black[2], 0);
    SDL_RenderClear(renderer);

    DrawBlocks();

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
