

#pragma once

#ifndef ESTRING_H
#define ESTRING_H

#include <string>

/*
	定义统一的string，做为DLL间传递的string，避免由于STL版本不同而导致的crash

	PS：模块间字符串传递尽量使用char * ，有时为了方便而需要使用string则使用estring
*/

typedef std::string estring;

#endif


