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
  return r_len;
}

//画布大小
static int canvas_w=0,canvas_h=0;
//相对于屏幕左上角的画布位置坐标
static int canvas_x=0,canvas_y=0;

// 打开一张(*w) X (*h)的画布
// 如果*w和*h均为0, 则将系统全屏幕作为画布, 并将*w和*h分别设为系统屏幕的大小
// 目前NDL_OpenCanvas()只需要记录画布的大小就可以了, 当然我们要求画布大小不能超过屏幕大小
void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }

   // NWM_APP logic ... 
  // printf("in the canvas\n");
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
  printf("画布的大小为宽%d X 高%d\n",canvas_w,canvas_h);
  printf("相对于屏幕左上角的画布位置坐标x:%d,y:%d\n",canvas_x,canvas_y);
}

//解析 /proc/dispinfo 文件的内容，并写入 screen_w 和 screen_h，作为屏幕大小：
static void init_dispinfo() {
  int buf_size = 1024;
  char * buf = (char *)malloc(buf_size * sizeof(char));
  int fd = open("/proc/dispinfo", 0, 0);
  int ret = read(fd, buf, buf_size);
  // printf("init_dispinfio: buf = %s\n", buf);
  assert(ret < buf_size); // to be cautious...
  char buf_wh[40];

  char *buf_w_num = strtok(buf, ":");
  char *buf_w_date = strtok(NULL, "\n");
  char *buf_h_num = strtok(NULL, ":");
  char *buf_h_date = strtok(NULL, "\n");
  printf("%s\n", buf_w_num );
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
  int fd = open("/dev/fb", 0, 0);
  size_t offset = 0;
  for (int i = 0; i < h && y + i < canvas_h; ++i) {
    offset += ((y + canvas_y) * screen_w + (x + canvas_x)) * 4;
    // printf("offset: %ld\tn: %d\tfd:%d\n", offset, 4*(w < canvas_w - x ? w : canvas_w - x), fd);
    lseek(fd, offset, SEEK_SET);
    write(fd, pixels + i * w, w*sizeof(uint32_t));
    printf("offset: %d\n", offset);
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
  printf("屏幕:WIDTH : %d  HEIGHT : %d\n", screen_w, screen_h);

  return 0;
}

void NDL_Quit() {
}
