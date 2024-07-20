#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>
#include <BMP.h>

int main() {
  NDL_Init(0);
  int w, h;
  void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
  assert(bmp);
  int canvas_w, canvas_h;
  NDL_OpenCanvas(&canvas_w, &canvas_h);
  int center_x = (canvas_w - w) / 2;
  int center_y = (canvas_h - h) / 2;
  NDL_DrawRect(bmp, center_x, center_y, w, h);
  free(bmp);
  NDL_Quit();
  printf("Test ends! Spinning...\n");
  while (1);
  return 0;
}

