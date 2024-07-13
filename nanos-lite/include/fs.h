#ifndef __FS_H__
#define __FS_H__

#include <common.h>

#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
/*=============
SEEK_SET: 从文件的起始位置开始加上偏移量。
SEEK_CUR: 从当前位置开始加上偏移量。
SEEK_END: 从文件的结束位置开始减去偏移量。
  =============*/
#endif

#endif
