#include <fs.h>
#include <common.h>

// #define NR_FILES 30

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;         // 文件名
  size_t size;        // 文件大小
  size_t disk_offset; // 文件在ramdisk中的偏移
  ReadFn read;        // 读函数指针
  WriteFn write;      // 写函数指针
  size_t open_offset; // 当前读写位置
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_EVENT, FD_DISPINFO, FD_FB};

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  [FD_EVENT]  = {"/dev/events", 0, 0, events_read ,invalid_write},
  [FD_DISPINFO]  = {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
  [FD_FB]  = {"/dev/fb", 0, 0, invalid_read, fb_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_open(const char *pathname, int flags, int mode){
  for (int i = 0; i < NR_FILES; i++) {
        if (strcmp(file_table[i].name, pathname) == 0) {
            file_table[i].open_offset = 0;
            return i;
        }
    }
  panic("file %s not found", pathname);
}


size_t fs_read(int fd, void *buf, size_t len){
  ReadFn readFn = file_table[fd].read;
  if (readFn != NULL) {
    // 特殊文件处理
    size_t open_offset = file_table[fd].open_offset;
    return readFn(buf, open_offset, len);
  }
  size_t read_len = len;
  size_t open_offset = file_table[fd].open_offset;
  size_t size = file_table[fd].size;
  size_t disk_offset = file_table[fd].disk_offset;
  if (open_offset > size) return 0;
  if (open_offset + len > size) {read_len = size - open_offset;}
  ramdisk_read(buf, disk_offset + open_offset, read_len);
  file_table[fd].open_offset += read_len;
  return read_len;
}

size_t fs_write(int fd, const void *buf, size_t len) {
    size_t write_len = len;
    size_t open_offset = file_table[fd].open_offset;
    size_t size = file_table[fd].size;
    size_t disk_offset = file_table[fd].disk_offset;
    WriteFn writeFn = file_table[fd].write;
    
    if (fd == 0) {
        Log("ignore write %s", file_table[fd].name);
        return 0;
    }
    // if (open_offset > size) return 0;

    if (writeFn != NULL) {
      write_len = writeFn(buf, open_offset, write_len);
    }else{
      if (open_offset + len > size) write_len = size - open_offset;
      ramdisk_write(buf, disk_offset + open_offset, write_len);
    }
    file_table[fd].open_offset += write_len;
    return write_len;
}
int num_fs = 0;


size_t fs_lseek(int fd, size_t offset, int whence){
  if (fd <= 2) {
        Log("ignore lseek %s", file_table[fd].name);
        return 0;
  }
  // printf("fs==> offset: %d \t file_table[%d].size: %d\n",offset, fd, file_table[fd].size);
  size_t new_offset;
  /*
    对于 SEEK_SET，new_offset 直接设置为 offset。
    对于 SEEK_CUR，new_offset 设置为当前打开位置 file->open_offset 加上 offset。
    对于 SEEK_END，new_offset 设置为文件大小 file->size 加上 offset。
  */
    if (whence == SEEK_SET) {
        new_offset = offset;
    } else if (whence == SEEK_CUR) {
        new_offset = file_table[fd].open_offset + offset;
    } else if (whence == SEEK_END) {
        new_offset =  file_table[fd].size + offset;
    } else {
        Log("Invalid whence value: %d", whence);
        return -1;
    }
     // 检查新的指针位置是否在文件范围内
    file_table[fd].open_offset = new_offset;
   if (new_offset < 0 || new_offset > file_table[fd].size) {
        num_fs++;
        Log("%d:Seek position out of bounds", num_fs);
        Log("%d:Seek position out of bounds", new_offset);
        return -1;
    } 
    
     // 设置新的文件读写指针
    return file_table[fd].open_offset;
}


int fs_close(int fd){
  file_table[fd].open_offset = 0;
  return 0;
}

// void print_file_talbe(void){
//   for(int i = 0; i < NR_FILES)
// }

void init_fs() {
  // TODO: initialize the size of /dev/fb
   AM_GPU_CONFIG_T ev = io_read(AM_GPU_CONFIG);
  file_table[FD_FB].size = ev.vmemsz;
}
