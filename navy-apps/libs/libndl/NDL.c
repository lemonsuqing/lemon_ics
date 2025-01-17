#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval tv;
  assert(gettimeofday(&tv, NULL) == 0);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int NDL_PollEvent(char *buf, int len) {
  // printf("evtdev = %d\n", evtdev);
  int r_len = read(evtdev, buf, len);
  // printf("NDL_PollEvent:buf=%s\n",buf);
  return r_len;
}

//画布大小
static int canvas_w=0,canvas_h=0;
// 相对于屏幕左上角的画布位置坐标
static int canvas_x=0,canvas_y=0;

// 打开一张(*w) X (*h)的画布
// 如果*w和*h均为0, 则将系统全屏幕作为画布, 并将*w和*h分别设为系统屏幕的大小
// 目前NDL_OpenCanvas()只需要记录画布的大小就可以了, 当然我们要求画布大小不能超过屏幕大小
void NDL_OpenCanvas(int *w, int *h) {
  if (*w == 0 && *h == 0) {
    canvas_w = screen_w;
    canvas_h = screen_h;
    *w = screen_w;
    *h = screen_h;
  }
  else if(*h <= screen_h && *w <= screen_w){
    canvas_w = *w;
    canvas_h = *h;
  }
  canvas_x = (screen_w - canvas_w) / 2;
  canvas_y = (screen_h - canvas_h) / 2;
  // printf("相对于屏幕左上角的画布位置坐标x:%d,y:%d\n",canvas_x,canvas_y);
}

//解析 /proc/dispinfo 文件的内容，并写入 screen_w 和 screen_h，作为屏幕大小：
static void init_dispinfo() {
  int buf_size = 1024;
  char * buf = (char *)malloc(buf_size * sizeof(char));
  int fd = open("/proc/dispinfo", 0, 0);
  int ret = read(fd, buf, buf_size);
  // printf("init_dispinfio: buf = %s, fd = %d\n", buf, fd);
  assert(ret < buf_size); // to be cautious...
  char buf_wh[40];

  char *buf_w_num = strtok(buf, ":");
  char *buf_w_date = strtok(NULL, "\n");
  char *buf_h_num = strtok(NULL, ":");
  char *buf_h_date = strtok(NULL, "\n");
  // printf("%s\n", buf_w_num );
  sscanf(buf_w_num, "%s", buf_wh);
  if(strcmp(buf_wh, "WIDTH") == 0){
    screen_w = atoi(buf_w_date);
  }

  sscanf(buf_h_num, "%s", buf_wh);
  if(strcmp(buf_wh, "HEIGHT") == 0){
    screen_h = atoi(buf_h_date);
  }

  free(buf);
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  x += (screen_w - canvas_w) / 2;
  y += (screen_h - canvas_h) / 2;
  int fb = open("/dev/fb", O_WRONLY);
  assert(fb != -1);
  //printf("drawing to %d, %08X: %d %d %d %d\n", fb, *pixels, x,y,w,h);
  size_t base_offset = (y * screen_w + x) * sizeof(uint32_t);
  size_t pixel_offset = 0;
  int j, ret_seek, ret_write;
  for (j = 0; j < h; ++ j) {
    ret_seek = lseek(fb, base_offset, SEEK_SET);
    ret_write = write(fb, pixels + pixel_offset, w * sizeof(uint32_t));
    pixel_offset += w;
    base_offset += screen_w * sizeof(uint32_t);
  }
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  // printf("evtdev = %d\n", evtdev);
  if(flags == 3){
    evtdev = 3;
  }
  init_dispinfo();
  // printf("屏幕:WIDTH : %d  HEIGHT : %d\n", screen_w, screen_h);

  return 0;
}

void NDL_Quit() {
}
