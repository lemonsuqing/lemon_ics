#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"
#include <assert.h>

SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}

SDL_Surface* IMG_Load(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if(fp == NULL){
    printf("IMG_Load: file is NULL\n");
    return NULL;
  }
  fseek(fp, 0L, SEEK_END); // 将指针移到末尾
  long size = ftell(fp);

  char * buf = (char *)malloc(size);
  assert(fread(buf, size, 1, fp) == size);

  SDL_Surface * ret_surface = STBIMG_LoadFromMemory(buf, size);
  assert(ret_surface != NULL);

  fclose(fp);
  free(buf);

  return ret_surface;
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
