
#pragma once

#ifndef SYSPATH_H
#define SYSPATH_H

#include "estring.h"

namespace SysPath
{
	static const int PI_MAX_PATH = 1024;
	void Down(estring* path, const char* part);
	bool Up(estring* path);
	bool SplitFileName(const estring &file, estring *name, estring *ext);
	int GetFilesByDir(const char *foldname, estring *flist, int len);
	bool CreateDir(const char *dir);
	bool CreateFileByPath(const char *path);
};

#endif
