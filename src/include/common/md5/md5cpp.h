

#ifndef MD5CPP_H
#define MD5CPP_H

#include "common/platform/file64.h"
#include <iostream>
#include <string>
#include <memory.h>

extern "C"
{
#include "md5.h"
};

typedef unsigned char MD5[16];
std::ostream& operator<<(std::ostream& stream, const MD5& md5);
std::istream& operator>>(std::istream& stream, MD5& md5);

void MD52String(std::string* out, const MD5& in);
void String2MD5(MD5* out, const std::string& in);

size_t MD5_stream(MD5* out, std::istringstream& file, unsigned long start=0, unsigned long length=-1);
size_t MD5_buffer(MD5* out, const char* buffer, unsigned long length);

unsigned long long MD5_file64(MD5* out, int file, unsigned long long start=0, unsigned long long length=-1);
unsigned long long MD5_file64(MD5* out, const char* filename, unsigned long long start=0, unsigned long long length=-1);

struct MD5Warpper
{
	MD5Warpper(const MD5& _md5)
	{
		memcpy(md5, _md5, sizeof(md5));
	}

	MD5Warpper(const char _md5[16])
	{
		memcpy(md5, _md5, sizeof(MD5));
	}


	bool operator<(const MD5Warpper& right)const
	{
		return memcmp(this->md5, right.md5, sizeof(md5)) < 0;
	}

	operator MD5&()
	{
		return md5;
	}

	MD5 md5;
};


#endif

