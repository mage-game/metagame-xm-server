

#pragma once

#ifndef CONFIGPATH_H
#define CONFIGPATH_H

#include "common/estring.h"

class ConfigPath
{
public:
	ConfigPath();
	ConfigPath(const char* path);

	ConfigPath& operator/(const char* path);
	ConfigPath& operator/(const estring& path);
	ConfigPath& operator/(const ConfigPath& path);
	const char* operator%(const char* path);
	const char* operator%(const estring& path);

	ConfigPath operator/(const char* path)const;
	ConfigPath operator/(const estring& path)const;
	ConfigPath operator/(const ConfigPath& path)const;

	operator const char*()const;
	const char* c_str()const;
	const estring& e_str()const;
private:
	estring m_buffer;
};

static const ConfigPath ROOT = "GS";
#endif
