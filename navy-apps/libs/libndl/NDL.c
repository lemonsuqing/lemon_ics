#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>

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

//屏幕大小
static int screen_w = 0, screen_h = 0;
//画布大小
static int canvas_w=0,canvas_h=0;
//相对于屏幕左上角的画布位置坐标
static int canvas_x=0,canvas_y=0;

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
     int buf_size = 1024; 
  char * buf = (char *)malloc(buf_size * sizeof(char));
  int fd = open("/proc/dispinfo", 0, 0);
  int ret = read(fd, buf, buf_size);
  assert(ret < buf_size); // to be cautious...
  assert(close(fd) == 0);
 
  int i = 0;
  int width = 0, height = 0;
//使用 strncmp 函数检查字符串 "WIDTH" 是否位于 buf 中 i 处开始的位置，以确保文件内容的格式正确。
  assert(strncmp(buf + i, "WIDTH", 5) == 0);
  //这一行将 i 增加 5，以跳过字符串 "WIDTH"。
  i += 5;
  for (; i < buf_size; ++i) {
      if (buf[i] == ':') { i++; break; }
      assert(buf[i] == ' ');
  }
  for (; i < buf_size; ++i) {
    //检查当前字符是否是数字字符。如果是，它跳出循环以开始解析宽度值。
      if (buf[i] >= '0' && buf[i] <= '9') break;
      assert(buf[i] == ' ');
  }
  for (; i < buf_size; ++i) {
    
      if (buf[i] >= '0' && buf[i] <= '9') {
        //检查当前字符是否是数字字符。如果是，它将当前字符的数字值添加到 width 变量中。
          width = width * 10 + buf[i] - '0';
      } else {
          break;
      }
  }
  assert(buf[i++] == '\n');
 
  assert(strncmp(buf + i, "HEIGHT", 6) == 0);
  i += 6;
  for (; i < buf_size; ++i) {
      if (buf[i] == ':') { i++; break; }
      assert(buf[i] == ' ');
  }
  for (; i < buf_size; ++i) {
      if (buf[i] >= '0' && buf[i] <= '9') break;
      assert(buf[i] == ' ');
  }
  for (; i < buf_size; ++i) {
      if (buf[i] >= '0' && buf[i] <= '9') {
          height = height * 10 + buf[i] - '0';
      } else {
          break;
      }
  }
 
  free(buf);
 
  screen_w = width;
  screen_h = height;
if (*w == 0 && *h == 0) {
    *w = screen_w;
    *h = screen_h;
  }
  canvas_w = *w;
  canvas_h = *h;
  canvas_x=(screen_w - canvas_w) / 2;
  canvas_y=(screen_h - canvas_h) / 2;
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {

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
  if(flags == 3){
    evtdev = 3;
  }
  if(flags == 4){
    evtdev = 4;
  }

  return 0;
}

void NDL_Quit() {
}
