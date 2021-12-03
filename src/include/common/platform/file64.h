#pragma once

#ifndef FILE64_H
#define FILE64_H

#ifdef __unix__
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#ifdef _FEATURES_H
#error Please make sure this file included before features.h
#endif
#define _FILE_OFFSET_BITS 64

#include <sys/stat.h>
#include <unistd.h>


#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef O_RANDOM
#define O_RANDOM 0
#endif

#define fstat64(f, s) fstat(f, s)
typedef struct stat Stat64;

#endif /* __unix__ */


#ifdef _WIN32
#include <sys/stat.h>
#include <stdio.h>
#include <io.h>
#define lseek64(f, offset, dir) _lseeki64(f, offset, dir)
#define fstat64(f, s) _fstat64(f, s)
#define tell64(f) _telli64(f)
#define access(path,mode) _access(path, mode) 

typedef struct _stat64 Stat64;
#endif /* _WIN32 */


/*
#define O_RDONLY        以只读方式打开文件
#define O_WRONLY        以只写方式打开文件
#define O_RDWR          以可读写方式打开文件
						上述三个标志互斥
#define O_APPEND        当读写文件时会从文件尾开始移动
#define O_CREAT         若欲打开的文件不存在则自动建立该文件
#define O_TRUNC         若文件存在并且以可写的方式打开时，此标志会令文件长度清为0
#define O_EXCL          果O_CREAT也被设置,此标志会去检查文件是否存在.文件若不存在则建立该文件,否则将导致打开文件错误.
						此外，若O_CREAT与O_EXCL同时设置，并且欲打开的文件为符号连接，则会打开文件失败
#define O_BINARY        
#define O_RANDOM        
*/

#include <fcntl.h>

#endif
