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
#define O_RDONLY        ��ֻ����ʽ���ļ�
#define O_WRONLY        ��ֻд��ʽ���ļ�
#define O_RDWR          �Կɶ�д��ʽ���ļ�
						����������־����
#define O_APPEND        ����д�ļ�ʱ����ļ�β��ʼ�ƶ�
#define O_CREAT         �����򿪵��ļ����������Զ��������ļ�
#define O_TRUNC         ���ļ����ڲ����Կ�д�ķ�ʽ��ʱ���˱�־�����ļ�������Ϊ0
#define O_EXCL          ��O_CREATҲ������,�˱�־��ȥ����ļ��Ƿ����.�ļ����������������ļ�,���򽫵��´��ļ�����.
						���⣬��O_CREAT��O_EXCLͬʱ���ã��������򿪵��ļ�Ϊ�������ӣ������ļ�ʧ��
#define O_BINARY        
#define O_RANDOM        
*/

#include <fcntl.h>

#endif
