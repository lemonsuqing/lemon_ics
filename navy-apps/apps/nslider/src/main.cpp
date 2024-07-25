#include <SDL.h>
#include <SDL_bmp.h>
#include <stdio.h>
#include <assert.h>

#define W 400
#define H 300

// USAGE:
//   j/down - page down
//   k/up - page up
//   gg - first page

// number of slides
const int N = 7;
// slides path pattern (starts from 0)
const char *path = "/share/slides/slides-%d.bmp";

static SDL_Surface *slide = NULL;
static int cur = 0;

void print_pixels(SDL_Surface *surface) {
    // 确保surface不是NULL
    assert(surface != NULL);

    // 获取像素数据
    uint32_t *pixels = (uint32_t *)surface->pixels;

    // 计算像素数量
    int num_pixels = surface->w * surface->h;

    // 遍历并打印每个像素的值
    for (int i = 0; i < num_pixels; i++) {
        printf("Pixel %d: %u\n", i, pixels[i]);
    }
}


void render() {
  // printf("if slide == NULL\n");
  if (slide) {
    // printf("slide=NULL\n");
    SDL_FreeSurface(slide);
  }
  // printf("slide != NULL\n");
  char fname[256];
  sprintf(fname, path, cur);
  printf("fname:%s\n",fname);
  slide = SDL_LoadBMP(fname);
  // printf("flags:%d  w:%d  h:%d  pithc:%d\n", slide->flags,slide->w, slide->h, slide->pitch);
//   printf("flags: %d\n", slide->flags);
//   printf("w: %d\n", slide->w);
//   printf("h: %d\n", slide->h);
//   printf("pitch: %d\n", slide->pitch);

//   if (slide->pixels) {
//       printf("pixels: %p\n", slide->pixels);
//   } else {
//       printf("pixels: NULL\n");
//   }

//   if (slide->format) {
//     printf("BitsPerPixel: %d\n", slide->format->BitsPerPixel);
//     printf("BytesPerPixel: %d\n", slide->format->BytesPerPixel);
//     printf("Rloss: %d\n", slide->format->Rloss);
//     printf("Gloss: %d\n", slide->format->Gloss);
//     printf("Bloss: %d\n", slide->format->Bloss);
//     printf("Aloss: %d\n", slide->format->Aloss);
//     printf("Rshift: %d\n", slide->format->Rshift);
//     printf("Gshift: %d\n", slide->format->Gshift);
//     printf("Bshift: %d\n", slide->format->Bshift);
//     printf("Ashift: %d\n", slide->format->Ashift);
//     printf("Rmask: %u\n", slide->format->Rmask);
//     printf("Gmask: %u\n", slide->format->Gmask);
//     printf("Bmask: %u\n", slide->format->Bmask);
//     printf("Amask: %u\n", slide->format->Amask);

//     if (slide->format->palette) {
//         printf("palette: %p\n", slide->format->palette);
//     } else {
//         printf("palette: NULL\n");
//     }
// } else {
//     printf("format: NULL\n");
// }
// print_pixels(slide);

  assert(slide);
  SDL_UpdateRect(slide, 0, 0, 0, 0);
}

void prev(int rep) {
  if (rep == 0) rep = 1;
  cur -= rep;
  if (cur < 0) cur = 0;
  render();
}

void next(int rep) {
  if (rep == 0) rep = 1;
  cur += rep;
  if (cur >= N) cur = N - 1;
  render();
}

int main() {
  SDL_Init(0);
  SDL_Surface *screen = SDL_SetVideoMode(W, H, 32, SDL_HWSURFACE);

  int rep = 0, g = 0;
  // printf("render start\n");
  render();
  // printf("render end_\n");

  while (1) {
    SDL_Event e;
    SDL_WaitEvent(&e);
    printf("e.key.keysym.sym: %d\n", e.key.keysym.sym);
    if (e.type == SDL_KEYDOWN) {
      switch(e.key.keysym.sym) {
        case SDLK_0: rep = rep * 10 + 0; break;
        case SDLK_1: rep = rep * 10 + 1; break;
        case SDLK_2: rep = rep * 10 + 2; break;
        case SDLK_3: rep = rep * 10 + 3; break;
        case SDLK_4: rep = rep * 10 + 4; break;
        case SDLK_5: rep = rep * 10 + 5; break;
        case SDLK_6: rep = rep * 10 + 6; break;
        case SDLK_7: rep = rep * 10 + 7; break;
        case SDLK_8: rep = rep * 10 + 8; break;
        case SDLK_9: rep = rep * 10 + 9; break;
        case SDLK_J:
        case SDLK_DOWN: next(rep); rep = 0; g = 0; break;
        case SDLK_K:
        case SDLK_UP: prev(rep); rep = 0; g = 0; break;
        case SDLK_G:
          g ++;
          if (g > 1) {
            prev(100000);
            rep = 0; g = 0;
          }
          break;
      }
    }
  }

  return 0;
}
