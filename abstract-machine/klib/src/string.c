#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  if(s != NULL){
    while(*s != '\0'){
      len++;
      s++;
    }
  }
  return len;
}

char *strcpy(char *dst, const char *src) {
    char *ret = dst;
    while(*src) {
        *dst++ = *src++;
    }
    *dst = '\0';
    return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  char *ret = dst;
  while(*dst != '\0'){
    dst++;
  }
  while (*src != '\0'){
    *dst++ = *src++;
  }
  *dst = '\0';
  return ret;
}

int strcmp(const char *s1, const char *s2) {
    while(*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while(n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}


void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
    char *dst = out;
    const char *src = in;
    while(n--) {
        *dst++ = *src++;
    }
    return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  panic("Not implemented");
}

#endif

/*
strlen(const char *s): 返回字符串s的长度。
strcpy(char *dst, const char *src): 将字符串src复制到dst。
strncpy(char *dst, const char *src, size_t n): 将字符串src的前n个字符复制到dst。
strcat(char *dst, const char *src): 将字符串src连接到dst的末尾。
strcmp(const char *s1, const char *s2): 比较两个字符串s1和s2。如果s1 < s2，返回负数；如果s1 == s2，返回0；如果s1 > s2，返回正数。
strncmp(const char *s1, const char *s2, size_t n): 比较两个字符串s1和s2的前n个字符。
memset(void *s, int c, size_t n): 将s指向的内存区域的前n个字节设置为常数c。
memmove(void *dst, const void *src, size_t n): 将src指向的内存区域的前n个字节复制到dst。如果src和dst区域重叠，memmove也能处理。
memcpy(void *out, const void *in, size_t n): 将in指向的内存区域的前n个字节复制到out。如果in和out区域重叠，memcpy的行为是未定义的。
memcmp(const void *s1, const void *s2, size_t n): 比较内存区域s1和s2的前n个字节。如果s1 < s2，返回负数；如果s1 == s2，返回0；如果s1 > s2，返回正数。
*/