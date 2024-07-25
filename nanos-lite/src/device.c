#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for (size_t i = 0; i < len; ++i)
    putch(*((char *)buf + i));
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  printf("events_read:buf=%s\n",buf);
  AM_INPUT_KEYBRD_T t = io_read(AM_INPUT_KEYBRD);
  if (t.keycode == AM_KEY_NONE) {
    *(char*)buf = '\0';
    return 0;
  }
  else{
    int res = sprintf((char *)buf, "%s %s\n", t.keydown ? "kd" : "ku", keyname[t.keycode]);
    // printf("%s\n", buf);
    return res;
  }
  
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T t = io_read(AM_GPU_CONFIG);
  char *str = (char *)buf;
  str[offset] = '\0';
  int ret = sprintf(str + offset, "WIDTH:%d\nHEIGHT:%d\n", t.width, t.height);
  return ret;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  // printf("offset : %d, len : %d\n", offset, len);
  AM_GPU_CONFIG_T fb_ctl = io_read(AM_GPU_CONFIG);
  offset /= 4;
  len /= 4;
  int w = fb_ctl.width;
  int x = offset%w, y = offset/w;
  io_write(AM_GPU_FBDRAW, x, y, (void *)buf, len, 1, 1);
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
